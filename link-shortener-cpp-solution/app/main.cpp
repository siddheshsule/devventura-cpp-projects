// The `hop` console for the support team. Run it with: ./build/hop  (build\Debug\hop.exe on Windows)
// You don't need to change this file; the logic lives in src/console.cpp.
#include <iostream>

#include "hop/console.hpp"
#include "hop/link_store.hpp"

int main() {
  hop::LinkStore store;
  const int errors = hop::run_console(std::cin, std::cout, store);
  return errors == 0 ? 0 : 1;
}
