// Milestone 1 tests. Run with: ctest --test-dir build -L m1 --output-on-failure
#include "doctest.h"
#include "frost/readings.hpp"

using frost::parse_reading;

TEST_CASE("parses a simple line") {
  const auto reading = parse_reading("freezer-2,-18.5");
  REQUIRE(reading.has_value());
  CHECK(reading->sensor == "freezer-2");
  CHECK(reading->celsius == doctest::Approx(-18.5));
}

TEST_CASE("ignores spaces around both parts") {
  const auto reading = parse_reading("  dock-1 , 4.0 ");
  REQUIRE(reading.has_value());
  CHECK(reading->sensor == "dock-1");
  CHECK(reading->celsius == doctest::Approx(4.0));
}

TEST_CASE("accepts whole numbers and zero") {
  const auto zero = parse_reading("cooler-7,0");
  REQUIRE(zero.has_value());
  CHECK(zero->celsius == doctest::Approx(0.0));
  const auto cold = parse_reading("freezer-1,-20");
  REQUIRE(cold.has_value());
  CHECK(cold->celsius == doctest::Approx(-20.0));
}

TEST_CASE("rejects malformed lines") {
  for (const char* line : {"", "ERR", "freezer-2,", ",4.0", "   ,4.0", "freezer-2,5abc",
                           "freezer-2,4.0,1", "freezer-2;4.0", "freezer-2,abc", "freezer-2,nan",
                           "freezer-2,inf", "freezer-2,   "}) {
    CAPTURE(line);
    CHECK_FALSE(parse_reading(line).has_value());
  }
}
