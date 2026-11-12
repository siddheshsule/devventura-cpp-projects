// Reference solution. Learners can download it after signing the project off; CI builds it
// against the starter's tests.
#include "hop/short_code.hpp"

#include <algorithm>
#include <limits>

namespace hop {

std::string encode_base62(std::uint64_t value) {
  if (value == 0) return std::string(1, kBase62Alphabet[0]);
  std::string out;
  while (value > 0) {
    out.push_back(kBase62Alphabet[value % 62]);
    value /= 62;
  }
  std::reverse(out.begin(), out.end());
  return out;
}

std::optional<std::uint64_t> decode_base62(std::string_view code) {
  if (code.empty()) return std::nullopt;
  constexpr auto kMax = std::numeric_limits<std::uint64_t>::max();
  std::uint64_t value = 0;
  for (char c : code) {
    const auto digit = kBase62Alphabet.find(c);
    if (digit == std::string_view::npos) return std::nullopt;
    if (value > (kMax - digit) / 62) return std::nullopt;  // value * 62 + digit would overflow
    value = value * 62 + digit;
  }
  return value;
}

}  // namespace hop
