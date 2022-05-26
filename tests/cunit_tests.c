#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "cUnit.h"
#include "cunit_tests.h"

#ifdef __RIOT__
#include "malloc.h"
#endif

/* Encapsulate the data. */
struct data {
    cUnit_t *cUnit_test;
};

/* Necessary data and functions for testing the framework. */
struct test_st
{
    int a;
    int b;
};

void dummy_setup(void *arg)
{
    (void) arg;
    return;
}

void dummy_teardown(void *arg)
{
    (void) arg;
    return;
}

/* Setup and Teardown functions. */
void setup(void *arg)
{
    struct data *cunit_data = (struct data *)arg;
    struct test_st st, *st2;
    st2 = &st;

    cunit_init(&cunit_data->cUnit_test, &dummy_setup, &dummy_teardown, st2);
}

void teardown(void *arg)
{
    struct data *cunit_data = (struct data *)arg;

    cunit_terminate(&cunit_data->cUnit_test);
}

/* Test functions. */
void test_cunit_init(void *arg)
{
    struct data *cunit_data = (struct data *)arg;
    cUnit_t *cUnit = cunit_data->cUnit_test;

    assert(cUnit != NULL);
    assert(cUnit->setup == dummy_setup);
    assert(cUnit->teardown == dummy_teardown);
    assert(cUnit->data != NULL);
    assert(cUnit->head == NULL);
    assert(cUnit->last == NULL);
}

void dummy_test(void *arg)
{
    (void) arg;
    printf("Dummy test.\n");
    return;
}

void test_cunit_terminate(void *arg)
{
    struct data *cunit_data = (struct data *)arg;
    cUnit_t *cUnit = cunit_data->cUnit_test;

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

    struct test_st st, *st2;
    st2 = &st;
    cunit_init(&cUnit, &dummy_setup, &dummy_teardown, (void *)&st2);
}

void test_cunit_add_test(void *arg)
{
    struct data *cunit_data = (struct data *)arg;
    cUnit_t *cUnit = cunit_data->cUnit_test;

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
}

void test_cunit_execute_tests(void *arg)
{
    struct data *cunit_data = (struct data *)arg;
    cUnit_t *cUnit = cunit_data->cUnit_test;

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

    cunit_execute_tests(cUnit);
}

void cunit_tests(void)
{
    cUnit_t *cUnit;
    struct data cunit_data;

    cunit_init(&cUnit, setup, teardown, (void *)&cunit_data);

    cunit_add_test(cUnit, test_cunit_init, "test_cunit_init\0");
    cunit_add_test(cUnit, test_cunit_terminate, "test_cunit_terminate\0");
    cunit_add_test(cUnit, test_cunit_add_test, "test_cunit_add_test\0");
    cunit_add_test(cUnit, test_cunit_execute_tests, "test_cunit_execute_tests\0");

    printf("Testing the cUnit framework.\n");
    cunit_execute_tests(cUnit);

    cunit_terminate(&cUnit);
    return;
}
