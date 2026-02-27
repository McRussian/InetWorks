#include <catch.hpp>

#include "bpe_tokenizer.h"

#include <string>
#include <vector>

namespace {

using Rules = std::vector<std::pair<std::string, std::string>>;
using Tokens = std::vector<std::string>;

std::string MakeCorpus(const std::vector<std::pair<std::string, int>>& word_freqs) {
    std::string corpus;
    for (const auto& [word, freq] : word_freqs) {
        for (int i = 0; i < freq; ++i) {
            if (!corpus.empty()) {
                corpus += ' ';
            }
            corpus += word;
        }
    }
    return corpus;
}

}  // namespace

TEST_CASE("Empty corpus") {
    BPETokenizer tok;
    tok.Train("", 10);

    CHECK(tok.GetMergeRules().empty());
    CHECK(tok.Encode("hello") == Tokens{"h", "e", "l", "l", "o"});
    CHECK(tok.Encode("") == Tokens{});
}

TEST_CASE("Encode empty word") {
    BPETokenizer tok;
    tok.Train("abc abc", 1);

    CHECK(tok.Encode("") == Tokens{});
}

TEST_CASE("Single character words") {
    BPETokenizer tok;
    tok.Train("a b c a b", 5);

    // All words are single characters — no adjacent pairs to merge
    CHECK(tok.GetMergeRules().empty());
    CHECK(tok.Encode("a") == Tokens{"a"});
    CHECK(tok.Encode("abc") == Tokens{"a", "b", "c"});
}

TEST_CASE("Single merge") {
    BPETokenizer tok;
    tok.Train("ab ab ab", 1);

    REQUIRE(tok.GetMergeRules().size() == 1);
    CHECK(tok.GetMergeRules()[0] == std::pair<std::string, std::string>{"a", "b"});

    CHECK(tok.Encode("ab") == Tokens{"ab"});
    CHECK(tok.Encode("abc") == Tokens{"ab", "c"});
    CHECK(tok.Encode("cab") == Tokens{"c", "ab"});
    CHECK(tok.Encode("ba") == Tokens{"b", "a"});
}

TEST_CASE("Word frequencies affect merge order") {
    // "ab" appears 3 times, "cd" appears 1 time
    // (a,b) has count 3, (c,d) has count 1
    BPETokenizer tok;
    tok.Train("ab cd ab ab", 1);

    REQUIRE(tok.GetMergeRules().size() == 1);
    CHECK(tok.GetMergeRules()[0] == std::pair<std::string, std::string>{"a", "b"});

    CHECK(tok.Encode("ab") == Tokens{"ab"});
    CHECK(tok.Encode("cd") == Tokens{"c", "d"});
}

TEST_CASE("Same pair multiple times in one word") {
    // "abab" has pairs: (a,b) at pos 0-1, (b,a) at pos 1-2, (a,b) at pos 2-3
    // (a,b) count: 2, (b,a) count: 1
    BPETokenizer tok;
    tok.Train("abab", 1);

    REQUIRE(tok.GetMergeRules().size() == 1);
    CHECK(tok.GetMergeRules()[0] == std::pair<std::string, std::string>{"a", "b"});

    CHECK(tok.Encode("abab") == Tokens{"ab", "ab"});
    CHECK(tok.Encode("aba") == Tokens{"ab", "a"});
}

TEST_CASE("Tie-breaking by lexicographic order") {
    // "ab" and "cd" each appear once → (a,b) and (c,d) both have count 1
    // (a,b) < (c,d) lexicographically
    BPETokenizer tok;
    tok.Train("ab cd", 1);

    REQUIRE(tok.GetMergeRules().size() == 1);
    CHECK(tok.GetMergeRules()[0] == std::pair<std::string, std::string>{"a", "b"});
}

TEST_CASE("Tie-breaking same left element") {
    // "ab" and "ac" each appear once → (a,b) and (a,c) both have count 1
    // (a,b) < (a,c) because b < c
    BPETokenizer tok;
    tok.Train("ab ac", 1);

    REQUIRE(tok.GetMergeRules().size() == 1);
    CHECK(tok.GetMergeRules()[0] == std::pair<std::string, std::string>{"a", "b"});
}

