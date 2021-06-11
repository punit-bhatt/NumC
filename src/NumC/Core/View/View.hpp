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
                 * @brief Gets the memory indexer to calculate data array index
                 * for the view.
                 *
                 * @return Pointer to the memory indexer instance.
                 */
                virtual const MemoryIndexer* get_memory_indexer() const = 0;

                /**
                 * @brief Gets the nd array reference.
                 *
                 * @return Reference to the nd array
                 */
                virtual NdArray<dtype>* const get_arr()
                {
                    return this->_arr;
                }

                /**
                 * @copydoc NdArray::shape()
                 *
                 * Overridden function.
                 */
                const shape_t& shape() const override
                {
                    return this->_dims;
                }

                /**
                 * @copydoc NdArray::strides()
                 *
                 * Overridden function.
                 */
                const stride_t& strides() const override
                {
                    return this->_strides;
                }

                /**
                 * @copydoc NdArray::indices()
                 *
                 * Overridden function.
                 */
                const indices_t_v& indices() const override
                {
                    return this->_indices;
                }

                /**
                 * @copydoc NdArray::get()
                 *
                 * Overridden function.
                 */
                dtype get(size_t index) const override
                {
                    if (index < 0 && index >= this->_nunits)
                    {
                        std::cout << "ERROR - view - 1" << std::endl;
                        // throw error
                    }

                    return this->_arr->get(
                        this->get_memory_indexer()->operator()(index));
                }

                /**
                 * @copydoc NdArray::set()
                 *
                 * Overridden function.
                 */
                void set(size_t index, dtype value) override
                {
                    if (index < 0 && index >= this->_nunits)
                    {
                        std::cout << "ERROR - view - 2" << std::endl;
                        // throw error
                    }

                    this->_arr->set(
                        this->get_memory_indexer()->operator()(index), value);
                }

            protected:

                /// @brief Pointer to the array object.
                NdArray<dtype>* _arr;
        };
    }
}