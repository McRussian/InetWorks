#include <catch.hpp>

#include "http_builder.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <utility>
#include <iterator>
#include <vector>
#include <unordered_map>

static bool EqualsIgnoreCase(std::string_view lhs, std::string_view rhs)
{
    if (lhs.size() != rhs.size())
    {
        return false;
    }
    for (std::size_t i = 0; i < lhs.size(); ++i)
    {
        if (std::tolower(static_cast<unsigned char>(lhs[i])) != std::tolower(static_cast<unsigned char>(rhs[i])))
        {
            return false;
        }
    }
    return true;
}

struct RenderedRequest
{
    std::string request_line;
    std::vector<std::pair<std::string, std::string>> headers;
    std::string body;
};

static RenderedRequest ParseRequest(const std::string &raw)
{
    auto separator = raw.find("\r\n\r\n");
    REQUIRE(separator != std::string::npos);

    RenderedRequest parsed;
    parsed.body = raw.substr(separator + 4);

    std::string header_block = raw.substr(0, separator);
    std::size_t cursor = 0;
    bool first_line = true;

    while (cursor <= header_block.size())
    {
        auto line_end = header_block.find("\r\n", cursor);
        std::size_t segment_end = (line_end == std::string::npos) ? header_block.size() : line_end;
        std::string line = header_block.substr(cursor, segment_end - cursor);

        if (first_line)
        {
            parsed.request_line = line;
            first_line = false;
        }
        else if (!line.empty())
        {
            auto colon_pos = line.find(':');
            REQUIRE(colon_pos != std::string::npos);
            std::string name = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);
            if (!value.empty() && value.front() == ' ')
            {
                value.erase(value.begin());
            }
            parsed.headers.emplace_back(std::move(name), std::move(value));
        }

        if (line_end == std::string::npos || segment_end == header_block.size())
        {
            break;
        }
        cursor = line_end + 2;
    }

    REQUIRE_FALSE(parsed.request_line.empty());
    return parsed;
}

static const std::pair<std::string, std::string> *FindHeader(const RenderedRequest &rendered, std::string_view name)
{
    auto it = std::find_if(rendered.headers.begin(), rendered.headers.end(),
                           [&](auto &header)
                           { return EqualsIgnoreCase(header.first, name); });
    if (it == rendered.headers.end())
    {
        return nullptr;
    }
    return &*it;
}

static std::string ExtractPath(std::string_view target)
{
    auto pos = target.find('?');
    if (pos == std::string::npos)
    {
        return std::string(target);
    }
    return std::string(target.substr(0, pos));
}

static std::unordered_map<std::string, std::string> ExtractQueryParams(std::string_view target)
{
    std::unordered_map<std::string, std::string> params;
    auto pos = target.find('?');
    if (pos == std::string::npos)
    {
        return params;
    }
    std::string_view query = target.substr(pos + 1);
    while (!query.empty())
    {
        auto amp = query.find('&');
        std::string_view pair = query.substr(0, amp);
        auto eq = pair.find('=');
        if (eq != std::string::npos)
        {
            params.emplace(std::string(pair.substr(0, eq)), std::string(pair.substr(eq + 1)));
        }
        if (amp == std::string::npos)
        {
            break;
        }
        query.remove_prefix(amp + 1);
    }
    return params;
}

static std::string BuildHostLine(const HttpRequest &request)
{
    std::string host_line{"Host: "};
    host_line += std::string(request.GetHost());
    if (auto port = request.GetPort())
    {
        host_line.push_back(':');
        host_line += std::to_string(*port);
    }
    return host_line;
}

TEST_CASE("http header collection updates entries case-insensitively")
{
    HttpHeaderCollection headers;
    headers.Set("Content-Type", "application/json");
    REQUIRE(headers.Size() == 1);
    REQUIRE(headers.Contains("content-type"));
    REQUIRE(headers.GetValue("CONTENT-TYPE") == "application/json");

    headers.Set("content-type", "text/plain");
    REQUIRE(headers.Size() == 1);
    REQUIRE(headers.GetValue("Content-Type") == "text/plain");

    headers.Set("X-Test", "one");
    headers.Set("x-test", "two");
    REQUIRE(headers.Size() == 2);
    REQUIRE(headers.GetValue("X-Test") == "two");

    headers.Remove("CONTENT-type");
    REQUIRE(headers.Size() == 1);
    REQUIRE_FALSE(headers.Contains("content-type"));
    REQUIRE(headers.GetValue("x-test") == "two");
}