TEST_CASE("Two merges") {
    // "abc" × 3
    // Step 1: (a,b):3 and (b,c):3 tie → (a,b) wins → merge → [ab, c]
    // Step 2: (ab,c):3 → merge → [abc]
    BPETokenizer tok;
    tok.Train("abc abc abc", 2);

    Rules expected_rules = {{"a", "b"}, {"ab", "c"}};
    CHECK(tok.GetMergeRules() == expected_rules);

    CHECK(tok.Encode("abc") == Tokens{"abc"});
    CHECK(tok.Encode("abcab") == Tokens{"abc", "ab"});
    CHECK(tok.Encode("ab") == Tokens{"ab"});
    CHECK(tok.Encode("bc") == Tokens{"b", "c"});
}

TEST_CASE("Three merges — worked example from README") {
    std::string corpus = MakeCorpus({{"low", 5}, {"lowest", 2}, {"newer", 6}, {"wider", 3}});
    BPETokenizer tok;
    tok.Train(corpus, 3);

    // Step 1: (e,r) = 6+3 = 9
    // Step 2: (l,o) = 5+2 = 7, (o,w) = 5+2 = 7, tie → (l,o) wins
    // Step 3: (lo,w) = 5+2 = 7
    Rules expected_rules = {{"e", "r"}, {"l", "o"}, {"lo", "w"}};
    CHECK(tok.GetMergeRules() == expected_rules);

    CHECK(tok.Encode("lower") == Tokens{"low", "er"});
    CHECK(tok.Encode("newest") == Tokens{"n", "e", "w", "e", "s", "t"});
    CHECK(tok.Encode("lowest") == Tokens{"low", "e", "s", "t"});
    CHECK(tok.Encode("low") == Tokens{"low"});
    CHECK(tok.Encode("wider") == Tokens{"w", "i", "d", "er"});
    CHECK(tok.Encode("newer") == Tokens{"n", "e", "w", "er"});
}

TEST_CASE("More merges than possible") {
    BPETokenizer tok;
    tok.Train("ab", 10);

    // Only 1 merge is possible: (a,b)
    REQUIRE(tok.GetMergeRules().size() == 1);
    CHECK(tok.GetMergeRules()[0] == std::pair<std::string, std::string>{"a", "b"});

    CHECK(tok.Encode("ab") == Tokens{"ab"});
}

TEST_CASE("Encode unseen word") {
    BPETokenizer tok;
    tok.Train("ab ab ab", 1);
    // Rule: (a,b)

    // "xyz" has no applicable rules
    CHECK(tok.Encode("xyz") == Tokens{"x", "y", "z"});

    // "xab" has (a,b) at positions 1-2
    CHECK(tok.Encode("xab") == Tokens{"x", "ab"});

    // "aby" has (a,b) at positions 0-1
    CHECK(tok.Encode("aby") == Tokens{"ab", "y"});
}

TEST_CASE("Overlapping pairs — left-to-right greedy") {
    // "aaa": [a, a, a], pairs: (a,a) × 2
    BPETokenizer tok;
    tok.Train("aaa", 1);

    REQUIRE(tok.GetMergeRules().size() == 1);
    CHECK(tok.GetMergeRules()[0] == std::pair<std::string, std::string>{"a", "a"});

    // Left-to-right greedy: merge first (a,a), skip merged, continue
    CHECK(tok.Encode("aa") == Tokens{"aa"});
    CHECK(tok.Encode("aaa") == Tokens{"aa", "a"});
    CHECK(tok.Encode("aaaa") == Tokens{"aa", "aa"});
    CHECK(tok.Encode("aaaaa") == Tokens{"aa", "aa", "a"});
}

TEST_CASE("Train resets previous state") {
    BPETokenizer tok;

    tok.Train("ab ab ab", 1);
    REQUIRE(tok.GetMergeRules().size() == 1);
    CHECK(tok.GetMergeRules()[0] == std::pair<std::string, std::string>{"a", "b"});

    tok.Train("cd cd cd", 1);
    REQUIRE(tok.GetMergeRules().size() == 1);
    CHECK(tok.GetMergeRules()[0] == std::pair<std::string, std::string>{"c", "d"});

    CHECK(tok.Encode("ab") == Tokens{"a", "b"});
    CHECK(tok.Encode("cd") == Tokens{"cd"});
}

