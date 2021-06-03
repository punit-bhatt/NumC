#include <NumC.hpp>

namespace nc = NumC;

int main()
{
    nc::shape_t shape = {5, 3, 3, 2};
    nc::slices_t slices =
        {
            nc::indices_t(2, -1),
            nc::indices_t(0, -1),
            nc::indices_t(1, -1)
        };

    nc::slices_t slices2 = {nc::indices_t(0, 3), nc::indices_t(1, -1)};

    auto arr1 = ND_ARRAY<nc::float32>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    auto arr2 = ND_ARRAY<nc::float32>({{0, 1, 2}, {3, 4, 5}, {6, 7, 8}});
    auto arr3 = ND_ARRAY<nc::float32>({{{0, 1, 2}}, {{3, 4, 5}}, {{6, 7, 8}}});

    auto arr4 = ND_ARRAY<nc::float32>(shape);
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

    auto s1 = nc::Utils::slice(arr4, slices);
    auto s2= nc::Utils::slice(s1, slices2);

    std::cout << "\nSliced array\n";
    nc::Utils::print_array(s1);

    auto it2 = s2.begin(), ite2 = s2.end();
    for (int i = 0; it2 != ite2; ++it2, ++i)
        *it2 *= 10;

    std::cout << "\nSliced SlicedView\n";
    nc::Utils::print_array(s2);

    std::cout << "\nShape based array post slice(slice) udpate\n";
    nc::Utils::print_array(arr4);

    return 0;
}