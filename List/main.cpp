#include "list.h"

int main ()
{
    struct list_t list = {};

    if (list_ctor (&list) != GOOD_WORKING)
    {
        return -1;
    }

    list_dump (&list);

    list_insert_first (&list, 28.98);
    list_insert_tail (&list, 34.89);
    list_insert_before (&list, 111, 1);
    list_insert_tail (&list, 17.867);
    list_insert_before (&list, 121, 3);
    list_delete_head (&list);
    list_delete (&list, 1);
    list_sorted (&list);

    list_graph_dump (&list);

    list_dtor (&list);

    return 0;
}