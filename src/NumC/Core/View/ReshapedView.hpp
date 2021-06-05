#pragma once

#define RESHAPED_VIEW NumC::Core::ReshapedView

#include <NumC/Core/View/View.hpp>

namespace NumC
{
    namespace Core
    {
        /**
         * @brief Reshaped view of the array object.
         *
         * A reshape can potentially change the number of dimensions and the
         * dimensions themselves. This would lead to change in the strides
         * as well. However, it cannot change the total number of units.
         */
        template<typename T>
        class ReshapedView : public View<T>
        {
            public:
                /// Aliases
                using dtype = T;
                using dtype_ptr = dtype*;
                using dtype_ref = dtype&;

                /**
                 * @brief Construct a new Reshaped View object.
                 *
                 * @note Reshaped View can have different shape/dimensions
                 * along with the overall number of dimensions. However, the
                 * number of elements/units can't differ from that of the
                 * array being reshaped.
                 *
                 * @warning Reshaping of other type of Views @b not
                 * @b supported.
                 * Reshaping is only allowed upon memory containers or other
                 * Reshaped Views.
                 *
                 * @param array Pointer to the array object.
                 * @param newShape The new shape/dims to reshape the array to.
                 */
                ReshapedView(NdArray<dtype>* array, const shape_t& newShape)
                {
                    // Reshaping only works directly on memory containers or on
                    // other reshaped views.
                    // Reshape of other views not supported at the moment.
                    if (dynamic_cast<View<dtype>*>(array) != nullptr &&
                        dynamic_cast<ReshapedView<dtype>*>(array) == nullptr)
                    {
                        std::cout << "ERROR - reshape - 1" << std::endl;
                        // throw error.
                    }

                    if (newShape.empty())
                    {
                        std::cout << "ERROR - reshape - 2" << std::endl;
                        // throw error
                    }

                    size_t nunits = 1;

                    for (auto element: newShape)
                    {
                        if (element <= 0)
                        {
                            std::cout << "ERROR - reshape - 3" << std::endl;
                            // throw error.
                        }

                        nunits *= element;
                    }

                    auto arr_shape = array->shape();
                    size_t arr_nunits =
                        std::accumulate(
                            arr_shape.begin(),
                            arr_shape.end(),
                            1,
                            std::multiplies<size_t>());

                    if (arr_nunits != nunits)
                    {
                        std::cout << "ERROR - reshape - 4" << std::endl;
                        // throw error.
                    }

                    size_t prev_dims = 1;
                    this->_nunits = nunits;

                    for (auto element: newShape)
                    {
                        prev_dims *= element;

                        this->_strides.push_back(this->_nunits / prev_dims);
                        this->_dims.push_back(element);
                        this->_indices.push_back(indices_t(0, element));
                    }

                    this->_arr = array;
                    if (dynamic_cast<View<dtype>*>(array) != nullptr)
                    {
                        this->_arr =
                            dynamic_cast<View<dtype>*>(array)->get_arr();
                    }
                }

                /**
                 * @brief Default Reshaped View destructor.
                 *
                 */
                ~ReshapedView() = default;

                /**
                 * @copydoc NdArray::begin()
                 *
                 * Overridden function.
                 */
                Iterator<dtype> begin()
                {
                    return Iterator<dtype>(
                        this->_arr->data() + this->get_memory_index(0),
                        this->_nunits);
                }

                /**
                 * @copydoc NdArray::end()
                 *
                 * Overridden function.
                 */
                Iterator<dtype> end()
                {
                    return Iterator<dtype>(
                        this->_arr->data() +
                        this->get_memory_index(this->_nunits));
                }

                /**
                 * @copydoc View::get_memory_index()
                 *
                 * Overridden function.
                 *
                 * @note Since slicing or other views are not allowed for
                 * reshape, the number of units are same for the reshaped view
                 * and the container.
                 * Therefor the index can be directly interpreted as the memory
                 * container index.
                 * Calculations will need to be done if we are to support
                 * reshape of other views.
                 */
                const size_t get_memory_index(const size_t index) override
                {
                    if (index < 0 || index > this->_nunits)
                    {
                        std::cout << "ERROR - reshape - 7" << std::endl;
                        // throw error.
                    }

                    return index;
                }
        };
    }
}