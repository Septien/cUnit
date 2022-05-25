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
}

void cunit_terminate(cUnit_t **cUnit)
{
    assert(cUnit != NULL);
    assert(*cUnit != NULL);
    
    (*cUnit)->setup = NULL;
    (*cUnit)->teardown = NULL;
    (*cUnit)->data = NULL;

    free(*cUnit);
    *cUnit = NULL;
}
