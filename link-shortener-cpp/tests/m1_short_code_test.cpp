// Milestone 1 tests. Run with: ctest --test-dir build -L m1 --output-on-failure
#include <cstdint>
#include <limits>

#include "doctest.h"
#include "hop/short_code.hpp"

using hop::decode_base62;
using hop::encode_base62;

TEST_CASE("encodes small numbers") {
  CHECK(encode_base62(0) == "0");
  CHECK(encode_base62(9) == "9");
  CHECK(encode_base62(10) == "a");
  CHECK(encode_base62(35) == "z");
  CHECK(encode_base62(36) == "A");
  CHECK(encode_base62(61) == "Z");
}

TEST_CASE("carries over into a second digit") {
  CHECK(encode_base62(62) == "10");
  CHECK(encode_base62(62 * 62) == "100");
  CHECK(encode_base62(125) == "21");
}

TEST_CASE("decodes what it encodes") {
  for (std::uint64_t value : {0ULL, 1ULL, 61ULL, 62ULL, 3843ULL, 1'000'000ULL, 9'876'543'210ULL}) {
    CAPTURE(value);
    CHECK(decode_base62(encode_base62(value)) == value);
  }
}

TEST_CASE("handles the largest 64-bit number") {
  const auto max = std::numeric_limits<std::uint64_t>::max();
  CHECK(encode_base62(max) == "lYGhA16ahyf");
  CHECK(decode_base62("lYGhA16ahyf") == max);
}

TEST_CASE("rejects invalid codes") {
  CHECK_FALSE(decode_base62("").has_value());
  CHECK_FALSE(decode_base62("abc-1").has_value());
  CHECK_FALSE(decode_base62("hello world").has_value());
}

TEST_CASE("rejects codes that don't fit in 64 bits") {
  CHECK_FALSE(decode_base62("lYGhA16ahyg").has_value());
  CHECK_FALSE(decode_base62("ZZZZZZZZZZZZ").has_value());
}
