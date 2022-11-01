#include "list.h"

int list_ctor (struct list_t* list)
{
    assert (list);

    list->size = 0;
    list->capacity = 10;
    list->sorted = 0;
    list->free = 1;
    list->code_error = 0;

    list->value = (elem_t*) calloc (list->capacity + 1, sizeof(elem_t));
    if (list->value == nullptr)
    {
        printf ("ERROR CALLOC on line %d in list.cpp", __LINE__);
        return ERROR_CALLOC;
    }
    list->next = (int*) calloc (list->capacity + 1, sizeof(int));
    if (list->next == nullptr)
    {
        printf ("ERROR CALLOC on line %d in list.cpp", __LINE__);
        return ERROR_CALLOC;
    }
    list->prev = (int*) calloc (list->capacity + 1, sizeof (int));
    if (list->prev == nullptr)
    {
        printf ("ERROR CALLOC on line %d in list.cpp", __LINE__);
        return ERROR_CALLOC;
    }

    for (int i = 1; i < list->capacity + 1; i++)
    {
        list->value[i] = POISON_VALUE;

        list->next[i] = (i != list->capacity) ? (i + 1) : -1;
        list->prev[i] = (i != 1) ? (i - 1) : -1;
    }

    ASSERT_OK (list);

    return GOOD_WORKING;
}

void list_dtor (struct list_t* list)
{
    ASSERT_OK (list);

    if (list->value != nullptr)
    {
        free (list->value);
        list->value = nullptr;
    }
    if (list->next != nullptr)
    {
        free (list->next);
        list->next = nullptr;
    }
    if (list->prev != nullptr)
    {
        free (list->prev);
        list->prev = nullptr;
    }

    list->capacity = SIZE_DTOR_VALUE;
    list->size     = SIZE_DTOR_VALUE;

    list->head = INT_DTOR_VALUE;
    list->tail = INT_DTOR_VALUE; 
    list->free = INT_DTOR_VALUE; 
}

//----------------------------------------------------------------------------

int list_insert_first (struct list_t* list, elem_t value)
{
    ASSERT_OK (list);

    if (list->size >= list->capacity)
    {
        list_realloc (list, list->capacity * 2);
    }

    int new_free = list->next[list->free];

    list->value[list->free] = value;

    list->prev[list->next[list->free]] = -1;

    list->next[list->free] = -1;
    list->prev[list->free] = -1;

    list->head = list->free;
    list->tail = list->free;

    list->free = new_free;

    list->size++;

    list_dump (list);
    ASSERT_OK (list);

    return GOOD_WORKING;
}

int list_insert_before (struct list_t* list, elem_t value, int index)
{
    ASSERT_OK (list);

    if (list->size >= list->capacity)
    {
        list_realloc (list, list->capacity * 2);
    }

    if (list->size == 0)
    {
        return list_insert_first (list, value);
    }
    if (index ==  0)
    {
        return list_insert_head (list, value);
    }

    list_dump (list);
    ASSERT_OK (list);

    return list_insert_after (list, value, index - 1);
}

int list_insert_after (struct list_t* list, elem_t value, int index)
{
    ASSERT_OK (list);

    if (list->size >= list->capacity)
    {
        list_realloc (list, list->capacity * 2);
    }

    if (list->size == 0)
    {
        return list_insert_first (list, value);
    }
    if (index == list->size - 1)
    {
        return list_insert_tail (list, value);
    }

    int physical_position = list_find_physical_position_logical_index (list, index);
    int new_free = list->next[list->free];

    list->value[list->free] = value;

    list->prev[list->free] = physical_position;
    list->prev[list->next[physical_position]] = list->free;
    list->next[list->free] = list->next[physical_position];
    list->next[physical_position] = list->free;

    list->prev[new_free] = -1;
    list->free = new_free;

    list->size++;

    list_dump (list);
    ASSERT_OK (list);

    return GOOD_WORKING;
}