TEST_CASE("http header collection copies string view inputs")
{
    HttpHeaderCollection headers;
    std::string header_name = "ETag";
    std::string header_value = "abc123";
    headers.Set(std::string_view(header_name), std::string_view(header_value));

    header_name.assign("Changed");
    header_value.assign("mutated");

    REQUIRE(headers.Size() == 1);
    REQUIRE(headers.Contains("etag"));
    REQUIRE(headers.GetValue("ETAG") == "abc123");
}

TEST_CASE("http header collection removing missing header keeps state")
{
    HttpHeaderCollection headers;
    headers.Set("A", "1");
    headers.Remove("b");
    REQUIRE(headers.Size() == 1);
    REQUIRE(headers.Contains("a"));
    REQUIRE(headers.GetValue("A") == "1");

    headers.Remove("A");
    REQUIRE(headers.Size() == 0);
    REQUIRE_FALSE(headers.Contains("A"));
}

TEST_CASE("http header collection retains expected entries after removals")
{
    auto request = HttpRequest::Builder{}
                       .SetHost("example.com")
                       .SetHeader("A", "1")
                       .SetHeader("B", "2")
                       .SetHeader("C", "3")
                       .RemoveHeader("b")
                       .SetHeader("D", "4")
                       .Build();

    auto rendered = ParseRequest(request.ToString());
    std::vector<std::string> non_host;
    for (const auto &header : rendered.headers)
    {
        if (!EqualsIgnoreCase(header.first, "Host"))
        {
            non_host.push_back(header.first);
        }
    }
    std::vector<std::string> expected = {"A", "C", "D"};
    auto to_lower_copy = [](std::string value)
    {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch)
                       { return static_cast<char>(std::tolower(ch)); });
        return value;
    };
    std::vector<std::string> actual_lower;
    std::transform(non_host.begin(), non_host.end(), std::back_inserter(actual_lower), to_lower_copy);
    std::vector<std::string> expected_lower;
    std::transform(expected.begin(), expected.end(), std::back_inserter(expected_lower), to_lower_copy);
    std::sort(actual_lower.begin(), actual_lower.end());
    std::sort(expected_lower.begin(), expected_lower.end());
    REQUIRE(actual_lower == expected_lower);
}

TEST_CASE("http header collection handles large workloads")
{
    HttpHeaderCollection headers;
    constexpr int kTotal = 512;
    for (int i = 0; i < kTotal; ++i)
    {
        headers.Set("Header-" + std::to_string(i), std::to_string(i));
    }

    REQUIRE(headers.Size() == kTotal);
    for (int i = 0; i < kTotal; ++i)
    {
        REQUIRE(headers.Contains("header-" + std::to_string(i)));
        REQUIRE(headers.GetValue("HEADER-" + std::to_string(i)) == std::to_string(i));
    }

    for (int i = 0; i < kTotal; i += 2)
    {
        headers.Remove("HEADER-" + std::to_string(i));
    }

    REQUIRE(headers.Size() == kTotal / 2);
    for (int i = 1; i < kTotal; i += 2)
    {
        REQUIRE(headers.Contains("Header-" + std::to_string(i)));
        REQUIRE(headers.GetValue("header-" + std::to_string(i)) == std::to_string(i));
    }
}

TEST_CASE("updating header preserves most recent casing in serialization")
{
    auto request = HttpRequest::Builder{}
                       .SetHost("example.com")
                       .SetHeader("Content-Type", "application/json")
                       .SetHeader("content-type", "text/plain")
                       .Build();

    auto rendered = ParseRequest(request.ToString());
    auto header = FindHeader(rendered, "content-type");
    REQUIRE(header);
    REQUIRE(header->first == "content-type");
    REQUIRE(header->second == "text/plain");
}

