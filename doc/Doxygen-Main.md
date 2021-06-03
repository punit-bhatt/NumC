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

### Display Array

    nc::Utils::print_array(arr);

#### [OUT]:

    array(
        shape(3, 3),

        indices(<0, 3>, <0, 3>),

        strides(3, 1),

        [[0.000000, 1.000000, 2.000000],
        [3.000000, 4.000000, 5.000000],
        [6.000000, 7.000000, 8.000000]]
    )
