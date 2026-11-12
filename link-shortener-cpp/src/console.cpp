#include "hop/console.hpp"

#include <istream>
#include <optional>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace hop {

namespace {

// Reads the single argument a command expects. Returns std::nullopt (and prints an error)
// if it is missing or followed by extra words.
std::optional<std::string> read_one_argument(std::istringstream& words, const std::string& command,
                                             std::ostream& out) {
  std::string arg;
  if (!(words >> arg)) {
    out << "error: '" << command << "' needs an argument\n";
    return std::nullopt;
  }
  std::string extra;
  if (words >> extra) {
    out << "error: '" << command << "' takes exactly one argument\n";
    return std::nullopt;
  }
  return arg;
}

}  // namespace

int run_console(std::istream& in, std::ostream& out, LinkStore& store) {
  int errors = 0;

  for (std::string line; std::getline(in, line);) {
    std::istringstream words(line);  // also handles a stray '\r' from CRLF input
    std::string command;
    if (!(words >> command)) {
      continue;  // empty or whitespace-only line
    }

    if (command == "quit") {
      break;
    }

    if (command == "count") {
      out << store.size() << '\n';
    } else if (command == "shorten") {
      const auto url = read_one_argument(words, command, out);
      if (!url) {
        ++errors;
        continue;
      }
      try {
        out << store.shorten(*url) << '\n';
      } catch (const std::invalid_argument& e) {
        out << "error: " << e.what() << '\n';
        ++errors;
      }
    } else if (command == "resolve") {
      const auto code = read_one_argument(words, command, out);
      if (!code) {
        ++errors;
        continue;
      }
      const auto url = store.resolve(*code);
      out << (url ? *url : "not found") << '\n';
    } else {
      out << "error: unknown command '" << command << "'\n";
      ++errors;
    }
  }

  return errors;
}

}  // namespace hop
