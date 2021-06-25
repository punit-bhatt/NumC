#pragma once

#define ND_ARRAY NumC::Core::NdArray

#include <NumC/Core/Iterator/Iterator.hpp>
#include <NumC/Core/Iterator/CIterator.hpp>

#include <memory>
#include <functional>

namespace NumC
{
    namespace Core
    {
        /**
         * @brief The basic N-D array class that defines the way data is stored
         * and traversed.
         *
         * @tparam T The data type to be stored.
         */
        template<typename T>
        class NdArray
        {
            public:
                /// Aliases
                using dtype = T;
                using dtype_ptr = dtype*;
                using dtype_ref = dtype&;
                using dtype_shrd_ptr = std::shared_ptr<dtype>;
                using const_dtype_ref = const dtype&;
                using init_1d = std::initializer_list<dtype>;
                using init_2d = std::initializer_list<init_1d>;
                using init_3d = std::initializer_list<init_2d>;

                /**
                 * @brief Default Nd Array constructor.
                 *
                 */
                NdArray() {}

                /**
                 * @brief Construct a new Nd Array object based on the shape
                 * provided.
                 *
                 * @param shape The shape/dimensions of array to be created.
                 */
                NdArray(const shape_t& shape)
                {
                    if (shape.empty())
                    {
                        std::cout << "ERROR - nd - 1" << std::endl;
                        // throw error
                    }

                    // Validation.
                    for (auto s : shape)
                    {
                        if (s <= 0)
                        {
                            std::cout << "ERROR - nd - 2" << std::endl;
                            // throw error
                        }
                    }

                    this->_nunits =
                        std::accumulate(
                            shape.begin(),
                            shape.end(),
                            1,
                            std::multiplies<size_t>());

                    for (size_t i = 0, prev_dims = 1; i < shape.size(); ++i)
                    {
                        prev_dims *= shape[i];

                        // Populating shape, strides and indices for Nd array.
                        this->_dims.push_back(shape[i]);
                        this->_strides.push_back(this->_nunits / prev_dims);
                        this->_indices.push_back(indices_t(0, shape[i]));
                    }

                    this->__data =
                        dtype_shrd_ptr(
                            (dtype_ptr)malloc(sizeof(dtype) * this->_nunits));
                }

                /**
                 * @brief Construct a 1D array object
                 *
                 * @param list Array list to be used for 1d array creation.
                 */
                NdArray(init_1d list)
                {
                    this->_nunits = list.size();
                    this->_dims.push_back(list.size());
                    this->_strides.push_back(1);
                    this->_indices.push_back(indices_t(0, list.size()));
                    this->__data =
                        dtype_shrd_ptr(
                            (dtype_ptr)malloc(sizeof(dtype) * this->_nunits));
                    this->__copy_data(list);
                }

                /**
                 * @brief Construct a 2D array object
                 *
                 * @param list_2d Array list to be used for 2d array creation.
                 */
                NdArray(init_2d list_2d)
                {
                    if (list_2d.size() == 0)
                    {
                        std::cout << "ERROR - nd - 3" << std::endl;
                        // throw error
                    }

                    auto list_1d_ptr = list_2d.begin();

                    for (size_t i = 0; i < list_2d.size() - 1; ++i)
                    {
                        if (list_1d_ptr[i].size() == 0 ||
                            (list_1d_ptr[i].size() != list_1d_ptr[i + 1].size()))
                        {
                            std::cout << "ERROR - nd - 4" << std::endl;
                            // throw error
                        }
                    }

                    size_t m = list_2d.size(), n = list_1d_ptr[0].size();

                    this->_nunits = m * n;

                    this->_dims.push_back(m);
                    this->_dims.push_back(n);

                    this->_strides.push_back(n);
                    this->_strides.push_back(1);

                    this->_indices.push_back(indices_t(0, m));
                    this->_indices.push_back(indices_t(0, n));

                    this->__data =
                        dtype_shrd_ptr(
                            (dtype_ptr)malloc(sizeof(dtype) * this->_nunits));

                    // Copying data.
                    for (size_t i = 0, r = 0; i < m; ++i, r = r + n)
                        this->__copy_data(list_1d_ptr[i], r);
                }

