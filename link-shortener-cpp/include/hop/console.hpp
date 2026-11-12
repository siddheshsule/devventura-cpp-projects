#pragma once

#include <iosfwd>

#include "hop/link_store.hpp"

namespace hop {

// Runs the support console: reads one command per line from `in` and writes one line of output
// per command to `out`, until the input ends or a `quit` command.
//
//   shorten <url>   prints the short code, or "error: <reason>"
//   resolve <code>  prints the URL, or "not found"
//   count           prints the number of stored links
//   quit            stops reading (prints nothing)
//
// Empty lines are ignored. Any other command prints "error: unknown command '<command>'".
// Returns the number of commands that printed an error.
int run_console(std::istream& in, std::ostream& out, LinkStore& store);

}  // namespace hop