TEST_CASE("default builder settings")
{
    auto request = HttpRequest::Builder{}.SetHost("example.com").Build();

    REQUIRE(request.GetMethod() == HttpMethod::Get);
    REQUIRE(request.GetTarget() == "/");
    REQUIRE_FALSE(request.GetPort().has_value());
    REQUIRE(request.GetHeaders().Size() == 0);
    REQUIRE_FALSE(request.GetBody());

    auto rendered = ParseRequest(request.ToString());
    REQUIRE(rendered.request_line == "GET / HTTP/1.1");
    REQUIRE(rendered.body.empty());
    auto host = FindHeader(rendered, "Host");
    REQUIRE(host);
    REQUIRE(host->second == "example.com");
}

TEST_CASE("method helpers keep http version constant")
{
    auto head = HttpRequest::Builder{}.Head("/status").SetHost("vhost.local").Build();
    REQUIRE(head.GetMethod() == HttpMethod::Head);
    REQUIRE(ParseRequest(head.ToString()).request_line == "HEAD /status HTTP/1.1");

    auto options = HttpRequest::Builder{}.Options("/ops").SetHost("vhost.local").Build();
    REQUIRE(options.GetMethod() == HttpMethod::Options);
    REQUIRE(ParseRequest(options.ToString()).request_line == "OPTIONS /ops HTTP/1.1");
}

TEST_CASE("host and port rendering")
{
    auto request = HttpRequest::Builder{}
                       .SetHost("api.service")
                       .SetPort(8080)
                       .SetHeader("X-Request-Id", "42")
                       .Build();

    REQUIRE(request.GetPort().has_value());
    REQUIRE(request.GetPort().value() == 8080);
    REQUIRE(request.GetHeaders().Size() == 1);
    REQUIRE(request.GetHeaders().Contains("X-Request-Id"));
    REQUIRE(request.GetHeaders().GetValue("X-Request-Id") == "42");

    auto rendered = ParseRequest(request.ToString());
    auto host_header = FindHeader(rendered, "Host");
    REQUIRE(host_header);
    REQUIRE(host_header->second == "api.service:8080");

    auto x_request = FindHeader(rendered, "X-Request-Id");
    REQUIRE(x_request);
    REQUIRE(x_request->second == "42");
}

TEST_CASE("query parameters appended to target without clearing")
{
    HttpRequest::Builder builder;
    builder.SetQuery("q", "hello world");

    builder.SetTarget("search");
    builder.SetHost("example.com");
    builder.SetQuery("page", "10");

    auto request = builder.Build();
    auto path = ExtractPath(request.GetTarget());
    auto params = ExtractQueryParams(request.GetTarget());
    REQUIRE(path == "/search");
    REQUIRE(params.size() == 2);
    REQUIRE(params.at("q") == "hello%20world");
    REQUIRE(params.at("page") == "10");
    REQUIRE(request.GetHeaders().Size() == 0);

    builder.SetTarget("results");
    auto updated = builder.Build();
    REQUIRE(ExtractPath(updated.GetTarget()) == "/results");
    auto updated_params = ExtractQueryParams(updated.GetTarget());
    REQUIRE(updated_params == params);

    builder.ClearQuery();
    auto cleared = builder.Build();
    REQUIRE(ExtractPath(cleared.GetTarget()) == "/results");
    REQUIRE(ExtractQueryParams(cleared.GetTarget()).empty());
}

TEST_CASE("remove query by key")
{
    HttpRequest::Builder builder;
    builder.SetTarget("/items").SetHost("example.com");
    builder.SetQuery("page", "1").SetQuery("lang", "ru");

    builder.RemoveQuery("page");
    auto request = builder.Build();
    REQUIRE(ExtractPath(request.GetTarget()) == "/items");
    auto params = ExtractQueryParams(request.GetTarget());
    REQUIRE(params.size() == 1);
    REQUIRE(params.at("lang") == "ru");

    builder.SetQuery("page", "2");
    builder.RemoveQuery("lang");
    request = builder.Build();
    REQUIRE(ExtractPath(request.GetTarget()) == "/items");
    params = ExtractQueryParams(request.GetTarget());
    REQUIRE(params.size() == 1);
    REQUIRE(params.at("page") == "2");

    builder.RemoveQuery("missing");
    auto after_missing = builder.Build();
    REQUIRE(ExtractQueryParams(after_missing.GetTarget()) == params);
}

TEST_CASE("clear query on empty builder is safe")
{
    HttpRequest::Builder builder;
    builder.ClearQuery();
    auto request = builder.Build();
    REQUIRE(request.GetTarget() == "/");
}

