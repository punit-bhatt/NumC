#include <NumC.hpp>

namespace nc = NumC;

int main()
{
    nc::shape_t shape = {5, 3, 3, 2};

    auto arr1 = ND_ARRAY<nc::float32>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    auto arr2 = ND_ARRAY<nc::float32>({{0, 1, 2}, {3, 4, 5}, {6, 7, 8}});
    auto arr3 = ND_ARRAY<nc::float32>({{{0, 1, 2}}, {{3, 4, 5}}, {{6, 7, 8}}});

    auto arr4 = nc::ND_ARRAY<nc::float32>(shape);
    auto it = arr4.begin(), ite = arr4.end();

    for (int i = 0; it != ite; ++it, ++i)
        *it = i * 10;

    std::cout << "\n1D initialized array\n";
    nc::Utils::print_array(arr1);

    std::cout << "\n2D initialized array\n";
    nc::Utils::print_array(arr2);

    std::cout << "\n3D initialized array\n";
    nc::Utils::print_array(arr3);

    std::cout << "\nShape based array\n";
    nc::Utils::print_array(arr4);

    return 0;
}