int list_insert_head (struct list_t* list, elem_t value)
{
    ASSERT_OK (list);

    if (list->size >= list->capacity)
    {
        list_realloc (list, list->capacity * 2);
    }

    if (list->size == 0)
    {
        return list_insert_first (list, value);
    }

    int new_free = list->next[list->free];

    list->value[list->free] = value;

    list->prev[list->head] = list->free;
    list->next[list->free] = list->head;
    list->prev[list->free] = -1;

    list->head = list->free;
    list->free = new_free;

    list->prev[list->free] = -1;

    list->size++;

    list_dump (list);
    ASSERT_OK (list);

    return GOOD_WORKING;
}

int list_insert_tail (struct list_t* list, elem_t value)
{
    ASSERT_OK (list);

    if (list->size >= list->capacity)
    {
        list_realloc (list, list->capacity * 2);
    }

    if (list->size == 0)
    {
        return list_insert_first (list, value);
    }

    int new_free = list->next[list->free];

    list->value[list->free] = value;

    list->next[list->tail] = list->free;
    list->prev[list->free] = list->tail;
    list->next[list->free] = -1;

    list->tail = list->free;
    list->free = new_free;

    list->prev[list->free] = -1;

    list->size++;

    list_dump (list);
    ASSERT_OK (list);

    return GOOD_WORKING;
}

int list_delete (struct list_t* list, int index)
{
    ASSERT_OK (list);

    if (index == 0)
    {
        return list_delete_head (list);
    }
    else if (index == list->size - 1)
    {
        return list_delete_tail (list);
    }

    int physical_position = list_find_physical_position_logical_index (list, index);

    list->next[list->prev[physical_position]] = list->next[physical_position];
    list->prev[list->next[physical_position]] = list->prev[physical_position];

    list->prev[list->free] = physical_position;

    list->value[physical_position] = POISON_VALUE;
    list->next [physical_position] = list->free;
    list->prev [physical_position] = -1;

    list->free = physical_position;

    list->size--;

    list_dump (list);
    ASSERT_OK (list);

    return GOOD_WORKING;
}

int list_delete_head (struct list_t* list)
{
    ASSERT_OK (list);

    int new_head = list->next[list->head];
    int new_free = list->head;

    list->value[list->head] = POISON_VALUE;

    list->prev[list->next[list->head]] = -1;

    list->prev[list->free] = list->head;
    list->next[list->head] = list->free;
    list->prev[list->head] = -1;

    list->free = new_free;
    list->head = new_head;

    list->size--;

    list_dump (list);
    ASSERT_OK (list);

    return GOOD_WORKING;
}

int list_delete_tail (struct list_t* list)
{
    ASSERT_OK (list);

    int new_tail = list->prev[list->tail];
    int new_free = list->tail;

    list->value[list->tail] = POISON_VALUE;

    list->next[list->prev[list->tail]] = -1;

    list->prev[list->free] = list->tail;
    list->next[list->tail] = list->free;
    list->prev[list->tail] = -1;

    list->free = new_free;
    list->tail = new_tail;

    list->size--;

    list_dump (list);
    ASSERT_OK (list);

    return GOOD_WORKING;
}

//----------------------------------------------------------------------------

int list_find_physical_position_logical_index (struct list_t* list, int index)
{
    ASSERT_OK (list);

    if (list->sorted)
    {
        return index;
    }

    int physical_position = list->head;
    for (int i = 0; i < index; i++)
    {
        physical_position = list->next[physical_position];
    }

    ASSERT_OK (list);

    return physical_position;
}

