#include <stdio.h>

void main() {
    int a, b;
    scanf("%d, %d", &a, &b);

    if (!b) {
        b = 1;
    }

    printf("%d + %d = %d\n", a, b, (a+b));
}
