#include "list_pointer.h"

int list_pointer_ctor (struct list_pointer_t* list_pointer)
{
    assert (list_pointer);

    list_pointer->size = 0;
    list_pointer->head = nullptr;
    list_pointer->tail = nullptr;

    list_pointer->code_error = 0;

    return GOOD_WORKING;
}

void list_pointer_dtor (struct list_pointer_t* list_pointer)
{
    ASSERT_OK (list_pointer);

    list_element* current_elem = list_pointer->head;
    list_element* next_elem = nullptr;

    for (int i = 0; i < list_pointer->size; i++)
    {
        next_elem = current_elem->next;

        current_elem->prev = nullptr;
        current_elem->next = nullptr;
        current_elem->value = POISON_VALUE;

        if (current_elem != nullptr)
        {
            free (current_elem);
            current_elem = nullptr;
        }

        current_elem = next_elem;
    }

    list_pointer->size = SIZE_DTOR_VALUE;

    list_pointer->head = nullptr;
    list_pointer->tail = nullptr;
}

//----------------------------------------------------------------------------

int list_pointer_insert_first (struct list_pointer_t* list_pointer, elem_t value)
{
    ASSERT_OK (list_pointer);

    list_element* new_elem = (list_element*) calloc (1, sizeof(list_element));
    if (new_elem == nullptr)
    {
        printf ("ERROR CALLOC on line %d in list_pointer.cpp", __LINE__);
        return ERROR_CALLOC;
    }

    new_elem->value = value;
    new_elem->prev = nullptr;
    new_elem->next = nullptr;

    list_pointer->size = 1;
    list_pointer->head = new_elem;
    list_pointer->tail = new_elem;

    ASSERT_OK (list_pointer);

    return GOOD_WORKING;
}

int list_pointer_insert_before (struct list_pointer_t* list_pointer, elem_t value, int index) //____________
{
    ASSERT_OK (list_pointer);

    if (list_pointer->size == 0)
    {
        return list_pointer_insert_first (list_pointer, value);
    }
    if (index ==  1)
    {
        return list_pointer_insert_head (list_pointer, value);
    }

    return list_pointer_insert_after (list_pointer, value, index - 1);
}

int list_pointer_insert_after (struct list_pointer_t* list_pointer, elem_t value, int index)
{
    ASSERT_OK (list_pointer);

    if (list_pointer->size == 0)
    {
        return list_pointer_insert_first(list_pointer, value);
    }
    if (index == list_pointer->size)
    {
        return list_pointer_insert_tail(list_pointer, value);
    }

    list_element* current_elem = list_pointer_find_element_by_index (list_pointer, index);
    list_element* new_elem = (list_element*) calloc (1, sizeof(list_element));
    if (new_elem == nullptr)
    {
        printf ("ERROR CALLOC on line %d in list_pointer.cpp", __LINE__);
        return ERROR_CALLOC;
    }

    new_elem->next = current_elem->next;
    new_elem->prev = current_elem;
    new_elem->value = value;

    current_elem->next->prev = new_elem;
    current_elem->next = new_elem;

    list_pointer->size++;

    ASSERT_OK (list_pointer);

    return GOOD_WORKING;
}

int list_pointer_insert_head (struct list_pointer_t* list_pointer, elem_t value)
{
    ASSERT_OK (list_pointer);
    
    if (list_pointer->size == 0)
    {
        return list_pointer_insert_first(list_pointer, value);
    }

    list_element* current_elem = list_pointer->head;
    list_element* new_elem = (list_element*) calloc (1, sizeof(list_element));
    if (new_elem == nullptr)
    {
        printf ("ERROR CALLOC on line %d in list_pointer.cpp", __LINE__);
        return ERROR_CALLOC;
    }

    new_elem->prev = nullptr;
    new_elem->next = current_elem;
    new_elem->value = value;

    current_elem->prev = new_elem;

    list_pointer->head = new_elem;

    list_pointer->size++;

    ASSERT_OK (list_pointer);

    return GOOD_WORKING;
}

