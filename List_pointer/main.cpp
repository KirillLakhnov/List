#include "list_pointer.h"

int main ()
{
    struct list_pointer_t list_pointer = {};

    list_pointer_ctor (&list_pointer);

    list_pointer_insert_after (&list_pointer, 35, 1);
    list_pointer_insert_after (&list_pointer, 45, 1);
    list_pointer_insert_head (&list_pointer, 567);
    list_pointer_graph_dump (&list_pointer);

    list_pointer_insert_before (&list_pointer, 567, 2);
    list_pointer_insert_after (&list_pointer, 45, 2);
    list_pointer_graph_dump (&list_pointer);

    list_pointer_insert_head (&list_pointer, 45);
    list_pointer_delete (&list_pointer, 1);
    list_pointer_delete (&list_pointer, 2);
    list_pointer_delete (&list_pointer, 3);
    list_pointer_graph_dump (&list_pointer);

    list_pointer_dtor (&list_pointer);
    return 0;
}