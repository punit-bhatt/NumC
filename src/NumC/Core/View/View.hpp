#pragma once

#define VIEW NumC::Core::View

#include <NumC/Core/NdArray.hpp>

namespace NumC
{
    namespace Core
    {
        /**
         * @brief The base class for a modified view of an N-D base array.
         *
         * @tparam T Array element data type.
         */
        template<typename T>
        class View : public NdArray<T>
        {

            public:
                /// Aliases
                using dtype = T;

                /**
                 * @brief A do-nothing destructor to ensure pointers to this
                 * interface can be deleted.
                 *
                 */
                virtual ~View() {};

                /**
                 * @brief Gets the corresponding memory block/ data array index
                 * for this index.
                 *
                 * @param index Index for an element of this view.
                 * @return Data array index.
                 */
                virtual const size_t get_memory_index(const size_t index) = 0;

                /**
                 * @brief Gets the nd array reference.
                 *
                 * @return Reference to the nd array
                 */
                virtual NdArray<dtype>* const get_arr()
                {
                    return this->_arr;
                }

            protected:

                /// @brief Pointer to the array object.
                NdArray<dtype>* _arr;
        };
    }
}