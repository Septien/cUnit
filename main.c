#include <stdio.h>
#include <stdlib.h>

#include "cUnit.h"
#include "cunit_tests.h"

int main(void)
{
    printf("cUnit framework.\n");
    cunit_tests();
    exit(0);
}