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
                 * One shape dimension can be -1. In this case, the value is
                 * inferred from the length of the array and remaining
                 * dimensions.
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

                    auto arr_shape = array->shape();
                    size_t arr_nunits =
                        std::accumulate(
                            arr_shape.begin(),
                            arr_shape.end(),
                            1,
                            std::multiplies<size_t>());

                    size_t n_pos_units = 1;
                    size_t negative_index = -1;

                    for (size_t i = 0; i < newShape.size(); ++i)
                    {
                        if (newShape[i] == 0)
                        {
                            std::cout << "ERROR - reshape - 3" << std::endl;
                            // throw error.
                        }
                        else if (newShape[i] < 0)
                        {
                            if (negative_index == -1)
                            {
                                negative_index = i;
                            }
                            else
                            {
                                std::cout << "ERROR - reshape - 5" << std::endl;
                                // throw error.
                            }
                        }
                        else
                        {
                            n_pos_units *= newShape[i];
                        }
                    }

                    if (negative_index == -1 && arr_nunits != n_pos_units)
                    {
                        std::cout << "ERROR - reshape - 4" << std::endl;
                        // throw error.
                    }

                    size_t other_dims = 1;
                    this->_nunits = n_pos_units;

                    // Copying shape as is.
                    for (auto element: newShape)
                        this->_dims.push_back(element);

                    // Replacing the negative element.
                    if (negative_index >= 0)
                    {
                        this->_dims[negative_index] = arr_nunits / n_pos_units;

                        // Correcting total element count.
                        this->_nunits *= this->_dims[negative_index];
                    }

                    size_t prev_dims = 1;

                    for (auto element: this->_dims)
                    {
                        prev_dims *= element;

                        this->_strides.push_back(this->_nunits / prev_dims);
                        this->_indices.push_back(indices_t(0, element));
                    }

                    this->_arr = array;
                    if (dynamic_cast<View<dtype>*>(array) != nullptr)
                    {
                        this->_arr =
                            dynamic_cast<View<dtype>*>(array)->get_arr();
                    }
                }

                /// @brief Default Reshaped View destructor.
                ~ReshapedView() = default;

                /**
                 * @copydoc MemoryIndexer::operator()()
                 *
                 * Overridden function.
                 *
                 * @note Since slicing or other views are not allowed
                 * for reshape, the number of units are same for the
                 * reshaped view and the container.
                 * Therefor the index can be directly interpreted as the
                 * memory container index.
                 * Calculations will need to be done if we are to
                 * support reshape of other views.
                 */
                size_t operator()(const size_t index) const override
                {
                    return index;
                }
        };
    }
}