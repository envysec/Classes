#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int year = atoi(argv[1]);
    if (!(year % 4)) {
        if (!(year % 100)) {
            if (!(year % 400))
                printf("%d was a leap year\n", year);
            else
                printf("%d was not a leap year\n", year);
        } else {
            printf("%d was a leap year\n", year);
        }
    } else {
        printf("%d was not a leap year\n", year);
    }
    return 0;
}
