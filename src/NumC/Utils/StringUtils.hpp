#pragma once

#include <algorithm>

namespace NumC
{
    namespace Utils
    {
        /**
         * @brief Utility function to stringify array.
         *
         * @tparam T Basic data type
         * @param ptr Read-only pointer to the array.
         * @param len Length of the array.
         * @return Output string.
         */
        template<typename T>
        static std::string to_string(const T* ptr, size_t len)
        {
            // Verify basic data type.

            std::string output;

            for (int i = 0; i < len; ++i)
            {
                output += std::to_string(ptr[i]);
                output += ", ";
            }

            int slen = output.length();
            int i = slen - 1;

            while (i >= 0 && (output[i] == ',' || output[i] == ' '))
                --i;

            output.erase(i + 1, slen - i);

            return output;
        }

        /**
         * @brief Utility function to stringify array of pairs.
         *
         * @tparam T Basic data type.
         * @param ptr Read-only pointer to the array.
         * @param len Length of the array.
         * @return Output string.
         */
        template<typename T>
        static std::string
        to_string(const std::pair<T, T>* ptr, size_t len)
        {
            // Verify basic data type.

            std::string output;

            for (int i = 0; i < len; ++i)
            {
                output += '[';
                output += std::to_string(ptr[i].first);
                output += ", ";
                output += std::to_string(ptr[i].second);
                output += "), ";
            }

            int slen = output.length();
            int i = slen - 1;

            while (i >= 0 && (output[i] == ',' || output[i] == ' '))
                --i;

            output.erase(i + 1, slen - i);

            return output;
        }

        /**
         * @brief Utility function to stringify vector of pairs.
         *
         * @tparam T Basic data type.
         * @param vctr Reference to the vector of pairs.
         * @return Output string.
         */
        template<typename T>
        static std::string
        to_string(const std::vector<std::pair<T, T>>& vctr)
        {
            // Verify basic data type.

            std::string output;

            for (auto element: vctr)
            {
                output += '[';
                output += std::to_string(element.first);
                output += ", ";
                output += std::to_string(element.second);
                output += "), ";
            }

            int slen = output.length();
            int i = slen - 1;

            while (i >= 0 && (output[i] == ',' || output[i] == ' '))
                --i;

            output.erase(i + 1, slen - i);

            return output;
        }

        /**
         * @brief Utility function to stringify vector.
         *
         * @tparam T Basic data type.
         * @param vctr Vector to be printed.
         * @return Output string.
         */
        template<typename T>
        static std::string to_string(const std::vector<T>& vctr)
        {
            // Verify basic data type.

            std::string output;

            for (auto element: vctr)
            {
                output += std::to_string(element);
                output += ", ";
            }

            int len = output.length();
            int i = len - 1;

            while (i >= 0 && (output[i] == ',' || output[i] == ' '))
                --i;

            output.erase(i + 1, len - i);

            return output;
        }

        /**
         * @brief Trims the string of the listed unwanted characters
         * from the left end.
         *
         * @param s Reference to the string to be trimmed.
         * @param chars Reference to the list of characters to be
         * removed.
         */
        static void ltrim(std::string& s, std::vector<char>& chars)
        {
            int s_len = s.length();
            int c_len = chars.size();
            int i = 0;

            while (i < s_len &&
                (std::find(
                    chars.begin(),
                    chars.end(), s[i]) != chars.end()))
                ++i;

            s.erase(0, i - 1);
        }

        /**
         * @brief Trims the string of the listed unwanted characters
         * from the right end.
         *
         * @param s Reference to the string to be trimmed.
         * @param chars Reference to the list of characters to be
         * removed.
         */
        static void rtrim(std::string& s, std::vector<char>& chars)
        {
            int s_len = s.length();
            int c_len = chars.size();
            int i = s_len - 1;

            while (i >= 0 &&
                (std::find(
                    chars.begin(),
                    chars.end(), s[i]) != chars.end()))
                --i;

            s.erase(i + 1, s_len - i);
        }

        /**
         * @brief Trims the string of the listed unwanted characters
         * from either ends.
         *
         * @param s Reference to the string to be trimmed.
         * @param chars Reference to the list of characters to be
         * removed.
         */
        static void trim(std::string& s, std::vector<char>& chars)
        {
            ltrim(s, chars);
            rtrim(s, chars);
        }
    }
}