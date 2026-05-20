// TRVRNB's Terminal Formatting ver. 1
// https://TRVRNB/CppTrvLib.git
// this should work cross-platform?
#ifndef TERMINAL_FORMAT_CODES
#define TERMINAL_FORMAT_CODES

namespace terminal_format_codes{
  // the special colors are all bold
  char RED[10] = "\033[1;31m";
  char GREEN[10] = "\033[1;32m";
  char YELLOW[10] = "\033[1;33m";
  char BLUE[10] = "\033[1;34m";
  char WHITE[10] = "\033[1;37m";
  char RESET[8] = "\033[0m"; // reset to normal formatting
}
using namespace terminal_format_codes;

#endif
