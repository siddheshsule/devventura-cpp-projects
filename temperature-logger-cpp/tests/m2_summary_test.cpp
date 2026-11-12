// Milestone 2 tests. Run with: ctest --test-dir build -L m2 --output-on-failure
#include <vector>

#include "doctest.h"
#include "frost/readings.hpp"

using frost::Reading;
using frost::summarize;

TEST_CASE("no readings, no summaries") {
  CHECK(summarize({}, -25.0, -15.0).empty());
}

TEST_CASE("summarises one sensor") {
  const std::vector<Reading> readings{{"freezer-1", -18.0}, {"freezer-1", -20.0}, {"freezer-1", -16.0}};
  const auto summaries = summarize(readings, -25.0, -15.0);
  REQUIRE(summaries.size() == 1);
  CHECK(summaries[0].sensor == "freezer-1");
  CHECK(summaries[0].count == 3);
  CHECK(summaries[0].min == doctest::Approx(-20.0));
  CHECK(summaries[0].max == doctest::Approx(-16.0));
  CHECK(summaries[0].average == doctest::Approx(-18.0));
  CHECK(summaries[0].out_of_range == 0);
}

TEST_CASE("sorts summaries by sensor ID") {
  const std::vector<Reading> readings{{"freezer-b", -18.0}, {"freezer-a", -19.0}, {"dock-1", 3.0},
                                      {"freezer-a", -21.0}};
  const auto summaries = summarize(readings, -25.0, -15.0);
  REQUIRE(summaries.size() == 3);
  CHECK(summaries[0].sensor == "dock-1");
  CHECK(summaries[1].sensor == "freezer-a");
  CHECK(summaries[1].count == 2);
  CHECK(summaries[2].sensor == "freezer-b");
}

TEST_CASE("the limits themselves are in range") {
  const std::vector<Reading> readings{{"f", -25.0}, {"f", -15.0}, {"f", -25.1}, {"f", -14.9}};
  const auto summaries = summarize(readings, -25.0, -15.0);
  REQUIRE(summaries.size() == 1);
  CHECK(summaries[0].out_of_range == 2);
}

TEST_CASE("averages are not rounded") {
  const std::vector<Reading> readings{{"room", 1.0}, {"room", 2.0}, {"room", 2.0}};
  const auto summaries = summarize(readings, -100.0, 100.0);
  REQUIRE(summaries.size() == 1);
  CHECK(summaries[0].average == doctest::Approx(5.0 / 3.0));
}

TEST_CASE("minimum and maximum work for freezers that never reach zero") {
  const std::vector<Reading> readings{{"deep", -18.0}, {"deep", -19.5}};
  const auto summaries = summarize(readings, -25.0, -15.0);
  REQUIRE(summaries.size() == 1);
  CHECK(summaries[0].max == doctest::Approx(-18.0));
  CHECK(summaries[0].min == doctest::Approx(-19.5));
}
