// Reference solution. Learners can download it after signing the project off; CI builds it
// against the starter's tests.
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace hop {

class LinkStore {
 public:
  std::string shorten(const std::string& url);
  std::optional<std::string> resolve(std::string_view code) const;
  std::size_t size() const;

 private:
  // Code n (decoded) is the URL at index n - 1, so codes start at "1" and lookups by code are a
  // decode plus a vector index. The map makes "same URL, same code" O(1).
  std::vector<std::string> urls_;
  std::unordered_map<std::string, std::uint64_t> ids_by_url_;
};

}  // namespace hop
