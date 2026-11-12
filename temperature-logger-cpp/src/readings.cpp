#include "frost/readings.hpp"

#include <stdexcept>

namespace frost {

std::optional<Reading> parse_reading(std::string_view line) {
  // TODO(milestone 1): implement. See FRO-11 and the milestone guide.
  (void)line;
  throw std::logic_error("parse_reading is not implemented yet");
}

std::vector<Summary> summarize(const std::vector<Reading>& readings, double low, double high) {
  // TODO(milestone 2): implement. See FRO-12.
  (void)readings;
  (void)low;
  (void)high;
  throw std::logic_error("summarize is not implemented yet");
}

}  // namespace frost
