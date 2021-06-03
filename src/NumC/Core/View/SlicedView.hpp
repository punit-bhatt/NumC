#pragma once

#define SLICED_VIEW NumC::Core::SlicedView

#include <NumC/Core/View/View.hpp>
#include <NumC/Core/Iterator/ViewIterator.hpp>

namespace NumC
{
    namespace Core
    {
        /**
         * @brief Sliced view of the array object.
         *
         * A slice can potentially change the start and end index per
         * dimension. Therefore, it could change the total number of
         * elements/units viewed. However, it cannot change the overall
         * count of dimensions and strides per dimension.
         *
         * @tparam T Array element data type.
         */
        template<typename T>
        class SlicedView : public View<T>
        {
            public:
                /// Aliases
                using dtype = T;
                using dtype_ptr = dtype*;
                using dtype_ref = dtype&;

                /**
                 * @brief Construct a new Sliced View object.
                 *
                 * @note A slice of a view will change it to the
                 * corresponding slice of the actual parent/orig array
                 * object. I.e. each sliced view's get_arr() will fetch the
                 * actual parent/orig ndarray regardless of whether this was
                 * created from some view.
                 * Eg. 1D array -> indices = (0, 5)
                 * Slice(array, (2, -1)) -> indices = (2, 5)
                 * Slice(above_slice, (0, 2)) -> indices = (2, 4)
                 *
                 * @warning Slicing of other type of Views @b not
                 * @b supported.
                 * Slicing is only allowed upon memory containers or other
                 * Sliced Views.
                 *
                 * @param array Pointer to array object.
                 * @param slices The slice indices along each dimension.
                 */
                SlicedView(NdArray<dtype>* array, slices_t& slices)
                {
                    // Slicing only works directly on memory containers or on
                    // other sliced views.
                    // Slicing of other view types not supported at the moment.
                    if (dynamic_cast<View<dtype>*>(array) != nullptr &&
                        dynamic_cast<SlicedView<dtype>*>(array) == nullptr)
                    {
                        std::cout << "ERROR - sliced - 4" << std::endl;
                        // throw error.
                    }

                    if (slices.empty() || slices.size() > array->shape().size())
                    {
                        std::cout << "ERROR - sliced - 1" << std::endl;
                        // throw error.
                    }

                    auto it = slices.begin(), ite = slices.end();
                    auto arr_shape = array->shape();

                    for (size_t i = 0; it != ite; ++i, ++it)
                    {
                        // Verifies that each start index is between 0 and
                        // corresponding dims/shape count. Also, verifies that
                        // end index is greater than start index or -1.
                        if ((*it).first < 0 || (*it).first >= arr_shape[i] ||
                            ((*it).second != -1 && (*it).second < (*it).first) ||
                            (*it).second > arr_shape[i])
                        {
                            std::cout << "ERROR - sliced - 2" << std::endl;
                            // throw error
                        }
                    }

                    size_t arr_ndims = arr_shape.size(), nslices = slices.size();
                    auto indices = array->indices();

                    for (size_t i = 0; i < arr_ndims; ++i)
                    {
                        if (i >= nslices)
                        {
                            this->_indices.push_back(
                                indices_t(indices[i].first, indices[i].second));
                            this->_dims.push_back(arr_shape[i]);

                            continue;
                        }

                        // Modifying the slices.
                        // Say, original array -> (5) and current slice view ->
                        // (1 -> 3).
                        // Slice (0, 1) of orig array -> (0 + 0, 0 + 1)
                        // Slice (0, 1) of this view would actually
                        // (1 + 0, 1 + 1)
                        this->_indices.push_back(
                            indices_t(
                                indices[i].first + slices[i].first,
                                slices[i].second == -1 ?
                                    indices[i].second :
                                    indices[i].first + slices[i].second));

                        // Just second - first is enough. Second is always 1 +
                        // last_index.
                        this->_dims.push_back(
                            this->_indices[i].second - this->_indices[i].first);
                    }

                    // Populating properties.
                    this->_nunits =
                        std::accumulate(
                            this->_dims.begin(),
                            this->_dims.end(),
                            1,
                            std::multiplies<size_t>());
                    // Populating strides based on the new dimensions.
                    for (size_t i = 0, prev_dims = 1; i < arr_ndims; ++i)
                    {
                        prev_dims *= this->_dims[i];
                        this->_strides.push_back(this->_nunits / prev_dims);
                    }

                    this->_arr = array;
                    if (dynamic_cast<View<dtype>*>(array) != nullptr)
                    {
                        this->_arr =
                            dynamic_cast<View<dtype>*>(array)->get_arr();
                    }
                }

                /**
                 * @brief Default Sliced View destructor.
                 *
                 */
                ~SlicedView() = default;

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
                dtype get(size_t index) override
                {
                    if (index < 0 && index >= this->_nunits)
                    {
                        std::cout << "ERROR - slice - 1" << std::endl;
                        // throw error
                    }

                    return this->_arr->get(this->get_memory_index(index));
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
                        std::cout << "ERROR - slice - 2" << std::endl;
                        // throw error
                    }

                    this->_arr->set(this->get_memory_index(index), value);
                }

                /**
                 * @copydoc NdArray::begin()
                 *
                 * Overridden function.
                 */
                ViewIterator<dtype> begin()
                {
                    return ViewIterator<dtype>(
                        this->_arr->data() + this->get_memory_index(0),
                        this->strides(),
                        this->indices(),
                        this->_arr->strides());
                }

                /**
                 * @copydoc NdArray::end()
                 *
                 * Overridden function.
                 */
                ViewIterator<dtype> end()
                {
                    return ViewIterator<dtype>(
                        this->_arr->data() +
                        this->get_memory_index(this->_nunits));
                }

                /**
                 * @copydoc View::get_memory_index()
                 *
                 * Overridden function.
                 */
                const size_t get_memory_index(const size_t index) override
                {
                    if (index < 0 || index > this->_nunits)
                    {
                        std::cout << "ERROR - slice - 3" << std::endl;
                        // throw error.
                    }

                    size_t ndims = this->_dims.size();
                    auto arr_strides = this->_arr->strides();

                    size_t pos = 0, c_index = index;

                    for (size_t i = 0; i < ndims; ++i)
                    {
                        size_t dim_count =
                            this->_indices[i].first +
                            c_index / this->_strides[i];

                        pos += (arr_strides[i] * dim_count);
                        c_index %= this->_strides[i];
                    }

                    return pos;
                }
        };
    }
}