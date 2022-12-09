#ifndef OCR_LINKED_LIST_H
#define OCR_LINKED_LIST_H

#include "image_split.h"

typedef struct list{
    struct list *next;
    struct Square data;
    int length;
} list;

void list_init(struct list *list);
void list_push_front(struct list *list, struct Square elm);
void free_list(struct list *list);

#endif