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
    assert(cUnit->last == NULL);

    cunit_terminate(&cUnit);
}

void dummy_test(void)
{
    return;
}

void test_cunit_terminate(void)
{
    cUnit_t *cUnit = NULL;

    struct test_st st, *st2;
    st2 = &st;

    cunit_init(&cUnit, &dummy_setup, &dummy_teardown, st2);
    char name[20] = "functions_name_name\0";
    cunit_add_test(cUnit, &dummy_test, name);

    Test_t *last = cUnit->last;
    for (int i = 0; i < 20; i++)
    {
        cunit_add_test(cUnit, &dummy_test, name);
        Test_t *test = cUnit->last;
        assert(test != NULL);
        assert(last->next == test);
        assert(strncmp(name, test->function_name, 19) == 0);
        assert(test->test == dummy_test);
        assert(test->next == NULL);
        last = test;
    }
    
    cunit_terminate(&cUnit);
    assert(cUnit == NULL);
}

void test_cunit_add_test(void)
{
    cUnit_t *cUnit = NULL;
    struct test_st st, *st2;
    st2 = &st;

    cunit_init(&cUnit, &dummy_setup, &dummy_teardown, st2);

    // Add first test
    char name[20] = "functions_name_name\0";
    cunit_add_test(cUnit, &dummy_test, name);
    assert(cUnit->head != NULL);
    assert(cUnit->last != NULL);
    assert(cUnit->last == cUnit->head);
    Test_t *test = cUnit->head;
    assert(test != NULL);
    assert(strncmp(name, test->function_name, 19) == 0);
    assert(test->test == dummy_test);
    assert(test->next == NULL);

    Test_t *last = cUnit->last;
    for (int i = 0; i < 20; i++)
    {
        cunit_add_test(cUnit, &dummy_test, name);
        test = cUnit->last;
        assert(test != NULL);
        assert(last->next == test);
        assert(strncmp(name, test->function_name, 19) == 0);
        assert(test->test == dummy_test);
        assert(test->next == NULL);
        last = test;
    }

    cunit_terminate(&cUnit);
    
}

void cunit_tests(void)
{
    printf("Testing the cUnit framework.\n");

    printf("Testing the cunit_init function.\n");
    test_cunit_init();
    printf("Test passed.\n");

    printf("Testing the cunit_terminate function.\n");
    test_cunit_terminate();
    printf("Test passed.\n");

    printf("Testing the cunit_add_test function.\n");
    test_cunit_add_test();
    printf("Test passed.\n");

    return;
}
