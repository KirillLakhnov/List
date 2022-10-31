#include "list.h"

int list_ctor (struct list_t* list, size_t capacity)
{
    list->size = 0;
    list->capacity = capacity;
    list->sorted = 0;
    list->free = 1;
    list->code_error = 0;

    list->value = (elem_t*) calloc (capacity + 1, sizeof(elem_t));
    if (list->value == nullptr)
    {
        printf ("ERROR CALLOC on line %d in list.cpp", __LINE__);
        return ERROR_CALLOC;
    }
    list->next = (int*) calloc (capacity + 1, sizeof(int));
    if (list->next == nullptr)
    {
        printf ("ERROR CALLOC on line %d in list.cpp", __LINE__);
        return ERROR_CALLOC;
    }
    list->prev = (int*) calloc (capacity + 1, sizeof (int));
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

    return GOOD_WORKING;
}

void list_dtor (struct list_t* list)
{
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

    list->size = SIZE_DTOR_VALUE;
    list->capacity = SIZE_DTOR_VALUE;

    list->head = INT_DTOR_VALUE;
    list->tail = INT_DTOR_VALUE; 
    list->free = INT_DTOR_VALUE; 
}

//----------------------------------------------------------------------------

int list_insert_first (struct list_t* list, elem_t value)
{
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

    return GOOD_WORKING;
}

int list_insert_before (struct list_t* list, elem_t value, int index)
{
    if (list->size == 0)
    {
        return list_insert_first (list, value);
    }
    if (index ==  0)
    {
        return list_insert_head (list, value);
    }

    list_dump (list);

    return list_insert_after (list, value, index - 1);
}

int list_insert_after (struct list_t* list, elem_t value, int index)
{
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

    return GOOD_WORKING;
}

int list_insert_head (struct list_t* list, elem_t value)
{
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

    return GOOD_WORKING;
}

int list_insert_tail (struct list_t* list, elem_t value)
{
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

    return GOOD_WORKING;
}

int list_delete (struct list_t* list, int index)
{
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

    return GOOD_WORKING;
}

int list_delete_head (struct list_t* list)
{
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

    return GOOD_WORKING;
}

int list_delete_tail (struct list_t* list)
{
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

    return GOOD_WORKING;
}

//----------------------------------------------------------------------------

int list_find_physical_position_logical_index (struct list_t* list, int index)
{
    if (list->sorted)
    {
        return index;
    }

    int physical_position = list->head;
    for (int i = 0; i < index; i++)
    {
        physical_position = list->next[physical_position];
    }

    return physical_position;
}

int list_sorted (struct list_t* list)
{
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

    return GOOD_WORKING;
}

int list_recalloc (struct list_t* list)
{
    
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

int list_next_element (struct list_t* list, int index)
{
    
}

int list_prev_element (struct list_t* list, int index)
{

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

int list_is_empty (struct list_t* list)
{

}
//----------------------------------------------------------------------------

void list_dump (struct list_t* list)
{
    FILE* list_log_file = fopen("list_log_file.txt", "ab");

    char* list_status = (char*) calloc (10, sizeof(char));

    if (list->code_error == 0)
    {
        strcpy (list_status, "OK");
    }
    else
    {
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
                            list->size, list->capacity, list->code_error, list->head, list->tail, list->free);
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

    fclose (list_log_file);
    free (list_status);
}