TEST_CASE("Single word corpus") {
    // "hello" × 1
    // Pairs: (h,e):1, (e,l):1, (l,l):1, (l,o):1
    // Tie → lexicographic: (e,l) < (h,e) < (l,l) < (l,o)
    // Step 1: merge (e,l) → [h, el, l, o]
    // Pairs: (h,el):1, (el,l):1, (l,o):1
    // Step 2: merge (el,l) → [h, ell, o]
    BPETokenizer tok;
    tok.Train("hello", 2);

    Rules expected_rules = {{"e", "l"}, {"el", "l"}};
    CHECK(tok.GetMergeRules() == expected_rules);

    CHECK(tok.Encode("hello") == Tokens{"h", "ell", "o"});
    CHECK(tok.Encode("bell") == Tokens{"b", "ell"});
}

TEST_CASE("All identical characters") {
    // "aaa" × 2 → freq 2
    // Pairs: (a,a) at positions 0-1 and 1-2 → 2 occurrences per word × freq 2 = 4
    BPETokenizer tok;
    tok.Train("aaa aaa", 1);

    REQUIRE(tok.GetMergeRules().size() == 1);
    CHECK(tok.GetMergeRules()[0] == std::pair<std::string, std::string>{"a", "a"});
}

TEST_CASE("Merge creates pair for next merge") {
    // "abcd" × 1
    // Step 1: all pairs have count 1. Tie → (a,b) < (b,c) < (c,d). Merge (a,b).
    // [ab, c, d]
    // Step 2: (ab,c):1, (c,d):1. Tie → (ab,c) < (c,d). Merge (ab,c).
    // [abc, d]
    // Step 3: (abc,d):1. Merge.
    // [abcd]
    BPETokenizer tok;
    tok.Train("abcd", 3);

    Rules expected_rules = {{"a", "b"}, {"ab", "c"}, {"abc", "d"}};
    CHECK(tok.GetMergeRules() == expected_rules);

    CHECK(tok.Encode("abcd") == Tokens{"abcd"});
    CHECK(tok.Encode("abcabc") == Tokens{"abc", "abc"});
    CHECK(tok.Encode("dabcd") == Tokens{"d", "abcd"});
}

TEST_CASE("Multiple words with shared substrings") {
    // "abx" × 3, "aby" × 3
    // Pairs: (a,b): 3+3=6, (b,x): 3, (b,y): 3
    // Step 1: merge (a,b) → [ab, x] and [ab, y]
    // Step 2: (ab,x):3, (ab,y):3 → tie → (ab,x) < (ab,y) → merge (ab,x)
    BPETokenizer tok;
    tok.Train(MakeCorpus({{"abx", 3}, {"aby", 3}}), 2);

    Rules expected_rules = {{"a", "b"}, {"ab", "x"}};
    CHECK(tok.GetMergeRules() == expected_rules);

    CHECK(tok.Encode("abx") == Tokens{"abx"});
    CHECK(tok.Encode("aby") == Tokens{"ab", "y"});
}

TEST_CASE("Frequency changes merge priority") {
    // Without frequency: "xy" and "ab" would tie → (a,b) wins
    // With frequency: "xy" × 10, "ab" × 1 → (x,y):10 >> (a,b):1
    BPETokenizer tok;
    tok.Train(MakeCorpus({{"xy", 10}, {"ab", 1}}), 1);

    REQUIRE(tok.GetMergeRules().size() == 1);
    CHECK(tok.GetMergeRules()[0] == std::pair<std::string, std::string>{"x", "y"});
}

TEST_CASE("Encode single character word") {
    BPETokenizer tok;
    tok.Train("ab ab", 1);

    CHECK(tok.Encode("a") == Tokens{"a"});
    CHECK(tok.Encode("x") == Tokens{"x"});
}

