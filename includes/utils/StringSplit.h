#pragma once

#include <map>
#include <string>
#include <vector>

namespace utils {

std::vector<std::string> stringSplit(const std::string &s,
                                     const std::string &seperator);
std::vector<std::string> stringSplit(const std::string &s,
                                     char seperator);

std::map<std::string, std::string>
stringsToMap(const std::vector<std::string> &data,
             const std::string &separator);

std::map<std::string, std::string>
stringsToMap(const std::vector<std::string> &data, char separator);

} // namespace utils