#include <iostream>

void b (int& a) {
    a += 1;
}

int main () {
    int a = 10;
    b(a);
    std::cout << a << std::endl;
    return 0;
}