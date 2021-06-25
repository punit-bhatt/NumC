# <u>NumC</u>

## <u>Summary</u>

- Library for N-D array manipulation in C++.

## <u>Example</u>

- ### <u>Array Creation</u>

    ```c++
    // 1D Array Initialization.
    auto arr1 = ND_ARRAY<nc::float32>({1 ,2 ,3});
    ```
    ```c++
    // 2D Array Initialization.
    auto arr2 = ND_ARRAY<nc::float32>({{0, 1, 2}, {3, 4, 5}, {6, 7, 8}});
    ```
    ```c++
    // 3D Array Initialization.
    auto arr3 = ND_ARRAY<nc::float32>({{{0, 1, 2}}, {{3, 4, 5}}, {{6, 7, 8}}});
    ```
    ```c++
    // Creating 4D Array by defining shape.
    nc::shape_t shape = {5, 3, 3, 2};
    auto arr4 = ND_ARRAY<nc::float32>(shape);
    ```

- ### <u>Array Iteration</u>
    ```c++
    // Iterating throughout the array to get and set values.
    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        *it *= 10;
        std::cout << *it << std::endl;
    }

    // Constant iterator.
    for (auto it = arr.cbegin(); it != arr.cend(); ++it)
        *it *= 10; // Error - cannot assign value to read-only location.
    ```

- ### <u>Array Basic Operations</u>
    ```c++
    auto arr1 = ND_ARRAY<nc::float32>({0, 1, 2, 3, 4});
    auto arr2 = ND_ARRAY<nc::int32>({5, 6, 7, 8, 9});
    auto arr3 = ND_ARRAY<nc::uint16>({10, 11, 12, 13, 14});
    auto arr4 = ND_ARRAY<nc::float64>({100, 100, 100, 100, 100});

    auto res1 = (arr1 + arr2 - arr3) * arr3 / arr4;
    ```

- ### <u>Array Broadcasted Operations</u>
    ```c++
    // Shape - (3, 1, 2)
    auto arr1 = ND_ARRAY<nc::float32>({{{1, 2}}, {{3, 4}}, {{5, 6}}});

    // Shape - (3, 2)
    auto arr2 = ND_ARRAY<nc::int32>({{1, 2} , {3, 4}, {5, 6}});

    // Shape - (2)
    auto arr3 = ND_ARRAY<nc::int32>({10, 20});

    auto bd_arr = arr3 - (arr1 + arr2);
    ```

- ### <u>Array Slicing</u>
    ```c++
    nc::slices_t slices = { nc::indices_t(2, -1), nc::indices_t(0, -1), nc::indices_t(1, -1)};
    auto s1 = nc::Utils::slice(arr, slices);

    // Nested slicing
    nc::slices_t slices2 = {nc::indices_t(1, 3), nc::indices_t(1, -1)};
    auto s2 = nc::Utils::slice(s1, slices2);
    ```

- ### <u>Array Reshaping</u>
    ```c++
    nc::shape_t newShape1 = {10, 9};
    auto r1 = nc::Utils::reshape(arr, newShape1);

    // Nested reshaping
    nc::shape_t newShape2 = {15, -1, 1, 1};
    auto r2 = nc::Utils::reshape(r1, newShape2);
    ```

- ### <u>Array Transposing</u>
    ```c++
    nc::size_t_v axes = {1, 0, 2, 3};
    auto t1 = nc::Utils::transpose(arr, axes);

    // Default transposing.
    auto t2 = nc::Utils::transpose(arr);
    ```

- ### <u>Display Array</u>
    ```c++
    nc::Utils::print_array(arr);
    ```
    ```
    [OUT]:
        array(
            shape(3, 3),

            indices([0, 3), [0, 3)),

            strides(3, 1),

            [[0.000000, 1.000000, 2.000000],
            [3.000000, 4.000000, 5.000000],
            [6.000000, 7.000000, 8.000000]]
        )
    ```