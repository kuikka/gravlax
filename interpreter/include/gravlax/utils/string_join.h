#pragma once

#include <iterator>
#include <numeric>
#include <string>

// template <typename T>
std::string string_join(const std::vector<std::string> &strings,
                        const std::string &delimeter)
{
    return std::accumulate(std::next(strings.begin()), strings.end(),
                           strings[0],
                           [&delimeter](std::string a, std::string b) {
                               std::string s;
                               s += a;
                               s += delimeter;
                               s += b;
                               return s;
                           });
}