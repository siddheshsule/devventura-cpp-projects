// Reference solution. Learners can download it after signing the project off; CI builds it
// against the starter's tests.
#include "hop/console.hpp"

#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace hop {

namespace {

std::string_view trim(std::string_view text) {
  const auto first = text.find_first_not_of(" \t\r");
  if (first == std::string_view::npos) return {};
  const auto last = text.find_last_not_of(" \t\r");
  return text.substr(first, last - first + 1);
}

// Splits "shorten https://x" into {"shorten", "https://x"}.
std::pair<std::string_view, std::string_view> split_command(std::string_view line) {
  const auto space = line.find_first_of(" \t");
  if (space == std::string_view::npos) return {line, {}};
  return {line.substr(0, space), trim(line.substr(space + 1))};
}

}  // namespace

int run_console(std::istream& in, std::ostream& out, LinkStore& store) {
  int errors = 0;
  for (std::string raw; std::getline(in, raw);) {
    const auto line = trim(raw);
    if (line.empty()) continue;
    const auto [command, argument] = split_command(line);

    if (command == "quit") break;
    if (command == "count") {
      out << store.size() << '\n';
    } else if (command == "resolve") {
      const auto url = store.resolve(argument);
      out << (url ? *url : "not found") << '\n';
    } else if (command == "shorten") {
      if (argument.empty()) {
        out << "error: shorten needs a URL\n";
        ++errors;
        continue;
      }
      try {
        out << store.shorten(std::string(argument)) << '\n';
      } catch (const std::invalid_argument& error) {
        out << "error: " << error.what() << '\n';
        ++errors;
      }
    } else {
      out << "error: unknown command '" << command << "'\n";
      ++errors;
    }
  }
  return errors;
}

}  // namespace hop
