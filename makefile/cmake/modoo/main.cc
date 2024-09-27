#include <iostream>

#include "foo.h"
#include "shape.h"

int main()
{
    class Rectangle rect(3,4);

    std::cout << "Hello, CMake" << std::endl;
    std::cout << "Foo : " << foo() << std::endl;
    std::cout << "rect : " << rect.GetSize() << std::endl;

    return 0;
}
