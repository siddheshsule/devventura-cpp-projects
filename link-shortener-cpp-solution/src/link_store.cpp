// Reference solution. Learners can download it after signing the project off; CI builds it
// against the starter's tests.
#include "hop/link_store.hpp"

#include <stdexcept>

#include "hop/short_code.hpp"

namespace hop {

namespace {
bool is_web_url(const std::string& url) {
  return url.starts_with("http://") || url.starts_with("https://");
}
}  // namespace

std::string LinkStore::shorten(const std::string& url) {
  if (!is_web_url(url)) throw std::invalid_argument("only http(s) URLs can be shortened");
  if (auto it = ids_by_url_.find(url); it != ids_by_url_.end()) return encode_base62(it->second);
  urls_.push_back(url);
  const auto id = static_cast<std::uint64_t>(urls_.size());
  ids_by_url_.emplace(url, id);
  return encode_base62(id);
}

std::optional<std::string> LinkStore::resolve(std::string_view code) const {
  const auto id = decode_base62(code);
  if (!id || *id == 0 || *id > urls_.size()) return std::nullopt;
  return urls_[*id - 1];
}

std::size_t LinkStore::size() const { return urls_.size(); }

}  // namespace hop
