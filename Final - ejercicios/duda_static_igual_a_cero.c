#include <stdio.h>

void a () {
    static int acum = 1;
    printf("%i", acum++);
}


int main () {
    for (int i = 3; i < 9; i++)
        a();

    return 0;
}