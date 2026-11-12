#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace hop {

// Keeps track of which short code points to which long URL.
class LinkStore {
 public:
  // Returns the short code for `url`, creating one if this URL hasn't been shortened before.
  // Shortening the same URL twice returns the same code.
  // Throws std::invalid_argument if `url` doesn't start with "http://" or "https://".
  std::string shorten(const std::string& url);

  // Returns the URL behind `code`, or std::nullopt if the code is unknown.
  std::optional<std::string> resolve(std::string_view code) const;

  // Number of distinct URLs stored.
  std::size_t size() const;

 private:
  // urls_[id] is the URL whose short code is encode_base62(id).
  std::vector<std::string> urls_;
  // Reverse lookup so shortening the same URL twice reuses its id.
  std::unordered_map<std::string, std::uint64_t> ids_by_url_;
};

}  // namespace hop
