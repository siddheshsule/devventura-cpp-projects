#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace hop {

// The 62 characters a short code may contain, in order: digits, lowercase, uppercase.
inline constexpr std::string_view kBase62Alphabet =
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Turns a number into a short code, e.g. 0 -> "0", 61 -> "Z", 62 -> "10".
std::string encode_base62(std::uint64_t value);

// The reverse of encode_base62. Returns std::nullopt for an empty code, a character outside
// the alphabet, or a code too large to fit in 64 bits.
std::optional<std::uint64_t> decode_base62(std::string_view code);

}  // namespace hop
