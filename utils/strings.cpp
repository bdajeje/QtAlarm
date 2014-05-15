#include "strings.hpp"

namespace utils {
namespace strings {

std::string fitIn( const std::string& sentence, size_t max, Fit fit )
{
  if(sentence.empty() || max == 0)
    return {};

  const auto length = sentence.length();
  if(length <= max)
    return sentence;

  const std::string delimiter("..");

  switch(fit)
  {
    case Fit::Left:
      return sentence.substr(0, max - delimiter.length()) + delimiter;
    case Fit::Right:
      return delimiter + sentence.substr(delimiter.length(), max - delimiter.length());
    case Fit::Middle:
    {
      const auto middle = max/2 - delimiter.length()/2;
      return sentence.substr(0, middle) + delimiter +
             sentence.substr(length - middle);
    }
    default: return {};
  }
}

} // namespace strings
} // namespace utils