int list_sorted (struct list_t* list)
{
    ASSERT_OK (list);

    elem_t* value_new = (elem_t*) calloc (list->capacity + 1, sizeof(elem_t));
    if (list->value == nullptr)
    {
        printf ("ERROR CALLOC on line %d in list.cpp", __LINE__);
        return ERROR_CALLOC;
    }
    int* next_new = (int*) calloc (list->capacity + 1, sizeof(int));
    if (list->next == nullptr)
    {
        printf ("ERROR CALLOC on line %d in list.cpp", __LINE__);
        return ERROR_CALLOC;
    }
    int* prev_new = (int*) calloc (list->capacity + 1, sizeof (int));
    if (list->prev == nullptr)
    {
        printf ("ERROR CALLOC on line %d in list.cpp", __LINE__);
        return ERROR_CALLOC;
    }

    for (int i = 1, current_position = list->head; i <= list->size; i++, current_position = list->next[current_position])
    {
        value_new[i] = list->value[current_position];
        next_new[i] = (i != list->size) ? (i + 1) : -1;
        prev_new[i] = (i != 1) ? (i - 1) : -1;
    }

    for (int i = list->size + 1; i < list->capacity + 1; i++)
    {
        value_new[i] = POISON_VALUE;

        next_new[i] = (i != list->capacity) ? (i + 1) : -1;
        prev_new[i] = (i != list->size + 1) ? (i - 1) : -1;
    }

    list->head = 1;
    list->tail = list->size;

    list->free = list->size + 1;

    elem_t* value_old = list->value;
    int* next_old = list->next;
    int* prev_old = list->prev;

    list->value = value_new;
    list->next = next_new;
    list->prev = prev_new;

    list->sorted = 1;

    free (value_old);
    free (next_old);
    free (prev_old);

    list_dump (list);
    ASSERT_OK (list);

    return GOOD_WORKING;
}

int list_realloc (struct list_t* list, size_t new_capacity)
{
    ASSERT_OK (list);

    elem_t* value_resize = (elem_t*) realloc (list->value, new_capacity * sizeof(elem_t));
    if (value_resize == nullptr)
    {
        printf ("ERROR REALLOC on line %d in list.cpp", __LINE__);
        return ERROR_REALLOC;
    }
    else
    {
        list->value = value_resize;
    }

    int* next_resize = (int*) realloc (list->next, new_capacity * sizeof(int));
    if (next_resize == nullptr)
    {
        printf ("ERROR REALLOC on line %d in list.cpp", __LINE__);
        return ERROR_REALLOC;
    }
    else
    {
        list->next = next_resize;
    }

    int* prev_resize = (int*) realloc (list->prev, new_capacity * sizeof(int));
    if (prev_resize == nullptr)
    {
        printf ("ERROR REALLOC on line %d in list.cpp", __LINE__);
        return ERROR_REALLOC;
    }
    else
    {
        list->prev = prev_resize;
    }

    if (new_capacity > list->capacity)
    {
        for (int i = list->capacity; i < new_capacity + 1; i++)
        {
            list->value[i] = POISON_VALUE;

            list->next[i] = (i != list->capacity) ? (i + 1) : -1;
            list->prev[i] = (i != 1) ? (i - 1) : -1;
        }
    }
    else
    {
        list->next[new_capacity] = -1;
    }

    list->capacity = new_capacity;

    ASSERT_OK (list);

    return GOOD_WORKING;
}

//----------------------------------------------------------------------------

int list_head_element (struct list_t* list)
{
    return list->head;
}

int list_tail_element (struct list_t* list)
{
    return list->tail;
}

elem_t list_value_element (struct list_t* list, int index)
{
    int physical_position = list_find_physical_position_logical_index (list, index);

    return list->value[physical_position];
}

size_t list_size (struct list_t* list)
{
    return list->size;
}

//----------------------------------------------------------------------------

