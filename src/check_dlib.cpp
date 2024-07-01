#include <iostream>
#include <dlib/matrix.h>

int main() {
    dlib::matrix<double> m(3, 3);
    m = 1, 2, 3,
        4, 5, 6,
        7, 8, 9;
    std::cout << "Matrix:\n" << m << std::endl;
    return 0;
}
