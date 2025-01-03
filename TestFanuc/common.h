#pragma once

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <string>

// Helper lambda to check if a character is not a space
static const auto is_not_space = [](int c) { return !std::isspace(c); };

// trim from start (in place)
static inline void ltrim(std::string& s) {
    if (!s.empty()) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), is_not_space));
    }
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    if (!s.empty()) {
        s.erase(std::find_if(s.rbegin(), s.rend(), is_not_space).base(), s.end());
    }
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

// Helper function to trim a string (copying)
static inline std::string trim_copy_helper(std::string s, void(*trim_func)(std::string&)) {
    trim_func(s);
    return s;
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    return trim_copy_helper(s, ltrim);
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    return trim_copy_helper(s, rtrim);
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    return trim_copy_helper(s, trim);
}