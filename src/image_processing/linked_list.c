#include "image_processing/linked_list.h"
#include <stdlib.h>

void list_init(struct list *list)
{
    list->next = NULL;
    list->length = 0;
}

void list_push_front(struct list *list, struct Square elm)
{
    struct list *newList = malloc(sizeof(struct list));
    newList->data = elm;
    newList->next = list->next;
    list->next = newList;
    list->length++;
}

void free_list(struct list *list)
{
    struct list *tmp;
    while (list != NULL)
    {
        tmp = list;
        list = list->next;
        free(tmp);
    }
}

