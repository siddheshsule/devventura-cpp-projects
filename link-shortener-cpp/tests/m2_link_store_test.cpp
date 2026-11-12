// Milestone 2 tests. Run with: ctest --test-dir build -L m2 --output-on-failure
#include <set>
#include <stdexcept>
#include <string>

#include "doctest.h"
#include "hop/link_store.hpp"

using hop::LinkStore;

namespace {
bool is_short_code(const std::string& code) {
  if (code.empty() || code.size() > 11) return false;
  for (char c : code) {
    const bool ok = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    if (!ok) return false;
  }
  return true;
}
}  // namespace

TEST_CASE("a new store is empty") {
  const LinkStore store;
  CHECK(store.size() == 0);
  CHECK_FALSE(store.resolve("abc").has_value());
}

TEST_CASE("shortened links resolve back to the original URL") {
  LinkStore store;
  const auto code = store.shorten("https://example.com/spring-sale?utm_source=newsletter");
  CHECK(is_short_code(code));
  CHECK(store.resolve(code) == "https://example.com/spring-sale?utm_source=newsletter");
}

TEST_CASE("the same URL always gets the same code") {
  LinkStore store;
  const auto first = store.shorten("https://example.com/a");
  const auto second = store.shorten("https://example.com/a");
  CHECK(first == second);
  CHECK(store.size() == 1);
}

TEST_CASE("different URLs get different codes") {
  LinkStore store;
  std::set<std::string> codes;
  for (int i = 0; i < 1000; ++i) {
    codes.insert(store.shorten("https://example.com/page/" + std::to_string(i)));
  }
  CHECK(codes.size() == 1000);
  CHECK(store.size() == 1000);
}

TEST_CASE("only http and https URLs are accepted") {
  LinkStore store;
  CHECK_THROWS_AS(store.shorten(""), std::invalid_argument);
  CHECK_THROWS_AS(store.shorten("example.com"), std::invalid_argument);
  CHECK_THROWS_AS(store.shorten("ftp://example.com/file"), std::invalid_argument);
  CHECK_THROWS_AS(store.shorten("javascript:alert(1)"), std::invalid_argument);
  CHECK(store.size() == 0);
  CHECK_NOTHROW(store.shorten("http://example.com"));
}

TEST_CASE("unknown and malformed codes resolve to nothing") {
  LinkStore store;
  store.shorten("https://example.com");
  CHECK_FALSE(store.resolve("").has_value());
  CHECK_FALSE(store.resolve("not-a-code!").has_value());
  CHECK_FALSE(store.resolve("ZZZZZZ").has_value());
}
