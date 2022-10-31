#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "enum.h"

typedef double elem_t;

const elem_t POISON_VALUE = NAN;
const size_t SIZE_DTOR_VALUE = 0xBABADEDA;
const int INT_DTOR_VALUE = 0xADAFDADA;

struct list_t
{
    elem_t* value;

    int* next;
    int* prev;

    size_t size;
    size_t capacity;

    int head;
    int tail;
    int free;

    size_t code_error;

    int sorted;
};

//----------------------------------------------------------------------------

int list_ctor (struct list_t* list, size_t capacity);

void list_dtor (struct list_t* list);

//----------------------------------------------------------------------------

int list_insert_first (struct list_t* list, elem_t value);

int list_insert_before (struct list_t* list, elem_t value, int index);

int list_insert_after (struct list_t* list, elem_t value, int index);

int list_insert_head (struct list_t* list, elem_t value);

int list_insert_tail (struct list_t* list, elem_t value);

int list_delete (struct list_t* list, int index);

int list_delete_head (struct list_t* list);

int list_delete_tail (struct list_t* list);

//----------------------------------------------------------------------------

int list_find_physical_position_logical_index (struct list_t* list, int index);

int list_sorted (struct list_t* list);

int list_recalloc (struct list_t* list);

//----------------------------------------------------------------------------

int list_head_element (struct list_t* list);

int list_tail_element (struct list_t* list);

int list_next_element (struct list_t* list, int index);

int list_prev_element (struct list_t* list, int index);

elem_t list_value_element (struct list_t* list, int index);

size_t list_size (struct list_t* list);

//----------------------------------------------------------------------------

int list_is_empty (struct list_t* list);

//----------------------------------------------------------------------------

void list_dump (struct list_t* list);

#endif // LIST_H