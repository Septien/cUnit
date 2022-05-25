#include <stdlib.h>
#include <stdio.h>
#include "assert.h"

#include "cUnit.h"

#ifdef __RIOT__
#include "malloc.h"
#endif

void cunit_init(cUnit_t **cUnit, void (*setup)(void), void (*teardown)(void), void *data)
{
    assert(cUnit != NULL);
    assert(setup != NULL);
    assert(teardown != NULL);
    assert(data != NULL);

    // Allocate memory
    *cUnit = (cUnit_t *)malloc(sizeof(cUnit_t));

    // Store the setup and teardown functions.
    (*cUnit)->setup = setup;
    (*cUnit)->teardown = teardown;
    (*cUnit)->data = data;
    (*cUnit)->head = NULL;
    (*cUnit)->last = NULL;
}

void cunit_terminate(cUnit_t **cUnit)
{
    assert(cUnit != NULL);
    assert(*cUnit != NULL);
    
    (*cUnit)->setup = NULL;
    (*cUnit)->teardown = NULL;
    (*cUnit)->data = NULL;


    for (Test_t *head = (*cUnit)->head; head != NULL; )
    {
        Test_t *next = NULL;
        if (head != NULL)
        {
            next = head->next;
        }
        free(head);
        head = next;
    }

    free(*cUnit);
    *cUnit = NULL;
}

void cunit_add_test(cUnit_t *cUnit, void (*test)(void), char *function_name)
{
    assert(cUnit != NULL);
    assert(test != NULL);
    assert(function_name != NULL);

    Test_t *new_test = (Test_t *)malloc(sizeof(Test_t));
    if (new_test == NULL)
        return;
    
    strcpy(new_test->function_name, function_name);
    new_test->test = test;
    new_test->next = NULL;
    
    // No tests added
    if (cUnit->head == NULL)
    {
        cUnit->head = new_test;
        cUnit->last = new_test;
    }
    else        // Append at end of list
    {
        cUnit->last->next = new_test;
        cUnit->last = cUnit->last->next;
    }
}

void cunit_execute_tests(cUnit_t *cUnit)
{
    assert(cUnit != NULL);
    assert(cUnit->setup != NULL);
    assert(cUnit->teardown != NULL);
    assert(cUnit->data != NULL);
    assert(cUnit->head != NULL);

    for (Test_t *test = cUnit->head; test != NULL; test = test->next)
    {
        cUnit->setup();
        printf("Testing the %s function.\n", test->function_name);
        test->test();
        cUnit->teardown();
    }
}