                /**
                 * @brief Construct a 3D array object
                 *
                 * @param list_3d Array list to be used for 3d array creation.
                 */
                NdArray(init_3d list_3d)
                {
                    if (list_3d.size() == 0)
                    {
                        std::cout << "ERROR - nd - 4" << std::endl;
                        // throw error
                    }

                    auto list_2d_ptr = list_3d.begin();
                    size_t n = list_2d_ptr[0].size();

                    for (size_t i = 0; i < list_3d.size(); ++i)
                    {
                        if (list_2d_ptr[i].size() == 0 ||
                            (list_2d_ptr[i].size() != n))
                        {
                            std::cout << "ERROR - nd - 5" << std::endl;
                            // throw error
                        }

                        auto list_1d_ptr = list_2d_ptr[i].begin();

                        for (size_t j = 0; j < n - 1; ++j)
                        {
                            if (list_1d_ptr[j].size() == 0||
                                (list_1d_ptr[j].size() !=
                                    list_1d_ptr[j + 1].size()))
                            {
                                std::cout << "ERROR - nd - 6" << std::endl;
                                // throw error
                            }
                        }
                    }

                    size_t m = list_3d.size();
                    size_t o = list_2d_ptr[0].begin()[0].size();

                    this->_nunits = m * n * o;

                    this->_dims.push_back(m);
                    this->_dims.push_back(n);
                    this->_dims.push_back(o);

                    this->_strides.push_back(n * o);
                    this->_strides.push_back(o);
                    this->_strides.push_back(1);

                    this->_indices.push_back(indices_t(0, m));
                    this->_indices.push_back(indices_t(0, n));
                    this->_indices.push_back(indices_t(0, o));

                    this->__data =
                        dtype_shrd_ptr(
                            (dtype_ptr)malloc(sizeof(dtype) * this->_nunits));

                    for (size_t i = 0, r = 0; i < m; ++i)
                    {
                        auto list_1d_ptr = list_2d_ptr[i].begin();

                        for (size_t j = 0; j < n; ++j, r = r + o)
                            this->__copy_data(list_1d_ptr[j], r);
                    }
                }

                /// @brief Destroy the Nd Array object.
                ~NdArray() = default;

                /**
                 * @brief Gets the dimensions/shape of the array.
                 *
                 * @return List of shape/dimensions.
                 */
                virtual const shape_t& shape() const
                {
                    return this->_dims;
                }

                /**
                 * @brief Gets the strides along each dimension.
                 *
                 * @return List of strides along each dimensions.
                 */
                virtual const stride_t& strides() const
                {
                    return this->_strides;
                }

                /**
                 * @brief Gets the pair of start and end indices along each
                 * dimension.
                 *
                 * @return List of indices.
                 */
                virtual const indices_t_v& indices() const
                {
                    return this->_indices;
                }

                /**
                 * @brief Gets the array element from memory.
                 *
                 * @warning Index passed is used on a flattened array.
                 *
                 * @param index Index of element to be read.
                 * @return Value at the index.
                 */
                virtual dtype get(size_t index) const
                {
                    if (index < 0 && index >= this->_nunits)
                    {
                        std::cout << "ERROR - nd - 5" << std::endl;
                        // throw error
                    }

                    return this->__data.get()[index];
                }

                /**
                 * @brief Updates the array element value.
                 *
                 * @warning Index passed is used on a flattened array.
                 *
                 * @param index Index of element to be updated.
                 * @param value Value to be set.
                 */
                virtual void set(size_t index, dtype value)
                {
                    if (index < 0 && index >= this->_nunits)
                    {
                        std::cout << "ERROR - nd - 6" << std::endl;
                        // throw error
                    }

                    this->__data.get()[index] = value;
                }

                /**
                 * @brief Iterator pointing to the first element.
                 *
                 * @return Iterator pointing to the start.
                 */
                virtual Iterator<dtype> begin()
                {
                    return Iterator<dtype>(
                        this->__data.get(),
                        0,
                        this->_nunits);
                }

                /**
                 * @brief Iterator pointing to last element + 1.
                 *
                 * @return Iterator pointing to the end.
                 */
                virtual Iterator<dtype> end()
                {
                    return Iterator<dtype>(this->__data.get(), this->_nunits);
                }

                /**
                 * @brief Constant iterator pointing to the first element.
                 *
                 * @return Constant terator pointing to the start.
                 */
                virtual CIterator<dtype> cbegin() const
                {
                    return CIterator<dtype>(
                        this->__data.get(),
                        0,
                        this->_nunits);
                }

                /**
                 * @brief Constant iterator pointing to last element + 1.
                 *
                 * @return Constant iterator pointing to the end.
                 */
                virtual CIterator<dtype> cend() const
                {
                    return CIterator<dtype>(this->__data.get(), this->_nunits);
                }

                /**
                 * @brief Gets the pointer to the data array.
                 *
                 * @return Pointer to the data array.
                 */
                dtype_ptr data()
                {
                    return this->__data.get();
                }

                /// @brief Default assignment operator.
                NdArray<dtype>&
                operator=(NdArray<dtype> const& other) = default;