int list_dump (struct list_t* list)
{
#ifndef NDEBUG
    FILE* list_log_file = fopen("list_log_file.txt", "ab");
    if (list_log_file == nullptr)
    {
        printf ("ERROR FOPEN on line %d in list.cpp", __LINE__);
        return ERROR_FILE_OPEN;
    }

    char* list_status = (char*) calloc (10, sizeof(char));

    if (list->code_error == 0)
    {
        strcpy (list_status, "OK");
    }
    else
    {
        decoder_list_error (list);
        strcpy (list_status, "ERROR");
    }

    fprintf (list_log_file, "List[%p] (%s)\n", list, list_status);
    fprintf (list_log_file, "{\n\t size       = %ld"
                             "\n\t capacity   = %ld"
                             "\n\t code_error = %ld"
                             "\n\t head       = %d"
                             "\n\t tail       = %d"
                             "\n\t free       = %d"
                             "\n\t {\n",
                             list->size, list->capacity, list->code_error, 
                             list->head, list->tail, list->free);
    fprintf (list_log_file, "\t\tvalue[%p]:\t", list->value);
    for (int i = 0; i < list->capacity + 1; i++)
    {
        fprintf (list_log_file, "%15lf", list->value[i]);
    }
    fprintf (list_log_file, "\n");
    fprintf (list_log_file, "\t\tnext[%p]:\t", list->next);
    for (int i = 0; i < list->capacity + 1; i++)
    {
        fprintf (list_log_file, "%15d", list->next[i]);
    }
    fprintf (list_log_file, "\n");
    fprintf (list_log_file, "\t\tprev[%p]:\t", list->prev);
    for (int i = 0; i < list->capacity + 1; i++)
    {
        fprintf (list_log_file, "%15d", list->prev[i]);
    }
    fprintf (list_log_file, "\n\t }");

    fprintf (list_log_file, "\n----------------------------------------------------------------------------\n");

    if (fclose (list_log_file))
    {
        printf ("ERROR FCLOSE on line %d in list.cpp", __LINE__);
        return ERROR_FILE_OPEN;
    }

    free (list_status);
#endif
    return GOOD_WORKING;
}

int list_error (struct list_t* list)
{
#ifndef NDEBUG
    int pointer_list_check_null = ((!list) ? LIST_ERROR_POINTER_STRUCT_NULL : 0);

    if (pointer_list_check_null == 0)
    {
        list->code_error |= CHECK_ERROR (!list->value,                LIST_ERROR_POINTER_BUFFER_VALUE_NULL);
        list->code_error |= CHECK_ERROR (!list->next,                 LIST_ERROR_POINTER_BUFFER_NEXT_NULL);
        list->code_error |= CHECK_ERROR (!list->prev,                 LIST_ERROR_POINTER_BUFFER_PREV_NULL);
        list->code_error |= CHECK_ERROR (list->size < 0,              LIST_ERROR_SIZE_SMALLER_ZERO);
        list->code_error |= CHECK_ERROR (list->capacity < 0,          LIST_ERROR_CAPACITY_SMALLER_ZERO);
        list->code_error |= CHECK_ERROR (list->size > list->capacity, LIST_ERROR_SIZE_BIGGER_CAPACITY);
        list->code_error |= CHECK_ERROR (list->head > list->capacity, LIST_ERROR_HEAD_BIGGER_CAPACITY);
        list->code_error |= CHECK_ERROR (list->tail > list->capacity, LIST_ERROR_TAIL_BIGGER_CAPACITY);
    }

    return list->code_error;
#else
    return GOOD_WORKING;
#endif
}

int decoder_list_error (struct list_t* list)
{
#ifndef NDEBUG
    FILE* list_log_file = fopen ("list_log_file.txt", "ab");
    if (list_log_file == nullptr)
    {
        printf ("ERROR FOPEN on line %d in list.cpp", __LINE__);
        return ERROR_FILE_OPEN;
    }

    PRINT_ERROR (list->code_error, LIST_ERROR_POINTER_BUFFER_VALUE_NULL);
    PRINT_ERROR (list->code_error, LIST_ERROR_POINTER_BUFFER_NEXT_NULL);
    PRINT_ERROR (list->code_error, LIST_ERROR_POINTER_BUFFER_PREV_NULL);
    PRINT_ERROR (list->code_error, LIST_ERROR_SIZE_SMALLER_ZERO);
    PRINT_ERROR (list->code_error, LIST_ERROR_CAPACITY_SMALLER_ZERO);
    PRINT_ERROR (list->code_error, LIST_ERROR_SIZE_BIGGER_CAPACITY);
    PRINT_ERROR (list->code_error, LIST_ERROR_HEAD_BIGGER_CAPACITY);
    PRINT_ERROR (list->code_error, LIST_ERROR_TAIL_BIGGER_CAPACITY);

    if (fclose (list_log_file) < 0)
    {
        printf ("ERROR FCLOSE on line %d in list.cpp", __LINE__);
        return ERROR_FILE_CLOSE;
    }
#endif

    return GOOD_WORKING;
}