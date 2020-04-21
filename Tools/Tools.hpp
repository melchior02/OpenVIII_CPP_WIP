#ifndef _TOOLS_H
#define _TOOLS_H

#include <string>
#include <algorithm>
#include <cctype>
#include <type_traits>

namespace OpenVIII::Tools {
/*
Case Insensitive strings equal
*/
    [[nodiscard]] constexpr inline static bool
    iEquals(const std::string_view &str1, const std::string_view &str2) {
        const constexpr auto iEqual = [](const auto &c1, const auto &c2) {
            return ::toupper(c1) == ::toupper(c2);
        };
        if (str1.length() != str2.length()) {
            return false;
        }
        for (size_t i = 0; i < str1.length(); i++) {
            if (!iEqual(str1.at(i), str2.at(i))) {
                return false;
            }
        }
        return true;
        //return std::equal(str1.begin(), str1.end(), str2.begin(), iEqual);
        //todo std::equal constexpr in cpp20
    }


    //Replace all of one char to another char. Arguments will be static_cast to the type of string char used in haystack.
    //I use this to make the separator on windows or linux matches the strings.
    template<typename needleType, typename replacementType>
    [[maybe_unused]] constexpr inline static void
    replaceAll(std::string &haystack, const needleType &needle, const replacementType &replacement) noexcept {

        static_assert(std::is_integral<needleType>::value && std::is_integral<replacementType>::value,
                      "Should be dealing with chars so is integral should cover that.");
        if (haystack.empty()) {
            return;
        }

        if (static_cast<int>(needle) ==
            static_cast<int>(replacement)) { //casting because worried if the types don't match.
            return;
        }
        const auto replace = [needle, replacement](const auto &input) {
            //handle when T2 doesn't match T by casting to match input;
            return input == static_cast<decltype(input)>(needle)
                   ? static_cast<decltype(input)>(replacement)
                   : input;
            //windows uses a wchar_t instead of char. So I need to static cast to make them match
            //though might need at least one value to be char. I'm unsure what would happen if we were in
            //a unicode mode.
        };
        std::transform(haystack.begin(), haystack.end(), haystack.begin(), replace);
    }
/*
Find Case Insensitive Sub String in a given substring (version returns location in string and allows offset)
*/
//[[nodiscard]] inline static size_t iFind(std::string haystack, std::string needle, size_t offset = 0)
//{
//	// Convert complete given String to lower case
//	std::transform(haystack.begin(), haystack.end(), haystack.begin(), ::toupper);
//	// Convert complete given Sub String to lower case
//	std::transform(needle.begin(), needle.end(), needle.begin(), ::toupper);
//	// Find sub string in given string
//	return haystack.find(needle, offset);
//}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-qualified-auto"

    //Find Case Insensitive Sub String in a given substring
    [[maybe_unused]] [[nodiscard]] inline static auto
    iFind(const std::string_view &haystack, const std::string_view &needle) {
        const constexpr auto comp = [](const auto &ch1, const auto &ch2) { return ::toupper(ch1) == ::toupper(ch2); };
        const auto it = std::search( //clang-tidy wants to make this a pointer. msvc doesn't return a pointer.
                haystack.begin(), haystack.end(),
                needle.begin(), needle.end(),
                comp
        );
        return (it != haystack.end());
        //todo make constexpr in cpp 20
    }

#pragma clang diagnostic pop
}
#endif // !_TOOLS_H
