#include <catch.hpp>

#include "search.h"

#include <random>
#include <string>
#include <vector>

#include "time_limiter.h"

namespace {

bool Belongs(std::string_view source, std::string_view view) {
    return (view.data() >= source.data()) &&
           (view.data() + view.size() <= source.data() + source.size());
}

void CheckBelongs(const std::vector<std::string_view>& docs,
                  const std::vector<std::string_view>& results) {
    for (const auto& result : results) {
        bool found = false;
        for (const auto& doc : docs) {
            if (Belongs(doc, result)) {
                found = true;
                break;
            }
        }
        REQUIRE(found);
    }
}

// Helper: build index from a list of documents
SearchIndex BuildIndex(const std::vector<std::string_view>& docs) {
    SearchIndex index;
    for (const auto& doc : docs) {
        index.Add(doc);
    }
    return index;
}

}  // namespace

TEST_CASE("Basic search") {
    std::vector<std::string_view> docs = {
        "the cat sat on the mat",
        "a dog chased a cat",
        "birds fly in the sky",
    };
    auto index = BuildIndex(docs);

    auto results = index.Query("cat", 2);

    REQUIRE(results.size() == 2);
    // TF(cat) in "a dog chased a cat" = 1/5, in "the cat sat on the mat" = 1/6
    // IDF(cat) = log(3/2)
    REQUIRE(results[0] == "a dog chased a cat");
    REQUIRE(results[1] == "the cat sat on the mat");
    CheckBelongs(docs, results);
}

TEST_CASE("Multiple query words") {
    std::vector<std::string_view> docs = {
        "apple banana cherry",
        "apple date elderberry",
        "fig grape apple",
        "banana fig hazelnut",
    };
    auto index = BuildIndex(docs);

    // cherry: df=1, IDF=log(4)~1.39
    // banana: df=2, IDF=log(2)~0.69
    // Line 0: TF(cherry)=1/3, TF(banana)=1/3 => rel = 1/3*log(4) + 1/3*log(2) ~ 0.694
    // Line 3: TF(banana)=1/3 => rel = 1/3*log(2) ~ 0.231
    auto results = index.Query("cherry banana", 5);

    REQUIRE(results.size() == 2);
    REQUIRE(results[0] == "apple banana cherry");
    REQUIRE(results[1] == "banana fig hazelnut");
    CheckBelongs(docs, results);
}

TEST_CASE("Results count exceeds relevant docs") {
    std::vector<std::string_view> docs = {
        "alpha beta",
        "gamma delta",
        "epsilon zeta",
    };
    auto index = BuildIndex(docs);

    auto results = index.Query("alpha", 100);

    REQUIRE(results.size() == 1);
    REQUIRE(results[0] == "alpha beta");
}

TEST_CASE("Case insensitivity") {
    std::vector<std::string_view> docs = {
        "The Cat sat on the mat",
        "A dog chased a CAT",
        "Birds fly in the sky",
    };
    auto index = BuildIndex(docs);

    auto results = index.Query("CAT", 3);

    REQUIRE(results.size() == 2);
    // "A dog chased a CAT" TF=1/5, "The Cat sat on the mat" TF=1/6
    REQUIRE(results[0] == "A dog chased a CAT");
    REQUIRE(results[1] == "The Cat sat on the mat");
    CheckBelongs(docs, results);
}

TEST_CASE("Word not found") {
    std::vector<std::string_view> docs = {"hello world", "foo bar"};
    auto index = BuildIndex(docs);

    auto results = index.Query("xyznonexistent", 5);
    REQUIRE(results.empty());
}

TEST_CASE("Empty query") {
    std::vector<std::string_view> docs = {"hello world"};
    auto index = BuildIndex(docs);

    REQUIRE(index.Query("", 5).empty());
    REQUIRE(index.Query("   ", 5).empty());
    REQUIRE(index.Query("123!@#", 5).empty());
}

TEST_CASE("No documents") {
    SearchIndex index;
    REQUIRE(index.Query("hello", 5).empty());
}

TEST_CASE("Zero results requested") {
    std::vector<std::string_view> docs = {"hello world", "foo bar"};
    auto index = BuildIndex(docs);

    REQUIRE(index.Query("hello", 0).empty());
}