TEST_CASE("percent encoding handles reserved characters")
{
    auto request = HttpRequest::Builder{}
                       .Get("/endpoint")
                       .SetHost("example.com")
                       .SetQuery("special", " !@#$%^&*()[]{}")
                       .SetQuery("plus", "+\n")
                       .Build();

    REQUIRE(ExtractPath(request.GetTarget()) == "/endpoint");
    auto params = ExtractQueryParams(request.GetTarget());
    REQUIRE(params.size() == 2);
    REQUIRE(params.at("special") == "%20%21%40%23%24%25%5E%26%2A%28%29%5B%5D%7B%7D");
    REQUIRE(params.at("plus") == "%2B%0A");
}

TEST_CASE("headers replace case-insensitively")
{
    auto request = HttpRequest::Builder{}
                       .SetHost("example.com")
                       .SetHeader("Accept", "text/plain")
                       .SetHeader("x-token", "abc")
                       .SetHeader("ACCEPT", "application/json")
                       .Build();

    REQUIRE(request.GetHeaders().Size() == 2);
    REQUIRE(request.GetHeaders().GetValue("accept") == "application/json");
    REQUIRE(request.GetHeaders().GetValue("x-token") == "abc");

    auto rendered = ParseRequest(request.ToString());
    auto accept = FindHeader(rendered, "accept");
    REQUIRE(accept);
    REQUIRE(accept->first == "ACCEPT");
    REQUIRE(accept->second == "application/json");
}

TEST_CASE("removing nonexistent header keeps builder state")
{
    HttpRequest::Builder builder;
    builder.SetHost("example.com");
    builder.RemoveHeader("X-Does-Not-Exist");
    auto request = builder.Build();
    REQUIRE(request.GetHost() == "example.com");
    REQUIRE(request.GetHeaders().Size() == 0);
}

TEST_CASE("removing host header resets dedicated fields")
{
    HttpRequest::Builder builder;
    builder.SetHost("example.com");
    builder.SetPort(8080);
    builder.SetHeader("X-Trace", "one");

    builder.RemoveHeader("x-trace");
    builder.RemoveHeader("HOST");

    auto request = builder.Build();
    REQUIRE(request.GetHeaders().Size() == 0);
    REQUIRE(request.GetHost().empty());
    REQUIRE_FALSE(request.GetPort().has_value());

    auto rendered = ParseRequest(request.ToString());
    auto host_header = FindHeader(rendered, "Host");
    REQUIRE(host_header);
    REQUIRE(host_header->second.empty());
}

TEST_CASE("host handled through dedicated storage")
{
    auto request = HttpRequest::Builder{}
                       .SetHeader("Host", "mirror.server")
                       .SetHeader("X-Test", "value")
                       .Build();

    REQUIRE(request.GetHost() == "mirror.server");
    REQUIRE_FALSE(request.GetPort().has_value());
    REQUIRE(request.GetHeaders().Size() == 1);
    REQUIRE(request.GetHeaders().Contains("X-Test"));
    REQUIRE_FALSE(request.GetHeaders().Contains("Host"));

    auto rendered = ParseRequest(request.ToString());
    auto host_header = FindHeader(rendered, "Host");
    REQUIRE(host_header);
    REQUIRE(host_header->second == "mirror.server");
}

TEST_CASE("host can be supplied via generic header api")
{
    auto request = HttpRequest::Builder{}.SetHeader("host", "mirror.server").SetPort(81).Build();

    REQUIRE(request.GetHost() == "mirror.server");
    REQUIRE(request.GetPort().has_value());
    REQUIRE(request.GetPort().value() == 81);
    REQUIRE_FALSE(request.GetHeaders().Contains("host"));

    auto rendered = ParseRequest(request.ToString());
    REQUIRE(FindHeader(rendered, "Host"));
    REQUIRE(FindHeader(rendered, "Host")->second == "mirror.server:81");
}

