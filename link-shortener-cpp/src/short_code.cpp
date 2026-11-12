#include "hop/short_code.hpp"

#include <algorithm>
#include <limits>

namespace hop {

std::string encode_base62(std::uint64_t value) {
  if (value == 0) {
    return "0";
  }
  std::string result;
  while (value > 0) {
    result.push_back(kBase62Alphabet[value % 62]);
    value /= 62;
  }
  std::reverse(result.begin(), result.end());
  return result;
}

std::optional<std::uint64_t> decode_base62(std::string_view code) {
  if (code.empty()) {
    return std::nullopt;
  }
  constexpr auto kMax = std::numeric_limits<std::uint64_t>::max();
  std::uint64_t result = 0;
  for (char c : code) {
    const auto pos = kBase62Alphabet.find(c);
    if (pos == std::string_view::npos) {
      return std::nullopt;
    }
    const auto digit = static_cast<std::uint64_t>(pos);
    // Reject if result * 62 + digit would exceed 64 bits.
    if (result > (kMax - digit) / 62) {
      return std::nullopt;
    }
    result = result * 62 + digit;
  }
  return result;
}

}  // namespace hop