                /**
                 * @brief Element-wise add operator overload.
                 * Performs the operation on 2 arrays if their shape is same and
                 * returns a new array containing the result.
                 *
                 * @tparam rhs_t RHS array element type. Allows operation on
                 * arrays of 2 different types.
                 * @param rhs Rhs array reference.
                 * @return New array containing the result.
                 */
                template<typename rhs_t>
                NdArray<dtype> operator+(const NdArray<rhs_t>& rhs) const
                {
                    std::function<dtype(dtype, rhs_t)> func =
                        [] (dtype x, rhs_t y) -> dtype { return x + y; };

                    return broadcast(*this, rhs, func);
                }

                /**
                 * @brief Element-wise subtract operator overload.
                 * Performs the operation on 2 arrays if their shape is same and
                 * returns a new array containing the result.
                 *
                 * @tparam rhs_t RHS array element type. Allows operation on
                 * arrays of 2 different types.
                 * @param rhs Rhs array reference.
                 * @return New array containing the result.
                 */
                template<typename rhs_t>
                NdArray<dtype> operator-(const NdArray<rhs_t>& rhs) const
                {
                    std::function<dtype(dtype, rhs_t)> func =
                        [] (dtype x, rhs_t y) -> dtype { return x - y; };

                    return broadcast(*this, rhs, func);
                }

                /**
                 * @brief Element-wise multiply operator overload.
                 * Performs the operation on 2 arrays if their shape is same and
                 * returns a new array containing the result.
                 *
                 * @tparam rhs_t RHS array element type. Allows operation on
                 * arrays of 2 different types.
                 * @param rhs Rhs array reference.
                 * @return New array containing the result.
                 */
                template<typename rhs_t>
                NdArray<dtype> operator*(const NdArray<rhs_t>& rhs) const
                {
                    std::function<dtype(dtype, rhs_t)> func =
                        [] (dtype x, rhs_t y) -> dtype { return x * y; };

                    return broadcast(*this, rhs, func);
                }

                /**
                 * @brief Element-wise division operator overload.
                 * Performs the operation on 2 arrays if their shape is same and
                 * returns a new array containing the result.
                 *
                 * @note A 0 element divisor results in +- inf.
                 *
                 * @tparam rhs_t RHS array element type. Allows operation on
                 * arrays of 2 different types.
                 * @param rhs Rhs array reference.
                 * @return New array containing the result.
                 */
                template<typename rhs_t>
                NdArray<dtype> operator/(const NdArray<rhs_t>& rhs) const
                {
                    std::function<dtype(dtype, rhs_t)> func =
                        [] (dtype x, rhs_t y) -> dtype { return x / y; };

                    return broadcast(*this, rhs, func);
                }

            protected:

                /// @brief Number of units/elements in the N-D array.
                size_t _nunits;

                /// @brief List of dimensions in array/view.
                shape_t _dims;

                /// @brief  of strides per dimension in array/view.
                stride_t _strides;

                /**
                 * @brief List of pair of indices along each dimension of array
                 * object.
                 */
                indices_t_v _indices;

            private:

                /// @brief 1-D array storing the actual data.
                dtype_shrd_ptr __data;

                /**
                 * @brief Internal helper method to copy data from 1D
                 * initializer list to the data array.
                 *
                 * @param list Initializer list to be copied.
                 * @param start Start index value. Defaults to 0.
                 */
                void __copy_data(init_1d list, size_t start = 0)
                {
                    auto it = list.begin(), ite = list.end();

                    for (size_t i = start; it != ite; ++i, ++it)
                        this->__data.get()[i] = *it;
                }

                /**
                 * @brief Static method validating if broadcasting would be
                 * possible for the given array shapes.
                 *
                 * @note Broadcasting rule (both vectors right-based) - In order
                 * to broadcast, the size of the trailing axes for both arrays
                 * in an operation must either be the same size or one of them
                 * must be one.
                 *
                 * @param lhs Reference to the LHS array shape (considered the
                 * bigger one).
                 * @param rhs Reference to the RHS array shape.
                 *
                 * @return Resultant broadcasted array shape.
                 */
                static shape_t
                validate_broadcast(const shape_t& lhs, const shape_t& rhs)
                {
                    // This ensures that LHS is always the bigger vector.
                    if (lhs.size() < rhs.size())
                        return validate_broadcast(rhs, lhs);

                    shape_t result_shape(lhs.size());
                    size_t l = lhs.size();

                    for (size_t r = rhs.size(); r >= 0; --r, --l)
                    {
                        if (rhs[r] != lhs[l] && rhs[r] > 1 && lhs[l] > 1)
                        {
                            std::cout << "ERROR - broadcast - 1" << std::endl;
                            // throw error.
                        }

                        result_shape[l] = std::max(rhs[r], lhs[l]);
                    }

                    while (l >= 0)
                        result_shape[l--] = lhs[l];

                    return result_shape;
                }

