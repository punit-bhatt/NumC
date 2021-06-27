#pragma once

#define MEMORY_INDEXER NumC::Core::MemoryIndexer

#include <NumC/Core/Type.hpp>

namespace NumC
{
    namespace Core
    {
        /**
         * @brief Base memory indexer functor class. Helps array/view implement
         * their own logic for memory access and pass it to the iterator.
         */
        class MemoryIndexer
        {
            public:

                /// @brief Default MemoryIndexer constructor.
                MemoryIndexer() = default;

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
        };
    }
}