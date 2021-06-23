#pragma once

#include <NumC/Core/View/ReshapedView.hpp>
#include <NumC/Core/View/SlicedView.hpp>
#include <NumC/Core/View/TransposedView.hpp>
#include <NumC/Utils/StringUtils.hpp>

namespace NumC
{
    namespace Utils
    {
        /**
         * @brief Helper to stringify array content.
         *
         * @tparam ArrayType Array object data type.
         * @tparam IteratorType Iterator type used.
         * @param output String that contains the output view.
         * @param array Reference to the array.
         * @param iter Reference to the array iterator.
         * @param axis Dimension to iterate over.
         */
        template<typename ArrayType, typename IteratorType>
        static void stringify_array(
            std::string& output,
            ArrayType& array,
            IteratorType& iter,
            int axis)
        {
            std::vector<char> unwanted_chars {'\n', '\t', ' ', ','};

            if (array.shape().size() <= axis)
            {
                output += (std::to_string(*iter) + ", ");
                ++iter;

                return;
            }

            int i = output.length() - 1;
            while (i >=0 &&
                (output[i] == ',' || output[i] == ' ' ||
                output[i] == '\n' || output[i] == '\t'))
                --i;

            if (i > 0 && output[i] == ']' &&
                output[i - 1] == ']')
            {
                output += "\n\t";
                output += std::string(axis, ' ');
            }

            output += '[';

            for (int i = 0; i < array.shape()[axis]; ++i)
                stringify_array(output, array, iter, axis + 1);

            rtrim(output, unwanted_chars);

            output += "],";

            if (output[output.length() - 3] != ']')
                output += "\n\t";

            output += std::string(axis, ' ');
        }

        /**
         * @brief Prints the array.
         *
         * @tparam ArrayType Array object data type.
         */
        template<typename ArrayType>
        static void print_array(ArrayType& array)
        {
            auto iter = array.begin();
            int axis = 0;

            std::vector<char> unwanted_chars {'\n', '\t', ' ', ','};

            std::string output = "array(\n";

            // printing shape.
            output += "\tshape(";
            output += to_string<size_t>(array.shape());
            output += "),\n\n";

            // printing indices.
            output += "\tindices(";
            output += to_string<size_t>(array.indices());
            output += "),\n\n";

            // printing shape.
            output += "\tstrides(";
            output += to_string<size_t>(array.strides());
            output += "),\n\n";

            output += "\t[";

            for (int i = 0; i < array.shape()[axis]; ++i)
                stringify_array(output, array, iter, axis + 1);

            rtrim(output, unwanted_chars);
            output += "]\n)\n";

            std::cout << output;
        }

        /**
         * @brief Slices the required indices off of an array.
         *
         * @tparam T Array element data type.
         * @param array Reference to the array object.
         * @param slices Indices to slice the array object.
         *
         * @return Sliced view of the array/view.
         */
        template<typename T>
        SLICED_VIEW<T> slice(ND_ARRAY<T>& array, slices_t& slices)
        {
            return SLICED_VIEW<T>(&array, slices);
        }

        /**
         * @brief Reshapes the array.
         *
         * @tparam T Array element data type.
         * @param array Reference to the array object.
         * @param newShape New shape to reshape the array to.
         *
         * @return Reshaped view of the array/view.
         */
        template<typename T>
        RESHAPED_VIEW<T> reshape(ND_ARRAY<T>& array, shape_t& newShape)
        {
            return RESHAPED_VIEW<T>(&array, newShape);
        }

        /**
         * @brief Transposes the array.
         *
         * @tparam T Array element data type.
         * @param array Reference to the array object.
         * @param axes The axes order to transpose to. Defaults to an empty size
         * list indicating reversal of axes.
         *
         * @return Transposed view of the array/view.
         */
        template<typename T>
        TRANSPOSED_VIEW<T>
        transpose(ND_ARRAY<T>& array, size_t_v& axes = size_t_v())
        {
            return TRANSPOSED_VIEW<T>(&array, axes);
        }
    }
}