int list_pointer_insert_tail (struct list_pointer_t* list_pointer, elem_t value)
{
    ASSERT_OK (list_pointer);

    if (list_pointer->size == 0)
    {
        return list_pointer_insert_first(list_pointer, value);
    }

    list_element* current_elem = list_pointer->tail;
    list_element* new_elem = (list_element*) calloc (1, sizeof(list_element));
    if (new_elem == nullptr)
    {
        printf ("ERROR CALLOC on line %d in list_pointer.cpp", __LINE__);
        return ERROR_CALLOC;
    }

    new_elem->prev  = current_elem;
    new_elem->next  = nullptr;
    new_elem->value = value;

    current_elem->next = new_elem;

    list_pointer->tail = new_elem;

    list_pointer->size++;

    ASSERT_OK (list_pointer);
    
    return GOOD_WORKING;
}

int list_pointer_delete (struct list_pointer_t* list_pointer, int index)
{
    ASSERT_OK (list_pointer);

    list_element* current_elem = list_pointer_find_element_by_index (list_pointer, index);

    if (list_pointer->size == 1)
    {
        list_pointer->head = nullptr;
        list_pointer->tail = nullptr;
    }
    else if (current_elem == list_pointer->head)
    {
        list_pointer->head = current_elem->next;
        current_elem->next->prev = nullptr;
    }
    else if (current_elem == list_pointer->tail)
    {
        list_pointer->tail = current_elem->prev;
        current_elem->prev->next = nullptr;
    }
    else
    {
        current_elem->prev->next = current_elem->next;
        current_elem->next->prev = current_elem->prev;
    }

    current_elem->next = nullptr;
    current_elem->prev = nullptr;
    current_elem->value = POISON_VALUE;

    free (current_elem);

    list_pointer->size--;

    ASSERT_OK (list_pointer);

    return GOOD_WORKING;
}

//----------------------------------------------------------------------------

list_element* list_pointer_find_element_by_index (struct list_pointer_t* list_pointer, int index)
{
    ASSERT_OK (list_pointer);

    list_element* current_elem = list_pointer->head;

    for (int i = 0; i < index; i++)
    {
        current_elem = current_elem->next;
    }

    return current_elem;
}

list_element* list_pointer_head_element (struct list_pointer_t* list_pointer)
{
    ASSERT_OK (list_pointer);

    return list_pointer->head;
}

list_element* list_pointer_tail_element (struct list_pointer_t* list_pointer)
{
    ASSERT_OK (list_pointer);

    return list_pointer->tail;
}

list_element* list_pointer_next_element(struct list_pointer_t* list_pointer, struct list_element* current_elem)
{
    ASSERT_OK (list_pointer);
    assert (current_elem);

    return current_elem->next;
}

list_element* list_pointer_prev_element(struct list_pointer_t* list_pointer, struct list_element* current_elem)
{
    ASSERT_OK (list_pointer);
    assert (current_elem);

    return current_elem->prev;
}

elem_t list_pointer_value_element (struct list_pointer_t* list_pointer, int index) //___________________
{
    ASSERT_OK (list_pointer);

    list_element* current_elem = list_pointer_find_element_by_index (list_pointer, index);

    elem_t value = current_elem->value;

    return value;
}

size_t list_pointer_size (struct list_pointer_t* list_pointer)
{
    return list_pointer->size;
}

//----------------------------------------------------------------------------

