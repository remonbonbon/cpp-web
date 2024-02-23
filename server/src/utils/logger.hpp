#ifndef _MY_LOGGER_
#define _MY_LOGGER_

#include <ctime>
#include <iostream>

#define RESET "\033[0m"
// #define BLACK "\033[30m"   /* Black */
#define RED "\033[31m"    /* Red */
#define GREEN "\033[32m"  /* Green */
#define YELLOW "\033[33m" /* Yellow */
// #define BLUE "\033[34m"    /* Blue */
// #define MAGENTA "\033[35m" /* Magenta */
// #define CYAN "\033[36m"    /* Cyan */
// #define WHITE "\033[37m"   /* White */

namespace logger {

std::string _timestamp() {
  time_t now;
  time(&now);
  char buf[sizeof "2011-10-08T07:07:09Z"];
  strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
  return std::string(buf);
}

void _log() {}

template <typename First, typename... Rest>
void _log(First first, Rest... rest) {
  std::cout << first;
  if (sizeof...(rest)) {
    std::cout << " ";
  }
  _log(rest...);
}

template <typename... Args> void info(Args... args) {
  std::cout << GREEN;
  _log(_timestamp(), "[INFO]", args...);
  std::cout << RESET << std::endl;
}

template <typename... Args> void warn(Args... args) {
  std::cout << YELLOW;
  _log(_timestamp(), "[WARN]", args...);
  std::cout << RESET << std::endl;
}

template <typename... Args> void error(Args... args) {
  std::cout << RED;
  _log(_timestamp(), "[ERROR]", args...);
  std::cout << RESET << std::endl;
}

} // namespace logger

#endif