TEST_CASE("body toggles content length header")
{
    HttpRequest::Builder builder;
    builder.Post("/submit").SetHost("example.com").SetBody("payload");

    auto first = builder.Build();
    auto first_body = first.GetBody();
    REQUIRE(first_body);
    REQUIRE(*first_body == "payload");
    REQUIRE(first.GetHeaders().GetValue("Content-Length") == "7");

    auto rendered = ParseRequest(first.ToString());
    REQUIRE(FindHeader(rendered, "Content-Length"));
    REQUIRE(FindHeader(rendered, "Content-Length")->second == "7");
    REQUIRE(rendered.body == "payload");

    builder.SetBody("next");
    auto second = builder.Build();
    auto second_body = second.GetBody();
    REQUIRE(second_body);
    REQUIRE(*second_body == "next");
    REQUIRE(second.GetHeaders().GetValue("Content-Length") == "4");

    rendered = ParseRequest(second.ToString());
    REQUIRE(FindHeader(rendered, "Content-Length"));
    REQUIRE(FindHeader(rendered, "Content-Length")->second == "4");
}

TEST_CASE("no body removes content length header")
{
    HttpRequest::Builder builder;
    builder.Post("/submit").SetHost("example.com").SetBody("payload");

    builder.SetNoBody();
    auto request = builder.Build();
    REQUIRE_FALSE(request.GetBody());
    REQUIRE(request.GetHeaders().Size() == 0);

    auto rendered = ParseRequest(request.ToString());
    REQUIRE_FALSE(FindHeader(rendered, "Content-Length"));
    REQUIRE(rendered.body.empty());
}

TEST_CASE("empty body still sets content length")
{
    auto request = HttpRequest::Builder{}
                       .Post("/submit")
                       .SetHost("example.com")
                       .SetBody("")
                       .Build();

    auto empty_body = request.GetBody();
    REQUIRE(empty_body);
    REQUIRE(empty_body->empty());
    REQUIRE(request.GetHeaders().Size() == 1);
    REQUIRE(request.GetHeaders().GetValue("content-length") == "0");

    auto rendered = ParseRequest(request.ToString());
    REQUIRE(FindHeader(rendered, "Content-Length"));
    REQUIRE(FindHeader(rendered, "Content-Length")->second == "0");
}

TEST_CASE("builder reuse does not mutate previous requests")
{
    HttpRequest::Builder builder;
    builder.Post("/upload").SetHost("example.com").SetBody("data");

    auto first = builder.Build();
    REQUIRE(first.GetMethod() == HttpMethod::Post);
    REQUIRE(first.GetTarget() == "/upload");
    auto first_body_again = first.GetBody();
    REQUIRE(first_body_again);
    REQUIRE(*first_body_again == "data");
    REQUIRE(first.GetHeaders().GetValue("Content-Length") == "4");

    builder.Get("/status").SetNoBody();
    auto second = builder.Build();
    REQUIRE(second.GetMethod() == HttpMethod::Get);
    REQUIRE(second.GetTarget() == "/status");
    REQUIRE_FALSE(second.GetBody());
    REQUIRE(second.GetHeaders().Size() == 0);

    REQUIRE(first.GetTarget() == "/upload");
    auto first_body_after = first.GetBody();
    REQUIRE(first_body_after);
    REQUIRE(*first_body_after == "data");
    REQUIRE(first.GetHeaders().GetValue("Content-Length") == "4");
}

TEST_CASE("builder reset to defaults")
{
    HttpRequest::Builder builder;
    builder.Post("/data").SetHost("example.com").SetPort(9000).SetBody("abc");

    builder.Reset();
    auto request = builder.Build();

    REQUIRE(request.GetMethod() == HttpMethod::Get);
    REQUIRE(request.GetTarget() == "/");
    REQUIRE(request.GetHost().empty());
    REQUIRE_FALSE(request.GetPort().has_value());
    REQUIRE(request.GetHeaders().Size() == 0);
    REQUIRE_FALSE(request.GetBody());
}

TEST_CASE("target normalization ensures leading slash")
{
    auto request_empty = HttpRequest::Builder{}.SetTarget("").SetHost("example.com").Build();
    REQUIRE(request_empty.GetTarget() == "/");

    auto request_relative = HttpRequest::Builder{}.SetTarget("status").SetHost("example.com").Build();
    REQUIRE(request_relative.GetTarget() == "/status");
}