int list_pointer_graph_dump (struct list_pointer_t* list_pointer)
{
    static int number_of_function_launches = 0;

    FILE* list_log_graph = fopen ("List_pointer/graph_log.dot", "w");
    if (list_log_graph == nullptr)
    {
        printf ("ERROR FOPEN on line %d in list.cpp", __LINE__);
        return ERROR_FILE_CLOSE;
    }

    list_element* current_elem = list_pointer->head;

    fprintf (list_log_graph, "digraph G\n{\n");
    fprintf (list_log_graph, "\tgraph [dpi = 300];\n");
    fprintf (list_log_graph, "\trankdir = LR;\n");

    for (int i = 0; i < list_pointer->size; i++)
    {   
        if (isnan(current_elem->value))
        {
            fprintf (list_log_graph,    "\t\"Nod_0\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\", fillcolor = \"#F4A460\","
                                        "\n\t\t   label = \"index = NULL_ELEMENT [%p]| {<prev> prev = %p | value = %lf | <next> next = %p}\"]\n",
                                        current_elem, current_elem->prev, current_elem->value, current_elem->next);
        }
        else
        {
            fprintf (list_log_graph,    "\t\"Nod_%d\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\", fillcolor = \"#87CEFA\","
                                        "\n\t\t   label = \"index = %d [%p]| {<prev> prev = %p| value = %lf | <next> next = %p}\"]\n", 
                                        i, i, current_elem, current_elem->prev, current_elem->value, current_elem->next);
        }
        current_elem = current_elem->next;
    }

    fprintf (list_log_graph,    "\t\"LIST_INFO\" [shape = \"record\", style = \"rounded, filled\", fontname = \"Helvetica-Bold\"\n\t\t\t\t "
				                "label = \"SIZE = %lu\\lCODE ERROR = %lu\"]\n",
                                list_pointer->size, list_pointer->code_error);
    fprintf(list_log_graph, "\t\"LIST DUMP for List[%p]\"[color = \"white\"]\t", list_pointer);

    if (list_pointer->size == 1)
    {
        fprintf (list_log_graph, "\n}");
        return GOOD_WORKING;
    }
    else
    {
        for (int i = 0; i < list_pointer->size - 1; i++)
        {
            fprintf (list_log_graph, "\t\"Nod_%d\"->\"Nod_%d\"[style = \"invis\", weight=1000]\n", i, i + 1);
        }

        current_elem = list_pointer->head;
        for (int i = 0; i < list_pointer->size - 1; i++)
        {
            fprintf (list_log_graph, "\t\"Nod_%d\":next -> \"Nod_%d\":prev[style = \"bold\", color = \"#df1b1bdf\"]\n", i, i+1);
            current_elem = current_elem->next;
        }

    }
    fprintf (list_log_graph, "\n}");

    if (fclose (list_log_graph))
    {
        printf ("ERROR FCLOSE on line %d in list.cpp", __LINE__);
        return ERROR_FILE_CLOSE;
    }

    char command[100] = "";
    sprintf (command, "dot -Tpng -oList_pointer/graph_log_list_%d.png List_pointer/graph_log.dot", number_of_function_launches);

    system(command);

    FILE* htm_log_file = fopen ("List_pointer/htm_log_file_pointer.htm", "a");

    fprintf (htm_log_file, "<pre>\n");
    fprintf (htm_log_file, "<img src = \"graph_log_list_%d.png\">", number_of_function_launches);
    fprintf (htm_log_file, "<hr>\n");

    fclose(htm_log_file);

    number_of_function_launches++;

    return GOOD_WORKING;
}

int list_pointer_error (struct list_pointer_t* list_pointer)
{
#ifndef NDEBUG
    int pointer_list_check_null = ((!list_pointer) ? LIST_ERROR_POINTER_STRUCT_NULL : 0);

    if (pointer_list_check_null == 0)
    {
        list_pointer->code_error |= CHECK_ERROR (!list_pointer->head && (list_pointer->size > 1), LIST_ERROR_HEAD_NULL);
        list_pointer->code_error |= CHECK_ERROR (!list_pointer->tail && (list_pointer->size > 1), LIST_ERROR_TAIL_NULL);
        list_pointer->code_error |= CHECK_ERROR (list_pointer->size < 0,                          LIST_ERROR_SIZE_SMALLER_ZERO);
    }

    return list_pointer->code_error;
#else
    return GOOD_WORKING;
#endif
}

int isnan (double number)
{
    if (number != number)
    {
        return 1;
    }
    return 0;
}