                /**
                 * @brief Static helper method that performs an element level
                 * operation on two arrays that need not have the same shape.
                 *
                 * @note This is done by broadcasting the shape such that the
                 * resultant shape contains max(lhs_dims, rhs_dims) and where
                 * the ith shape value is max(lhs_dims[i], rhs_dims[i]).
                 * @htmlonly
                 * In case of a valid configuration, either lhs_dims[i] ==
                 * rhs_dims[i] or one of the two is 1. A right-based comparison
                 * is done. <br>Example, A(2, 1, 3) and B(2, 3). Then B is
                 * inlined towards the right.
                 * <br>(2, 1, 3)
                 * <br>(_, 2, 3)
                 * <br>Thus, a valid configuration and the resultant shape would
                 * be (2, 2, 3).
                 * @endhtmlonly
                 *
                 * @warning Scalar broadcasting yet to be implemented.
                 *
                 * @tparam lhs_t LHS array element type.
                 * @tparam rhs_t RHS array element type.
                 * @tparam res_t Returned array element type.
                 * @param lhs Reference to the LHS array.
                 * @param rhs Reference to the RHS array.
                 * @param element_op Reference to the operation function to be
                 * performed between elements of the 2 arrays.
                 * @return New array of the broadcasted shape containing the
                 * result.
                 */
                template<typename lhs_t, typename rhs_t, typename res_t>
                static NdArray<res_t>
                broadcast(
                    const NdArray<lhs_t>& lhs,
                    const NdArray<rhs_t>& rhs,
                    std::function<res_t(lhs_t, rhs_t)>& element_op)
                {
                    auto result_shape =
                        validate_broadcast(lhs.shape(), rhs.shape());
                    auto result = NdArray<res_t>(result_shape);

                    auto lhs_shape = lhs.shape();
                    auto rhs_shape = rhs.shape();
                    auto n_lhs = lhs_shape.size();
                    auto n_rhs = rhs_shape.size();
                    auto n_res = result_shape.size();

                    /**
                     * @brief Internal helper lambda that's not generic enough
                     * to be exposed. Helps recursively go through each element
                     * in a broadcasted method and perform the required
                     * operation.
                     */
                    std::function<void(size_t, size_t, size_t, size_t)>
                    broadcast_helper =
                        [&](size_t axis,size_t lhs_i, size_t rhs_i, size_t res_i)
                        {
                            size_t lhs_pre = lhs_i;
                            size_t rhs_pre = rhs_i;
                            size_t res_pre = res_i;

                            for (size_t i = 0; i < result_shape[axis]; ++i)
                            {
                                // Ensures that index is not updated in case the
                                // right- based shape vector hasn't begun yet.
                                // Indices would begin once n_res - axis is
                                // equal to n_l.
                                // Correspondin index 0 would be given by
                                // n_l - (n_res - axis) or n_l + axis - n_res.
                                // The reason this is checked for both is
                                // because either could be the smaller array.
                                if (n_lhs + axis >= n_res)
                                {
                                    // The above condition would always be true
                                    // for the larger array.
                                    // Thus, the following ternary to ensure
                                    // that the index formular is used only if
                                    // array is smaller.
                                    auto axis_l =
                                        n_res > n_lhs ?
                                        n_lhs + axis - n_res : axis;
                                    lhs_i =
                                        lhs_pre +
                                        (i % lhs_shape[axis_l]) *
                                            lhs.strides()[axis_l];
                                }

                                if (n_rhs + axis >= n_res)
                                {
                                    auto axis_r =
                                        n_res > n_rhs ?
                                        n_rhs + axis - n_res : axis;
                                    rhs_i =
                                        rhs_pre +
                                        (i % rhs_shape[axis_r]) *
                                            rhs.strides()[axis_r];
                                }

                                res_i = res_pre + i * result.strides()[axis];

                                // Recurse until the final axis is reached.
                                // Perform the element operation then.
                                if (n_res - 1 > axis)
                                    broadcast_helper(
                                        axis + 1,
                                        lhs_i,
                                        rhs_i,
                                        res_i);
                                else
                                    result.set(
                                        res_i,
                                        element_op(
                                            lhs.get(lhs_i),
                                            rhs.get(rhs_i)));
                            }
                        };

                    broadcast_helper(0, 0, 0, 0);

                    return result;
                }
        };
    }
}