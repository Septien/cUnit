#include <stdio.h>
#include <stdlib.h>

#include "cUnit.h"
#include "cunit_tests.h"

int main(void)
{
#ifdef __LINUX__
    printf("Testing the cUnit framework.\n");
    printf("cUnit framework.\n");
    cunit_tests();
#endif
#ifdef __RIOT__
    puts("\nWelcome to RIOT!.\n");
    do {
        printf("Testing the cUnit framework.\n");
        printf("cUnit framework.\n");
        cunit_tests();
    }while (0);
#endif
    exit(0);
}
