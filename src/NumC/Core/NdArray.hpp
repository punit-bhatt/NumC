#pragma once

#define ND_ARRAY NumC::Core::NdArray

#include <NumC/Core/Iterator/Iterator.hpp>
#include <memory>

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
                NdArray(shape_t &shape)
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
                virtual dtype get(size_t index)
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
                Iterator<dtype> begin()
                {
                    return Iterator<dtype>(this->__data.get(), this->_nunits);
                }

                /**
                 * @brief Iterator pointing to last element + 1.
                 *
                 * @return Iterator pointing to the end.
                 */
                Iterator<dtype> end()
                {
                    return Iterator<dtype>(this->__data.get() + this->_nunits);
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
                operator=(const NdArray<dtype>& other) = default;

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
        };
    }
}