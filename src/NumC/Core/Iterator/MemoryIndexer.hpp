#pragma once

#define MEMORY_INDEXER NumC::Core::MemoryIndexer

#include <NumC/Core/Type.hpp>

namespace NumC
{
    namespace Core
    {
        /**
         * @brief Base memory indexer functor class. Helps array/view implement
         * their own logic for memory access and pass it to the iterator.         *
         */
        class MemoryIndexer
        {
            public:

                /// @brief Default MemoryIndexer constructor.
                MemoryIndexer() = default;

                /**
                 * @brief Construct a new Memory Indexer object with the basic
                 * information required for indexing.
                 *
                 * @param shape Constant reference to array/view shape.
                 * @param strides Constant reference to array/view strides.
                 * @param arr_strides Constant reference to base array strides.
                 * @param indices Constant reference to array/view indices.
                 */
                MemoryIndexer(
                    const shape_t& shape,
                    const stride_t& strides,
                    const stride_t& arr_strides,
                    const indices_t_v& indices) :
                    _shape(&shape),
                    _strides(&strides),
                    _arr_strides(&arr_strides),
                    _indices(&indices) {}

                /// @brief Default MemoryIndexer destructor.
                ~MemoryIndexer() = default;

                /**
                 * @brief Calculates the memory index corresponding to the index
                 * parameter for the array/view.
                 *
                 * @param index Array/view index.
                 * @return Memory index.
                 */
                virtual size_t operator()(const size_t index) const = 0;

            public:

                /// @brief Pointer to array/view shape.
                const shape_t* _shape;

                /// @brief Pointer to array/view stride.
                const stride_t* _strides;

                /// @brief Pointer to base array stride.
                const stride_t* _arr_strides;

                /// @brief Pointer to array/view indices.
                const indices_t_v* _indices;
        };
    }
}