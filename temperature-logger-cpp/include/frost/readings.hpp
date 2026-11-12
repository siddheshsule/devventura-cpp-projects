#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace frost {

struct Reading {
  std::string sensor;
  double celsius = 0.0;
};

// Parses one export line like "freezer-2,-18.5". Spaces around both parts are ignored.
// Returns std::nullopt for malformed lines (see FRO-11).
std::optional<Reading> parse_reading(std::string_view line);

struct Summary {
  std::string sensor;
  std::size_t count = 0;
  double min = 0.0;
  double max = 0.0;
  double average = 0.0;
  std::size_t out_of_range = 0;
};

// One summary per sensor, sorted by sensor ID. Readings below `low` or above `high` are out of
// range; the limits themselves are in range (see FRO-12).
std::vector<Summary> summarize(const std::vector<Reading>& readings, double low, double high);

}  // namespace frost