TEST_CASE("Stable ordering for equal relevance") {
    std::vector<std::string_view> docs = {
        "alpha beta gamma",
        "delta epsilon zeta",
        "alpha beta eta",
    };
    auto index = BuildIndex(docs);

    // Lines 0 and 2 both have TF(alpha)=1/3, TF(beta)=1/3
    // Equal relevance => order of addition preserved
    auto results = index.Query("alpha beta", 3);

    REQUIRE(results.size() == 2);
    REQUIRE(results[0] == "alpha beta gamma");
    REQUIRE(results[1] == "alpha beta eta");
}

TEST_CASE("Punctuation splits words") {
    std::vector<std::string_view> docs = {
        "hello,world foo",
        "bar baz",
        "hello world again",
    };
    auto index = BuildIndex(docs);

    // "hello,world" => two words: "hello" and "world"
    auto results = index.Query("hello", 3);

    REQUIRE(results.size() == 2);
    REQUIRE(results[0] == "hello,world foo");
    REQUIRE(results[1] == "hello world again");
}

TEST_CASE("Digits are not words") {
    std::vector<std::string_view> docs = {
        "abc 123 def",
        "456 ghi",
        "789",
    };
    auto index = BuildIndex(docs);

    // "789" has no words => not a document, N=2
    auto results = index.Query("ghi", 5);

    REQUIRE(results.size() == 1);
    REQUIRE(results[0] == "456 ghi");
}

TEST_CASE("Document without words is not counted") {
    std::vector<std::string_view> docs = {
        "hello world",
        "123 456 !!!",
        "goodbye world",
    };
    auto index = BuildIndex(docs);

    // "123 456 !!!" has no alpha words => N=2
    // "world" is in both real docs => IDF = log(2/2) = 0
    // "hello" in 1 doc => IDF = log(2/1) > 0
    auto results = index.Query("world", 5);

    // IDF("world") = 0 => all relevances are 0
    REQUIRE(results.empty());

    auto results2 = index.Query("hello", 5);
    REQUIRE(results2.size() == 1);
    REQUIRE(results2[0] == "hello world");
}

TEST_CASE("IDF zero for ubiquitous word") {
    std::vector<std::string_view> docs = {
        "the cat sleeps",
        "the dog runs",
        "the bird flies",
    };
    auto index = BuildIndex(docs);

    // "the" in all 3 => IDF = 0
    REQUIRE(index.Query("the", 5).empty());

    // "the cat" => only "cat" contributes (IDF > 0)
    auto results = index.Query("the cat", 5);
    REQUIRE(results.size() == 1);
    REQUIRE(results[0] == "the cat sleeps");
}

TEST_CASE("Duplicate words in query") {
    std::vector<std::string_view> docs = {
        "cat dog bird",
        "cat cat fish",
        "dog bird fish",
    };
    auto index = BuildIndex(docs);

    auto results1 = index.Query("cat", 5);
    auto results2 = index.Query("cat cat cat", 5);

    REQUIRE(results1 == results2);
}

TEST_CASE("Single document") {
    std::vector<std::string_view> docs = {"hello world"};
    auto index = BuildIndex(docs);

    // N=1, every word has df=N => IDF=0
    REQUIRE(index.Query("hello", 5).empty());
}

TEST_CASE("Repeated word in document boosts TF") {
    std::vector<std::string_view> docs = {
        "cat cat cat dog",
        "cat bird fish frog",
        "dog bird fish frog",
    };
    auto index = BuildIndex(docs);

    // df(cat)=2, IDF=log(3/2)
    // Line 0: TF(cat) = 3/4
    // Line 1: TF(cat) = 1/4
    auto results = index.Query("cat", 5);

    REQUIRE(results.size() == 2);
    REQUIRE(results[0] == "cat cat cat dog");
    REQUIRE(results[1] == "cat bird fish frog");
}

TEST_CASE("Query with non-alpha characters") {
    std::vector<std::string_view> docs = {
        "cpp is great",
        "python is ok",
        "rust is fast",
    };
    auto index = BuildIndex(docs);

    // "c++ is great!!!" => words: "c", "is", "great"
    // "is" in all 3 => IDF=0
    // "great" in 1 => IDF=log(3)
    // "c" in 0 => ignored
    auto results = index.Query("c++ is great!!!", 5);

    REQUIRE(results.size() == 1);
    REQUIRE(results[0] == "cpp is great");
}

