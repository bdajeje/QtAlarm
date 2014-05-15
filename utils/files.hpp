#ifndef FILES_HPP
#define FILES_HPP

#include <string>

namespace utils {
namespace files {

bool read( const std::string& filepath, std::string& result );
bool create( const std::string& filepath, const std::string& content );

} // namespace files
} // namespace utils

#endif // FILES_HPP
