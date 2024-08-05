#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace gravlax::utils
{

std::string trim_string(const std::string &s,
                        const std::string whitespace = " \t")
{
    auto start_of_txt = s.find_first_not_of(whitespace);
    auto end_of_txt = s.find_last_not_of(whitespace);

    // Only whitespace or empty string
    if (start_of_txt == std::string::npos) {
        return "";
    }

    return std::string(s.cbegin() + start_of_txt, s.cbegin() + end_of_txt + 1);
}

std::string to_lowercase(const std::string &s)
{
    std::string ret{s};

    std::transform(ret.begin(), ret.end(), ret.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    return ret;
}

template <typename T>
std::vector<std::string> tokenize_string(const std::string &s,
                                         const T &delimeter, int max_split = -1)

{
    std::vector<std::string> ret;

    std::string::size_type pos = 0;

    while (pos != std::string::npos) {
        // Find beginning of word (ignore leading delimeters)
        auto bi = s.find_first_not_of(delimeter, pos);
        if (bi != std::string::npos) {
            // Find the end of this word
            auto ei = s.find_first_of(delimeter, bi);
            if (ei == std::string::npos) {
                // Last word
                ret.push_back(s.substr(bi));
            } else {
                ret.push_back(s.substr(bi, ei - bi));
            }
            if (max_split > 0 && ret.size() == max_split) {
                if (ei != std::string::npos) {
                    auto i = s.find_first_not_of(delimeter, ei);
                    ret.push_back(s.substr(i));
                    return ret;
                }
            }
            pos = ei;
        } else
            pos = bi; // end
    }
    return ret;
}

} // namespace gravlax::utils