TEST_CASE("string rendering contains all parts regardless of header order")
{
    auto request = HttpRequest::Builder{}
                       .Put("resource")
                       .SetHost("example.com")
                       .SetHeader("X-One", "1")
                       .SetHeader("X-Two", "2")
                       .SetBody("abc")
                       .Build();

    auto rendered = ParseRequest(request.ToString());
    REQUIRE(rendered.request_line == "PUT /resource HTTP/1.1");
    REQUIRE(rendered.body == "abc");

    auto host = FindHeader(rendered, "HOST");
    REQUIRE(host);
    REQUIRE(host->second == "example.com");

    auto first_header = FindHeader(rendered, "X-One");
    auto second_header = FindHeader(rendered, "X-Two");
    REQUIRE(first_header);
    REQUIRE(second_header);
    REQUIRE(FindHeader(rendered, "Content-Length"));
    REQUIRE(FindHeader(rendered, "Content-Length")->second == "3");
}

TEST_CASE("explicit method setter overrides helper")
{
    auto request = HttpRequest::Builder{}
                       .SetTarget("explicit")
                       .SetMethod(HttpMethod::Patch)
                       .SetHost("example.com")
                       .Build();

    REQUIRE(request.GetMethod() == HttpMethod::Patch);
    REQUIRE(request.GetTarget() == "/explicit");
}

TEST_CASE("options method with query and headers")
{
    auto request = HttpRequest::Builder{}
                       .Options("status")
                       .SetHost("example.com")
                       .SetHeader("Cache-Control", "no-cache")
                       .SetQuery("pretty", "true")
                       .Build();

    REQUIRE(request.GetMethod() == HttpMethod::Options);
    REQUIRE(ExtractPath(request.GetTarget()) == "/status");
    auto options_params = ExtractQueryParams(request.GetTarget());
    REQUIRE(options_params.size() == 1);
    REQUIRE(options_params.at("pretty") == "true");
    auto rendered = ParseRequest(request.ToString());
    REQUIRE(FindHeader(rendered, "Cache-Control"));
    REQUIRE(FindHeader(rendered, "Cache-Control")->second == "no-cache");
}

TEST_CASE("request without host keeps header placeholder")
{
    auto request = HttpRequest::Builder{}.Build();

    REQUIRE(request.GetHost().empty());

    auto rendered = ParseRequest(request.ToString());
    auto host_header = FindHeader(rendered, "Host");
    REQUIRE(host_header);
    REQUIRE(host_header->second.empty());
}

TEST_CASE("host string_view overload copies data")
{
    std::string host = "svc.internal";
    auto request = HttpRequest::Builder{}
                       .SetHost(std::string_view(host).substr(0, 3))
                       .SetPort(443)
                       .Build();

    REQUIRE(request.GetHost() == "svc");
    REQUIRE(request.GetPort().has_value());
    REQUIRE(request.GetPort().value() == 443);
}

TEST_CASE("build host header helper")
{
    auto request = HttpRequest::Builder{}.SetHost("api").SetPort(9000).Build();

    REQUIRE(BuildHostLine(request) == "Host: api:9000");
}

TEST_CASE("stress builder with many headers and query params")
{
    HttpRequest::Builder builder;
    builder.SetHost("example.com");

    for (int i = 0; i < 100; ++i)
    {
        builder.SetQuery("k" + std::to_string(i), "v" + std::to_string(i));
        builder.SetHeader("X-Header-" + std::to_string(i), std::to_string(i));
    }

    builder.SetBody(std::string(256, 'x'));

    auto request = builder.Build();
    REQUIRE(request.GetHeaders().Size() == 101); // +1 for Content-Length
    auto params = ExtractQueryParams(request.GetTarget());
    REQUIRE(params.size() == 100);
    for (int i = 0; i < 100; ++i)
    {
        auto key = "k" + std::to_string(i);
        auto it = params.find(key);
        REQUIRE(it != params.end());
        REQUIRE(it->second == "v" + std::to_string(i));
    }
    REQUIRE(std::count(request.GetTarget().begin(), request.GetTarget().end(), '&') == 99);
    auto bulk_body = request.GetBody();
    REQUIRE(bulk_body);
    REQUIRE(bulk_body->size() == 256);

    auto rendered = ParseRequest(request.ToString());
    REQUIRE(rendered.body.size() == 256);
    REQUIRE(FindHeader(rendered, "Content-Length"));
    REQUIRE(FindHeader(rendered, "Content-Length")->second == "256");
    REQUIRE(FindHeader(rendered, "X-Header-0"));
    REQUIRE(FindHeader(rendered, "X-Header-99"));
}
