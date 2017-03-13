#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <string>

namespace utils {
namespace strings {

enum class Fit { Left, Middle, Right };

std::string fitIn( const std::string& sentence, size_t max, Fit fit );

} // namespace strings
} // namespace utils

#endif // STRINGS_HPP
