#include <NumC.hpp>

#include <functional>

namespace nc = NumC;

int main()
{
    auto arr1 = ND_ARRAY<nc::float32>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    auto arr2 = ND_ARRAY<nc::float32>({{0, 1, 2}, {3, 4, 5}, {6, 7, 8}});
    auto arr3 = ND_ARRAY<nc::float32>({{{0, 1, 2}}, {{3, 4, 5}}, {{6, 7, 8}}});

    nc::shape_t shape = {5, 3, 3, 2};
    auto arr4 = ND_ARRAY<nc::float32>(shape);
    auto it = arr4.begin(), ite = arr4.end();

    for (int i = 0; it != ite; ++it, ++i)
        *it = i * 10;

    std::cout << "\n3D initialized array\n";
    nc::Utils::print_array(arr3);

    // Array slice.
    nc::slices_t slices1 = {nc::indices_t(2, 5), nc::indices_t(1, -1)};
    auto s1 = nc::Utils::slice(arr4, slices1);

    // Array reshape.
    nc::shape_t newShape1 = {-1, 9};
    auto r1 = nc::Utils::reshape(arr4, newShape1);

    // Array transpose.
    nc::size_t_v axes = {1, 0, 2, 3};
    auto t1 = nc::Utils::transpose(arr4, axes);

    auto op_arr1 = ND_ARRAY<nc::float32>({0, 1, 2, 3, 4});
    auto op_arr2 = ND_ARRAY<nc::int32>({5, 6, 7, 8, 9});
    auto op_arr3 = ND_ARRAY<nc::uint16>({10, 11, 12, 13, 14});
    auto op_arr4 = ND_ARRAY<nc::float64>({100, 100, 100, 100, 100});

    // Normal Array Operations.
    auto res1 = (op_arr1 + op_arr2 - op_arr3) * op_arr3 / op_arr4;

    auto arr5 = ND_ARRAY<nc::float32>({{{1, 2}}, {{3, 4}}, {{5, 6}}});
    auto arr6 = ND_ARRAY<nc::int32>({{1, 2} , {3, 4}, {5, 6}});
    auto arr7 = ND_ARRAY<nc::int32>({10, 20});

    // Broadcasted array operations - (2) - (3, 1, 2) + (3, 2).
    auto bd_arr = arr7 - (arr5 * arr6);
    std::cout << "\nBroadcasted array operation result\n";
    nc::Utils::print_array(bd_arr);

    return 0;
}