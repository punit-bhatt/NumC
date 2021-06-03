# <u>NumC</u>

## <u>Summary</u>

- Library for N-D array manipulation in C++.

## <u>Install</u>

---
**NOTE** - Currently installation only gets the headers into the ./lib folder.

---

```bash
mkdir build | cd build
cmake ..

# <u>Currently this wont install it in the /usr/lib
make | make install
```

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
    ```

- ### <u>Array Slicing</u>
    ```c++
    nc::slices_t slices = { nc::indices_t(2, -1), nc::indices_t(0, -1), nc::indices_t(1, -1)};
    auto s1 = nc::Utils::slice(arr, slices);

    // Nested slicing
    nc::slices_t slices2 = {nc::indices_t(1, 3), nc::indices_t(1, -1)};
    auto s2 = nc::Utils::slice(s1, slices2);
    ```

- ### <u>Display Array</u>
    ```c++
    nc::Utils::print_array(arr);
    ```
    ```json
    [OUT]:
        array(
            shape(3, 3),

            indices(<0, 3>, <0, 3>),

            strides(3, 1),

            [[0.000000, 1.000000, 2.000000],
            [3.000000, 4.000000, 5.000000],
            [6.000000, 7.000000, 8.000000]]
        )
    ```