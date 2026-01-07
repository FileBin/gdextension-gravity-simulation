#include "macro.h"
#include <stdlib.h>
#ifndef LINKED_LIST_TYPE
#define LINKED_LIST_TYPE char
#endif

#define LINKED_LIST_NAME PPCAT(linked_list_, LINKED_LIST_TYPE)

struct LINKED_LIST_NAME;

typedef struct LINKED_LIST_NAME {
    struct LINKED_LIST_NAME *p_next;
    LINKED_LIST_TYPE data; 
} LINKED_LIST_NAME;

#define LINKED_LIST_NEW_NODE_FUNC PPCAT(LINKED_LIST_NAME, _new_node)

LINKED_LIST_NAME * PPCAT(LINKED_LIST_NAME, _new_node)(LINKED_LIST_TYPE data) {
  LINKED_LIST_NAME *new_node = malloc(sizeof(LINKED_LIST_NAME));
  new_node->data = data;
  new_node->p_next = NULL;
  
  return new_node;
}

void PPCAT(LINKED_LIST_NAME, _push_front)(LINKED_LIST_NAME **start_node,
                                    LINKED_LIST_TYPE data) {
  LINKED_LIST_NAME *new_node = LINKED_LIST_NEW_NODE_FUNC(data);

  new_node->p_next = (*start_node);
  (*start_node) = new_node;
}

LINKED_LIST_TYPE PPCAT(LINKED_LIST_NAME, _pop_front)(LINKED_LIST_NAME **start_node) {
  LINKED_LIST_TYPE value = (*start_node)->data;

  LINKED_LIST_NAME* first = (*start_node);
  (*start_node) = first->p_next;
  free(first);
  
  return value;
}

void PPCAT(LINKED_LIST_NAME, _destroy)(LINKED_LIST_NAME *start_node) {
  LINKED_LIST_NAME* iter = start_node;

  while (iter) {
    LINKED_LIST_NAME* next = iter->p_next;
    free(iter);
    iter = next;
  }
}