TEST_CASE("Multiple searches on same index") {
    std::vector<std::string_view> docs = {
        "the quick brown fox",
        "the lazy brown dog",
        "a swift red fox",
    };
    auto index = BuildIndex(docs);

    auto r1 = index.Query("fox", 5);
    REQUIRE(r1.size() == 2);
    // Both have TF(fox)=1/4, equal relevance => order of addition
    REQUIRE(r1[0] == "the quick brown fox");
    REQUIRE(r1[1] == "a swift red fox");

    auto r2 = index.Query("dog", 5);
    REQUIRE(r2.size() == 1);
    REQUIRE(r2[0] == "the lazy brown dog");

    auto r3 = index.Query("brown", 5);
    REQUIRE(r3.size() == 2);
    // Both have TF(brown)=1/4, equal relevance => order of addition
    REQUIRE(r3[0] == "the quick brown fox");
    REQUIRE(r3[1] == "the lazy brown dog");

    CheckBelongs(docs, r1);
    CheckBelongs(docs, r2);
    CheckBelongs(docs, r3);
}

TEST_CASE("Adding documents incrementally") {
    SearchIndex index;

    std::string_view doc1 = "alpha beta";
    std::string_view doc2 = "gamma delta";

    index.Add(doc1);
    // With only 1 doc, everything has IDF=0
    REQUIRE(index.Query("alpha", 5).empty());

    index.Add(doc2);
    // Now N=2, alpha is in 1 doc => IDF > 0
    auto results = index.Query("alpha", 5);
    REQUIRE(results.size() == 1);
    REQUIRE(results[0] == "alpha beta");
}

TEST_CASE("Empty and whitespace documents") {
    SearchIndex index;
    index.Add("");
    index.Add("   ");
    index.Add("\t\t");
    index.Add("hello");
    index.Add("world");

    // Only "hello" and "world" are real documents, N=2
    auto results = index.Query("hello", 5);
    REQUIRE(results.size() == 1);
    REQUIRE(results[0] == "hello");
}

TEST_CASE("Large number of documents") {
    SearchIndex index;
    std::vector<std::string> storage;
    storage.reserve(10001);

    for (int i = 0; i < 10000; ++i) {
        storage.push_back("common word extra padding text number" + std::to_string(i));
    }
    storage.push_back("unique special target words here");

    for (const auto& doc : storage) {
        index.Add(doc);
    }

    auto results = index.Query("unique special", 3);
    REQUIRE(results.size() == 1);
    REQUIRE(results[0] == "unique special target words here");
}

TEST_CASE("Result views point into original documents") {
    std::string_view doc1 = "one two three";
    std::string_view doc2 = "four five six";
    std::string_view doc3 = "seven eight nine";

    SearchIndex index;
    index.Add(doc1);
    index.Add(doc2);
    index.Add(doc3);

    auto results = index.Query("four seven", 5);

    REQUIRE(results.size() == 2);
    for (const auto& r : results) {
        REQUIRE((Belongs(doc1, r) || Belongs(doc2, r) || Belongs(doc3, r)));
    }
}

TEST_CASE("Stress: many documents, many queries") {
    std::mt19937 rng(42);

    auto random_word = [&rng]() {
        std::string w;
        int len = 2 + static_cast<int>(rng() % 3);
        for (int j = 0; j < len; ++j) {
            w.push_back(static_cast<char>('a' + rng() % 26));
        }
        return w;
    };

    SearchIndex index;
    std::vector<std::string> storage;
    storage.reserve(100000);

    for (int i = 0; i < 100000; ++i) {
        std::string doc;
        int words = 3 + static_cast<int>(rng() % 4);
        for (int j = 0; j < words; ++j) {
            if (j > 0) {
                doc += ' ';
            }
            doc += random_word();
        }
        storage.push_back(std::move(doc));
    }

    for (const auto& doc : storage) {
        index.Add(doc);
    }

    std::vector<std::string> queries;
    queries.reserve(100000);
    for (int i = 0; i < 100000; ++i) {
        queries.push_back(random_word());
    }

    using namespace std::chrono_literals;
    TimeLimiter limiter(5s);

    for (const auto& q : queries) {
        auto results = index.Query(q, 3);
        REQUIRE(results.size() <= 3);
    }

    CHECK(limiter.ElapsedCPUTime() < 5s);
    CHECK(limiter.ElapsedWallTime() < 5s);
}
