#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
bool test_cunit_init(void *arg)
{
    struct data *cunit_data = (struct data *)arg;
    cUnit_t *cUnit = cunit_data->cUnit_test;

    bool passed = cUnit != NULL;
    passed = passed && (cUnit->setup == dummy_setup);
    passed = passed && (cUnit->teardown == dummy_teardown);
    passed = passed && (cUnit->data != NULL);
    passed = passed && (cUnit->head == NULL);
    passed = passed && (cUnit->last == NULL);
    passed = passed && (cUnit->tests_passed == 0);
    passed = passed && (cUnit->tests_failed == 0);

    return passed;
}

bool dummy_test(void *arg)
{
    (void) arg;
    printf("Dummy test.\n");
    return true;
}

bool test_cunit_terminate(void *arg)
{
    struct data *cunit_data = (struct data *)arg;
    cUnit_t *cUnit = cunit_data->cUnit_test;

    char name[20] = "functions_name_name\0";
    cunit_add_test(cUnit, &dummy_test, name);

    Test_t *last = cUnit->last;
    bool passed = true;
    for (int i = 0; i < 20; i++)
    {
        cunit_add_test(cUnit, &dummy_test, name);
        Test_t *test = cUnit->last;
        passed = passed && (test != NULL);
        passed = passed && (last->next == test);
        passed = passed && (strncmp(name, test->function_name, 19) == 0);
        passed = passed && (test->test == dummy_test);
        passed = passed && (test->next == NULL);
        last = test;
    }
    
    cunit_terminate(&cUnit);
    passed = passed && (cUnit == NULL);

    struct test_st st, *st2;
    st2 = &st;
    cunit_init(&cUnit, &dummy_setup, &dummy_teardown, (void *)&st2);

    return passed;
}

bool test_cunit_add_test(void *arg)
{
    struct data *cunit_data = (struct data *)arg;
    cUnit_t *cUnit = cunit_data->cUnit_test;

    // Add first test
    char name[20] = "functions_name_name\0";
    cunit_add_test(cUnit, &dummy_test, name);
    bool passed = cUnit->head != NULL;
    passed = passed && (cUnit->last != NULL);
    passed = passed && (cUnit->last == cUnit->head);
    Test_t *test = cUnit->head;
    passed = passed && (test != NULL);
    passed = passed && (strncmp(name, test->function_name, 19) == 0);
    passed = passed && (test->test == dummy_test);
    passed = passed && (test->next == NULL);
    if (!passed) {
        printf("Adding first test failed.\n");
        return false;
    }

    passed = true;
    Test_t *last = cUnit->last;
    for (int i = 0; i < 20; i++)
    {
        cunit_add_test(cUnit, &dummy_test, name);
        test = cUnit->last;
        passed = passed && (test != NULL);
        passed = passed && (last->next == test);
        passed = passed && (strncmp(name, test->function_name, 19) == 0);
        passed = passed && (test->test == dummy_test);
        passed = passed && (test->next == NULL);
        last = test;
    }

    return passed;
}

bool dummy_test_failed(void *arg)
{
    (void) arg;
    printf("Dummy test failed.\n");
    return false;
}

bool test_cunit_execute_tests(void *arg)
{
    struct data *cunit_data = (struct data *)arg;
    cUnit_t *cUnit = cunit_data->cUnit_test;

    char name[20] = "functions_name_name\0";
    cunit_add_test(cUnit, &dummy_test, name);
    bool passed = cUnit->head != NULL;
    passed = passed && (cUnit->last != NULL);
    passed = passed && (cUnit->last == cUnit->head);
    Test_t *test = cUnit->head;
    passed = passed && (test != NULL);
    passed = passed && (strncmp(name, test->function_name, 19) == 0);
    passed = passed && (test->test == dummy_test);
    passed = passed && (test->next == NULL);
    if (!passed) {
        printf("First test not passed.\n");
        return false;
    }

    passed = true;
    Test_t *last = cUnit->last;
    for (int i = 0; i < 20; i++)
    {
        cunit_add_test(cUnit, &dummy_test, name);
        test = cUnit->last;
        passed = passed && (test != NULL);
        passed = passed && (last->next == test);
        passed = passed && (strncmp(name, test->function_name, 19) == 0);
        passed = passed && (test->test == dummy_test);
        passed = passed && (test->next == NULL);
        last = test;
    }
    if (!passed) {
        printf("Adding succesful tests failed.\n");
        return false;
    }

    passed = true;
    for (int i = 0; i < 20; i++)
    {
        cunit_add_test(cUnit, &dummy_test_failed, name);
        test = cUnit->last;
        passed = passed && (test != NULL);
        passed = passed && (last->next == test);
        passed = passed && (strncmp(name, test->function_name, 19) == 0);
        passed = passed && (test->test == dummy_test_failed);
        passed = passed && (test->next == NULL);
        last = test;
    }
    if (!passed) {
        printf("Adding failing tests failed.\n");
        return false;
    }

    passed = true;
    cunit_execute_tests(cUnit);
    passed = passed && (cUnit->tests_passed == 21);
    passed = passed && (cUnit->tests_failed == 20);
    uint total = 41;
    uint actual = cUnit->tests_passed + cUnit->tests_failed;
    passed = passed && (total == actual);

    return passed;
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
