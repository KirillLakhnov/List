#ifndef LIST_POINTER_H
#define LIST_POINTER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "enum.h"

typedef double elem_t;

#define specifier_elem_t "%lf"

const elem_t POISON_VALUE = NAN;
const size_t SIZE_DTOR_VALUE = 0xBABADEDA;
const int INT_DTOR_VALUE = 0xADAFDADA;

struct list_element 
{
    elem_t value;

    list_element* next;
    list_element* prev;
};

struct list_pointer_t
{
    struct list_element* head;
    struct list_element* tail;

    size_t size;
};

int list_pointer_ctor (struct list_pointer_t* list_pointer);

void list_pointer_dtor (struct list_pointer_t* list_pointer);

//----------------------------------------------------------------------------

int list_pointer_insert_first (struct list_pointer_t* list_pointer, elem_t value);

int list_pointer_insert_before (struct list_pointer_t* list_pointer, elem_t value, int index);

int list_pointer_insert_after (struct list_pointer_t* list_pointer, elem_t value, int index);

int list_pointer_insert_head (struct list_pointer_t* list_pointer, elem_t value);

int list_pointer_insert_tail (struct list_pointer_t* list_pointer, elem_t value);

int list_pointer_delete (struct list_pointer_t* list_pointer, int index);

//----------------------------------------------------------------------------

list_element* list_pointer_find_element_by_index (struct list_pointer_t* list_pointer, int index);

list_element* list_pointer_head_element (struct list_pointer_t* list_pointer);

list_element* list_pointer_tail_element (struct list_pointer_t* list_pointer);

list_element* list_pointer_next_element(struct list_pointer_t* list_pointer, struct list_element* current_elem);

list_element* list_pointer_prev_element(struct list_pointer_t* list_pointer, struct list_element* current_elem);

elem_t list_pointer_value_element (struct list_pointer_t* list_pointer, int index);

size_t list_pointer_size (struct list_pointer_t* list_pointer);

//----------------------------------------------------------------------------

int list_pointer_graph_dump (struct list_pointer_t* list_pointer);

int isnan (double number); 

#endif // LIST_POINTER_H