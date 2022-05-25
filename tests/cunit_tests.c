#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "cUnit.h"
#include "cunit_tests.h"

#ifdef __RIOT__
#include "malloc.h"
#endif

void dummy_setup(void)
{
    return;
}

void dummy_teardown(void)
{
    return;
}

struct test_st
{
    int a;
    int b;
};


void test_cunit_init(void)
{
    cUnit_t *cUnit = NULL;

    struct test_st st, *st2;
    st2 = &st;

    cunit_init(&cUnit, &dummy_setup, &dummy_teardown, st2);
    assert(cUnit != NULL);
    assert(cUnit->setup == dummy_setup);
    assert(cUnit->teardown == dummy_teardown);
    assert(cUnit->data == st2);
    assert(cUnit->head == NULL);

    cUnit->setup = NULL;
    cUnit->teardown = NULL;
    cUnit->data = NULL;
    free(cUnit);
}



void cunit_tests(void)
{
    printf("Testing the cUnit framework.\n");

    printf("Testing the cunit_init function.\n");
    test_cunit_init();
    printf("Test passed.\n");

    return;
}