TEST_CASE("Whitespace handling in corpus") {
    BPETokenizer tok1;
    tok1.Train("ab ab", 1);

    BPETokenizer tok2;
    tok2.Train("  ab   ab  ", 1);

    // Both should produce the same result
    CHECK(tok1.GetMergeRules() == tok2.GetMergeRules());
    CHECK(tok1.Encode("ab") == tok2.Encode("ab"));
}

TEST_CASE("BPE discovers English morphemes") {
    // A corpus of English verb forms. BPE should discover:
    // - "ing" suffix (from playing, working, singing)
    // - "ed" suffix (from played, worked)
    // - stems "play", "work", "sing"
    std::string corpus = MakeCorpus({
        {"play", 10},
        {"plays", 5},
        {"playing", 5},
        {"played", 5},
        {"work", 10},
        {"works", 5},
        {"working", 5},
        {"worked", 5},
        {"sing", 10},
        {"sings", 5},
        {"singing", 5},
    });

    BPETokenizer tok;
    tok.Train(corpus, 10);

    auto rules = tok.GetMergeRules();
    REQUIRE(rules.size() == 10);

    // Steps 1-2: "i"+"n" → "in", then "in"+"g" → "ing"
    // These are the most frequent pairs (count 35 each, from all *ing words + sing*)
    CHECK(rules[0] == std::pair<std::string, std::string>{"i", "n"});
    CHECK(rules[1] == std::pair<std::string, std::string>{"in", "g"});

    // Step 10: "e"+"d" → "ed"
    CHECK(rules[9] == std::pair<std::string, std::string>{"e", "d"});

    // After 10 merges, stems and suffixes are separate tokens:
    CHECK(tok.Encode("playing") == Tokens{"play", "ing"});
    CHECK(tok.Encode("worked") == Tokens{"work", "ed"});
    CHECK(tok.Encode("singing") == Tokens{"sing", "ing"});
    CHECK(tok.Encode("plays") == Tokens{"play", "s"});
    CHECK(tok.Encode("sings") == Tokens{"sing", "s"});
    CHECK(tok.Encode("play") == Tokens{"play"});
    CHECK(tok.Encode("work") == Tokens{"work"});
    CHECK(tok.Encode("sing") == Tokens{"sing"});
}

TEST_CASE("BPE generalizes to unseen words") {
    // Train on verb forms (same corpus as above)
    std::string corpus = MakeCorpus({
        {"play", 10},
        {"plays", 5},
        {"playing", 5},
        {"played", 5},
        {"work", 10},
        {"works", 5},
        {"working", 5},
        {"worked", 5},
        {"sing", 10},
        {"sings", 5},
        {"singing", 5},
    });

    BPETokenizer tok;
    tok.Train(corpus, 10);

    // Words NEVER seen in training — but BPE still finds the right subwords:
    CHECK(tok.Encode("replaying") == Tokens{"r", "e", "play", "ing"});
    CHECK(tok.Encode("workings") == Tokens{"work", "ing", "s"});
    CHECK(tok.Encode("played") == Tokens{"play", "ed"});
    CHECK(tok.Encode("unworked") == Tokens{"u", "n", "work", "ed"});
}

TEST_CASE("Stress test") {
    std::string corpus;
    for (int i = 0; i < 500; ++i) {
        corpus += "hello ";
    }
    for (int i = 0; i < 300; ++i) {
        corpus += "world ";
    }
    for (int i = 0; i < 200; ++i) {
        corpus += "helloworld ";
    }
    for (int i = 0; i < 100; ++i) {
        corpus += "programming ";
    }
    for (int i = 0; i < 50; ++i) {
        corpus += "tokenizer ";
    }

    BPETokenizer tok;
    tok.Train(corpus, 50);

    CHECK(tok.GetMergeRules().size() > 0);
    CHECK(tok.GetMergeRules().size() <= 50);

    // Tokens must reconstruct the original word
    auto tokens = tok.Encode("helloworld");
    std::string reconstructed;
    for (const auto& t : tokens) {
        reconstructed += t;
    }
    CHECK(reconstructed == "helloworld");

    auto unknown = tok.Encode("unknown");
    std::string reconstructed_unknown;
    for (const auto& t : unknown) {
        reconstructed_unknown += t;
    }
    CHECK(reconstructed_unknown == "unknown");
}
