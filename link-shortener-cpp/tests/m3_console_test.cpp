// Milestone 3 tests. Run with: ctest --test-dir build -L m3 --output-on-failure
#include <sstream>
#include <string>
#include <vector>

#include "doctest.h"
#include "hop/console.hpp"
#include "hop/link_store.hpp"

namespace {

struct Session {
  std::vector<std::string> lines;
  int errors;
};

// Feeds `input` to the console and returns every output line.
Session run(const std::string& input, hop::LinkStore& store) {
  std::istringstream in(input);
  std::ostringstream out;
  const int errors = hop::run_console(in, out, store);
  Session session{{}, errors};
  std::istringstream lines(out.str());
  for (std::string line; std::getline(lines, line);) session.lines.push_back(line);
  return session;
}

Session run(const std::string& input) {
  hop::LinkStore store;
  return run(input, store);
}

}  // namespace

TEST_CASE("shortens and resolves through the console") {
  hop::LinkStore store;
  const auto first = run("shorten https://example.com/sale\n", store);
  REQUIRE(first.lines.size() == 1);
  const auto code = first.lines[0];
  CHECK(store.resolve(code) == "https://example.com/sale");

  const auto second = run("resolve " + code + "\n", store);
  CHECK(second.lines == std::vector<std::string>{"https://example.com/sale"});
  CHECK(second.errors == 0);
}

TEST_CASE("prints one line per command, in order") {
  const auto session = run(
      "shorten https://example.com/a\n"
      "shorten https://example.com/b\n"
      "shorten https://example.com/a\n"
      "count\n");
  REQUIRE(session.lines.size() == 4);
  CHECK(session.lines[0] == session.lines[2]);
  CHECK(session.lines[0] != session.lines[1]);
  CHECK(session.lines[3] == "2");
}

TEST_CASE("reports unknown codes without failing") {
  const auto session = run("resolve zzz\n");
  CHECK(session.lines == std::vector<std::string>{"not found"});
  CHECK(session.errors == 0);
}

TEST_CASE("reports invalid input as errors and keeps going") {
  const auto session = run(
      "shorten ftp://example.com\n"
      "shorten\n"
      "launch rockets\n"
      "count\n");
  REQUIRE(session.lines.size() == 4);
  CHECK(session.lines[0].rfind("error: ", 0) == 0);
  CHECK(session.lines[1].rfind("error: ", 0) == 0);
  CHECK(session.lines[2] == "error: unknown command 'launch'");
  CHECK(session.lines[3] == "0");
  CHECK(session.errors == 3);
}

TEST_CASE("ignores empty lines and surrounding spaces") {
  const auto session = run("\n   \n  count  \n\n");
  CHECK(session.lines == std::vector<std::string>{"0"});
}

TEST_CASE("stops at quit") {
  const auto session = run("count\nquit\ncount\n");
  CHECK(session.lines == std::vector<std::string>{"0"});
}

TEST_CASE("handles input without a trailing newline") {
  const auto session = run("count");
  CHECK(session.lines == std::vector<std::string>{"0"});
}
