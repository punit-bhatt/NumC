# NumC

## Summary

- Library for N-D array manipulation in C++.

## Usage

### Array Creation

    // 1D Array Initialization.
    auto arr1 = ND_ARRAY<nc::float32>({1 ,2 ,3});

    // 2D Array Initialization.
    auto arr2 = ND_ARRAY<nc::float32>({{0, 1, 2}, {3, 4, 5}, {6, 7, 8}});

    // 3D Array Initialization.
    auto arr3 = ND_ARRAY<nc::float32>({{{0, 1, 2}}, {{3, 4, 5}}, {{6, 7, 8}}});

    // Creating 4D Array by defining shape.
    nc::shape_t shape = {5, 3, 3, 2};
    auto arr4 = ND_ARRAY<nc::float32>(shape);


### Array Iteration

    // Iterating throughout the array to get and set values.
    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        *it *= 10;
        std::cout << *it << std::endl;
    }

    // Constant iterator.
    for (auto it = arr.cbegin(); it != arr.cend(); ++it)
        *it *= 10; // Error - cannot assign value to read-only location.

### Array Basic Operations

    auto arr1 = ND_ARRAY<nc::float32>({0, 1, 2, 3, 4});
    auto arr2 = ND_ARRAY<nc::int32>({5, 6, 7, 8, 9});
    auto arr3 = ND_ARRAY<nc::uint16>({10, 11, 12, 13, 14});
    auto arr4 = ND_ARRAY<nc::float64>({100, 100, 100, 100, 100});

    auto res1 = (arr1 + arr2 - arr3) * arr3 / arr4;

### Array Broadcasted Operations

    // Shape - (3, 1, 2)
    auto arr1 = ND_ARRAY<nc::float32>({{{1, 2}}, {{3, 4}}, {{5, 6}}});

    // Shape - (3, 2)
    auto arr2 = ND_ARRAY<nc::int32>({{1, 2} , {3, 4}, {5, 6}});

    // Shape - (2)
    auto arr3 = ND_ARRAY<nc::int32>({10, 20});

    auto bd_arr = arr3 - (arr1 + arr2);

    // Scalar broadcasting.
    auto bd_arr2 = arr2 + (arr1 * 100);

### Array Slicing

    nc::slices_t slices = { nc::indices_t(2, -1), nc::indices_t(0, -1), nc::indices_t(1, -1)};
    auto s1 = nc::Utils::slice(arr, slices);

    // Nested slicing
    nc::slices_t slices2 = {nc::indices_t(1, 3), nc::indices_t(1, -1)};
    auto s2 = nc::Utils::slice(s1, slices2);

### Array Reshaping

    nc::shape_t newShape1 = {10, 9};
    auto r1 = nc::Utils::reshape(arr, newShape1);

    // Nested reshaping
    nc::shape_t newShape2 = {15, 6, 1, 1};
    auto r2 = nc::Utils::reshape(r1, newShape2);

### Array Transposing

    nc::size_t_v axes = {1, 0, 2, 3};
    auto t1 = nc::Utils::transpose(arr, axes);

    // Default transposing.
    auto t2 = nc::Utils::transpose(arr);

### Display Array

    nc::Utils::print_array(arr);

#### [OUT]:

    array(
        shape(3, 3),

        indices([0, 3), [0, 3)),

        strides(3, 1),

        [[0.000000, 1.000000, 2.000000],
        [3.000000, 4.000000, 5.000000],
        [6.000000, 7.000000, 8.000000]]
    )
