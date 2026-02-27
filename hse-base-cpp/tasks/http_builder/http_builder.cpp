#include "http_builder.h"

#include <stdexcept>

std::string_view HttpHeaderCollection::GetValue(std::string_view name) const {
    static_cast<void>(name);
    return "";
}

void HttpHeaderCollection::Set(std::string_view name, std::string_view value) {
    static_cast<void>(name);
    static_cast<void>(value);
}

void HttpHeaderCollection::Remove(std::string_view name) {
    static_cast<void>(name);
}

std::size_t HttpHeaderCollection::Size() const {
    return 0;
}

bool HttpHeaderCollection::Contains(std::string_view name) const {
    static_cast<void>(name);
    return false;
}

const HttpHeaderCollection::Storage &HttpHeaderCollection::Items() const {
    return {};
}

HttpRequest::HttpRequest() {
}

std::string HttpRequest::ToString() const {
    return {};
}

HttpMethod HttpRequest::GetMethod() const {
    return HttpMethod::Get;
}

std::string_view HttpRequest::GetTarget() const {
    return "";
}

std::string_view HttpRequest::GetHost() const {
    return "";
}

std::optional<std::uint16_t> HttpRequest::GetPort() const {
    return std::nullopt;
}

const HttpHeaderCollection &HttpRequest::GetHeaders() const {
    return {};
}

std::optional<std::string_view> HttpRequest::GetBody() const {
    return std::nullopt;
}

HttpRequest::Builder::Builder() {
}

HttpRequest::Builder &HttpRequest::Builder::Reset() {
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetMethod(HttpMethod method) {
    static_cast<void>(method);
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetTarget(std::string_view target) {
    static_cast<void>(target);
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::Get(std::string_view target) {
    SetMethod(HttpMethod::Get);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::Head(std::string_view target) {
    SetMethod(HttpMethod::Head);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::Post(std::string_view target) {
    SetMethod(HttpMethod::Post);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::Put(std::string_view target) {
    SetMethod(HttpMethod::Put);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::Delete(std::string_view target) {
    SetMethod(HttpMethod::Delete);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::Patch(std::string_view target) {
    SetMethod(HttpMethod::Patch);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::Options(std::string_view target) {
    SetMethod(HttpMethod::Options);
    return SetTarget(std::move(target));
}

HttpRequest::Builder &HttpRequest::Builder::SetHost(std::string_view host) {
    static_cast<void>(host);
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetPort(std::uint16_t port) {
    static_cast<void>(port);
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetQuery(std::string_view key, std::string_view value) {
    static_cast<void>(key);
    static_cast<void>(value);
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::ClearQuery() {
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::RemoveQuery(std::string_view key) {
    static_cast<void>(key);
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetHeader(std::string_view name, std::string_view value) {
    static_cast<void>(name);
    static_cast<void>(value);
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::RemoveHeader(std::string_view name) {
    static_cast<void>(name);
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetBody(std::string_view data) {
    static_cast<void>(data);
    return *this;
}

HttpRequest::Builder &HttpRequest::Builder::SetNoBody() {
    return *this;
}

HttpRequest HttpRequest::Builder::Build() const {
    throw std::logic_error("HttpRequest::Builder::Build is not implemented");
}
