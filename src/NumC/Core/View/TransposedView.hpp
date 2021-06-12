#pragma once

#define TRANSPOSED_VIEW NumC::Core::TransposedView

#include <NumC/Core/View/View.hpp>

#include <algorithm>

namespace NumC
{
    namespace Core
    {
        /**
         * @brief Transposed view of the array object.
         *
         * A transpose will change the order of the dimensions and thus, affect
         * the strides. However, it can't change the overall number of elements.
         */
        template<typename T>
        class TransposedView : public View<T>
        {
            public:
                /// Aliases
                using dtype = T;
                using dtype_ptr = dtype*;
                using dtype_ref = dtype&;

                /**
                 * @brief Construct a new Transposed View object.
                 *
                 * @note Transposed View will change the order of the dimensions
                 * and thus, affect the strides. However, it can't change the
                 * overall number of elements.
                 *
                 * @param array Pointer to the array object.
                 * @param axes The axes order to transpose to. In case nothing
                 * is passed, defaults to reverse of the original shape.
                 */
                TransposedView(
                    NdArray<dtype>* array,
                    const size_t_v& axes = size_t_v())
                {
                    // Transposing only works directly on memory containers or
                    // on other transposed views.
                    // Transpose of other views not supported at the moment.
                    if (dynamic_cast<View<dtype>*>(array) != nullptr &&
                        dynamic_cast<TransposedView<dtype>*>(array) == nullptr)
                    {
                        std::cout << "ERROR - transpose - 1" << std::endl;
                        // throw error.
                    }

                    auto arr_shape = array->shape();

                    // Copying axes.
                    for (auto axis: axes)
                        this->__axes.push_back(axis);

                    // If no axes information passed then, reverse the original
                    // shape order.
                    if (this->__axes.empty())
                    {
                        this->__axes.resize(arr_shape.size());
                        std::iota(
                            this->__axes.rbegin(),
                            this->__axes.rend(),
                            0);
                    }
                    else if (this->__axes.size() != arr_shape.size())
                    {
                        std::cout << "ERROR - transpose - 2" << std::endl;
                        // throw error.
                    }
                    else
                    {
                        size_t_v orig_axes(this->__axes.size());
                        std::iota(orig_axes.begin(), orig_axes.end(), 0);

                        if (!std::is_permutation(
                            this->__axes.begin(),
                            this->__axes.end(),
                            orig_axes.begin()))
                        {
                            std::cout << "ERROR - transpose - 3" << std::endl;
                            // throw error.
                        }
                    }

                    auto arr_indices = array->indices();
                    size_t prev_dims = 1;
                    this->_nunits =
                        std::accumulate(
                            arr_shape.begin(),
                            arr_shape.end(),
                            1,
                            std::multiplies<size_t>());

                    // Copying shape and indices in the new order.
                    for (auto index: this->__axes)
                    {
                        prev_dims *= arr_shape[index];

                        this->_strides.push_back(this->_nunits / prev_dims);
                        this->_dims.push_back(arr_shape[index]);
                        this->_indices.push_back(arr_indices[index]);
                    }

                    this->_arr = array;
                    if (dynamic_cast<View<dtype>*>(array) != nullptr)
                    {
                        this->_arr =
                            dynamic_cast<View<dtype>*>(array)->get_arr();
                    }

                    // Creating a new TransposedViewIndexer and using that to
                    // populate.
                    this->_memory_indexer = TransposedViewIndexer(this);
                }

                /**
                 * @brief Default Transposed View destructor.
                 *
                 */
                ~TransposedView() = default;

                /**
                 * @copydoc View::memory_indexer()
                 *
                 * Overridden function.
                 */
                MemoryIndexer* const memory_indexer() override
                {
                    return &(this->_memory_indexer);
                }

                /**
                 * @copydoc View::cmemory_indexer()
                 *
                 * Overridden function.
                 */
                const MemoryIndexer* cmemory_indexer() const override
                {
                    return &(this->_memory_indexer);
                }

            private:

                /// @brief The axes order that the array is being transposed to.
                size_t_v __axes;

                /**
                 * @brief Indexer class to calculate memory index for reshaped
                 * views. This class is accessible only to TransposedVIew.
                 */
                class TransposedViewIndexer : public MemoryIndexer
                {
                    public:

                        /// @brief Default TransposedViewIndexer constructor.
                        TransposedViewIndexer() = default;

                        /**
                         * @brief Construct a new Transposed View Indexer object
                         * by calling the memory indexer constructor
                         * using current array/view.
                         *
                         * @param view Pointer to the transposed view.
                         */
                        TransposedViewIndexer(const TransposedView* view) :
                            MemoryIndexer(
                                view->shape(),
                                view->strides(),
                                view->_arr->strides(),
                                view->indices()),
                            __axes(&(view->__axes)) {}

                        /// @brief Default TransposedViewIndexer destructor.
                        ~TransposedViewIndexer() = default;

                        /**
                         * @copydoc MemoryIndexer::operator()()
                         *
                         * Overridden function.
                         *
                         * @note The logic is pretty simple. Find the
                         * coordinates corresponding to the index for the
                         * transposed view. Multiply these by the reordered data
                         * array strides.
                         * @htmlonly
                         * <br>
                         * Example,
                         * <br>(2, 3) - strides(3, 1)
                         * <br>
                         * [0, 1, 2],
                         * <br>
                         * [3, 4, 5].
                         * <br>
                         * Transposed view -
                         * <br>(3, 2) - strides(2, 1)
                         * <br>
                         * [0, 3],
                         * <br>
                         * [1, 4],
                         * <br>
                         * [2, 5]
                         * <br>
                         * Transposed view index 1 would be element 3. Calculate
                         * coordinates for this - 0th row, 1st column.
                         * Multiply by these by the reordered strides -
                         * 0 * 1 + 1 * 3 = 3
                         * @endhtmlonly
                         */
                        size_t operator()(const size_t index) const override
                        {
                            size_t pos = 0, c_index = index;

                            for (size_t i = 0; i < this->_shape->size(); ++i)
                            {
                                size_t dim_count =
                                    this->_indices->at(i).first +
                                    c_index / this->_strides->at(i);

                                pos +=
                                    (this->_arr_strides->at(
                                        this->__axes->at(i)) * dim_count);
                                c_index %= this->_strides->at(i);
                            }

                            return pos;
                        }

                    private:

                        /**
                         * @brief Pointer to the axes order of the transposed
                         * array.
                         */
                        const size_t_v* __axes;
                };

                /// @brief Transposed view indexer instance.
                TransposedViewIndexer _memory_indexer;
        };
    }
}