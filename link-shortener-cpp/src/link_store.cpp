#include "hop/link_store.hpp"

#include <stdexcept>

#include "hop/short_code.hpp"

namespace hop {

namespace {

bool starts_with(std::string_view s, std::string_view prefix) {
  return s.substr(0, prefix.size()) == prefix;
}

bool has_web_scheme(std::string_view url) {
  return starts_with(url, "http://") || starts_with(url, "https://");
}

}  // namespace

std::string LinkStore::shorten(const std::string& url) {
  if (!has_web_scheme(url)) {
    throw std::invalid_argument("URL must start with http:// or https://");
  }

  if (const auto it = ids_by_url_.find(url); it != ids_by_url_.end()) {
    return encode_base62(it->second);
  }

  const auto id = static_cast<std::uint64_t>(urls_.size());
  urls_.push_back(url);
  ids_by_url_.emplace(url, id);
  return encode_base62(id);
}

std::optional<std::string> LinkStore::resolve(std::string_view code) const {
  const auto id = decode_base62(code);
  if (!id || *id >= urls_.size()) {
    return std::nullopt;
  }
  return urls_[static_cast<std::size_t>(*id)];
}

std::size_t LinkStore::size() const { return urls_.size(); }

}  // namespace hop
