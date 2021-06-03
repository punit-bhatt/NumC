#pragma once

#define ITERATOR NumC::Core::Iterator

#include <NumC/Core/Type.hpp>

#include <numeric>

namespace NumC
{
    namespace Core
    {
        /**
         * @brief Iterator for any array.
         *
         * @tparam T Array element data type.
         */
        template <typename T>
        class Iterator
        {
            public:
                /// Aliases
                using dtype = T;
                using dtype_ptr = dtype*;
                using dtype_ref = dtype&;

                /// @brief Default Iterator constructor.
                Iterator() {}

                /**
                 * @brief Construct a new Iterator object
                 *
                 * @param ptr Pointer to the array data.
                 * @param nunits Total number of elements in the array.
                 */
                Iterator(dtype_ptr ptr, size_t nunits)
                {
                    if (ptr == nullptr)
                    {
                        std::cout << "ERROR - iter - 1" << std::endl;
                        // throw error or add asserts.
                    }

                    // 0 allowed for end() iterators.
                    if (nunits < 0)
                    {
                        std::cout << "ERROR - iter - 2" << std::endl;
                        // throw error or add asserts.
                    }

                    this->_index = 0;
                    this->_ptr = ptr;
                    this->_nunits = nunits;
                }

                /// @brief Default copy constructor.
                Iterator(const Iterator& other) = default;

                /// @brief Destroy the Iterator object.
                ~Iterator() = default;

                /**
                 * @brief Dereference current iteration element.
                 *
                 * @return Reference to the current iteration element.
                 */
                dtype& operator*() const
                {
                    return *(this->_ptr);
                }

                /**
                 * @brief Prefixed increment operator.
                 */
                void operator++()
                {
                    // Not equating as that would be the end() scenario.
                    if (this->_index + 1 > this->_nunits)
                    {
                        std::cout << "ERROR - iter - 3" << std::endl;
                        // throw error or add asserts.
                    }

                    ++(this->_index);
                    ++(this->_ptr);
                }

                /**
                 * @brief Suffixed increment operator.
                 *
                 * @return Iterator prior to increment.
                 */
                Iterator<dtype>& operator++(int)
                {
                    auto tmp = *this
                    ++*this;

                    return tmp;
                }

                /**
                 * @brief Prefixed decrement operator.
                 */
                void operator--()
                {
                    if (this->_index - 1 < 0)
                    {
                        std::cout << "ERROR - iter - 4" << std::endl;
                        // throw error.
                    }

                    --(this->_index);
                    --(this->_ptr);
                }

                /**
                 * @brief Suffixed decrement operator.
                 *
                 * @return Iterator prior to decrement.
                 */
                Iterator<dtype>& operator--(int)
                {
                    auto tmp = *this
                    --*this;

                    return tmp;
                }

                /**
                 * @brief Equality operator.
                 *
                 * @param other Iterator being compared.
                 * @return Value indicating if the iterators are equal.
                 */
                bool operator==(Iterator<dtype> const& other) const
                {
                    return this->_ptr == other._ptr;
                }

                /**
                 * @brief Inequality operator.
                 *
                 * @param other Iterator being compared.
                 * @return Value indicating if the iterators are inequal.
                 */
                bool operator!=(Iterator<dtype> const& other) const
                {
                    return !((*this) == other);
                }

                /// @brief Default assignment operator.
                Iterator<dtype>&
                operator=(const Iterator<dtype>& other) = default;

            protected:

                /// @brief Pointer to the array data.
                dtype_ptr _ptr;

                /// @brief Total number of elements in the array.
                size_t _nunits;

                /// @brief Current array/view index being pointed to.
                size_t _index;
        };
    }
}