#pragma once

#define ITERATOR NumC::Core::Iterator

#include <NumC/Core/Iterator/Iterator.hpp>

namespace NumC
{
    namespace Core
    {
        /**
         * @brief Iterator for any view.
         *
         * @tparam T View array element data type.
         */
        template <typename T>
        class ViewIterator : public Iterator<T>
        {
            public:
                /// Aliases
                using dtype = T;
                using dtype_ptr = dtype*;
                using dtype_ref = dtype&;

                /// @brief Default View Iterator constructor.
                ViewIterator() {}

                /**
                 * @brief Construct a new View Iterator object
                 *
                 * @param ptr Pointer to the array data.
                 * @param strides Strides of the View being iterated.
                 * @param indices Indices of the data array that the View views.
                 * @param arr_strides Strides of the actual data stored in nd
                 * array
                 */
                ViewIterator(
                    dtype_ptr ptr,
                    const stride_t& strides = stride_t(),
                    const indices_t_v& indices = indices_t_v(),
                    const stride_t& arr_strides = stride_t())
                {
                    if (ptr == nullptr)
                    {
                        std::cout << "ERROR - viter - 1" << std::endl;
                        // throw error or add asserts.
                    }

                    this->_index = 0;
                    this->_ptr = ptr;
                    this->_nunits = 1;

                    if (strides.empty() || indices.empty() ||
                        arr_strides.empty())
                        return;

                    for (size_t i = 0; i < indices.size(); ++i)
                    {
                        this->_nunits *= (indices[i].second - indices[i].first);
                        this->__indices.push_back(indices[i]);
                        this->__strides.push_back(strides[i]);
                        this->__arr_strides.push_back(arr_strides[i]);
                    }

                    this->__arr_index = this->__get_memory_index();
                }

                /// @brief Default copy constructor.
                ViewIterator(const ViewIterator& other) = default;

                /// @brief Destroy the View Iterator object.
                ~ViewIterator() = default;

                /**
                 * @brief Prefixed increment operator.
                 */
                void operator++()
                {
                    // Not equating as that would be the end() scenario.
                    if (this->_index + 1 > this->_nunits)
                    {
                        std::cout << "ERROR - viter - 2" << std::endl;
                        // throw error or add asserts.
                    }

                    ++(this->_index);

                    auto next_arr_index = this->__get_memory_index();

                    this->_ptr += (next_arr_index - this->__arr_index);

                    this->__arr_index = next_arr_index;
                }

                /**
                 * @brief Prefixed decrement operator.
                 */
                void operator--()
                {
                    if (this->_index - 1 < 0)
                    {
                        std::cout << "ERROR - viter - 3" << std::endl;
                        // throw error.
                    }

                    --(this->_index);

                    auto prev_arr_index = this->__get_memory_index();

                    this->_ptr -= (this->__arr_index - prev_arr_index);

                    this->__arr_index = prev_arr_index;
                }

            private:

                /// @brief Current array index being pointed to.
                size_t __arr_index;

                /// @brief Strides of the view being iterated.
                stride_t __strides;

                /// @brief Indices of the data array being iterated.
                indices_t_v __indices;

                /// @brief Strides of the nd array object.
                stride_t __arr_strides;

                /**
                 * @brief Gets the corresponding memory block/ data array index
                 * for this index.
                 */
                const size_t __get_memory_index()
                {
                    if (this->_index < 0 || this->_index > this->_nunits)
                    {
                        std::cout << "ERROR - viter - 4" << std::endl;
                        // throw error.
                    }

                    size_t ndims = this->__indices.size();
                    size_t pos = 0, c_index = this->_index;

                    for (size_t i = 0; i < ndims; ++i)
                    {
                        size_t dim_count =
                            this->__indices[i].first +
                            c_index / this->__strides[i];

                        pos += (__arr_strides[i] * dim_count);
                        c_index %= this->__strides[i];
                    }

                    return pos;
                }
        };
    }
}