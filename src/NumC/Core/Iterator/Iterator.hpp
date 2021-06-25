#pragma once

#define ITERATOR NumC::Core::Iterator

#include <NumC/Core/Iterator/MemoryIndexer.hpp>

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
                 * @param start_ptr Pointer to the array data start.
                 * @param index Initial index.
                 * @param nunits Total number of elements in the array.
                 * Defaults to 0.
                 * @param memory_indexer Pointer to array/view memory indexer.
                 * Defaults to null pointer.
                 */
                Iterator(
                    dtype_ptr start_ptr,
                    size_t index = 0,
                    size_t nunits = 0,
                    MemoryIndexer* memory_indexer = nullptr) :
                    _index(index),
                    _start_ptr(start_ptr),
                    _nunits(nunits),
                    __memory_indexer(memory_indexer)
                {
                    if (start_ptr == nullptr)
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

                    if (memory_indexer != nullptr)
                        this->__arr_index =
                            this->__memory_indexer->operator()(this->_index);
                    else
                        this->__arr_index = this->_index;

                    this->_ptr = start_ptr + this->__arr_index;
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
                 *
                 * @note In case of null indexer, the data pointer is simply
                 * incremented by 1.
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

                    if (this->__memory_indexer == nullptr)
                    {
                        this->__arr_index = this->_index;
                        ++(this->_ptr);

                        return;
                    }

                    auto next_arr_index =
                        this->__memory_indexer->operator()(this->_index);
                    this->_ptr += (next_arr_index - this->__arr_index);
                    this->__arr_index = next_arr_index;
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
                 *
                 * @note In case of null indexer, the data pointer is simply
                 * decremented by 1.
                 */
                void operator--()
                {
                    if (this->_index - 1 < 0)
                    {
                        std::cout << "ERROR - iter - 4" << std::endl;
                        // throw error.
                    }

                    --(this->_index);

                    if (this->__memory_indexer == nullptr)
                    {
                        this->__arr_index = this->_index;
                        --(this->_ptr);

                        return;
                    }

                    auto prev_arr_index =
                        this->__memory_indexer->operator()(this->_index);
                    this->_ptr -= (this->__arr_index - prev_arr_index);
                    this->__arr_index = prev_arr_index;
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
                    return this->_start_ptr == other._start_ptr &&
                        this->_index == other._index;
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

                /// @brief Pointer to the array data start.
                const dtype_ptr _start_ptr;

                /// @brief Total number of elements in the array.
                size_t _nunits;

                /// @brief Current array/view index being pointed to.
                size_t _index;

            private:

                /// @brief Current array index being pointed to.
                size_t __arr_index;

                /// @brief Pointer to the array/view memory indexer.
                MemoryIndexer* __memory_indexer;
        };
    }
}