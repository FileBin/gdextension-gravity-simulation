#include "macro.h"
#include <stdlib.h>
#ifndef LINKED_LIST_TYPE
#define LINKED_LIST_TYPE char
#endif

#define LINKED_LIST_NAME PPCAT(linked_list_, LINKED_LIST_TYPE)

struct LINKED_LIST_NAME;

typedef struct LINKED_LIST_NAME {
    struct LINKED_LIST_NAME *p_next, *p_prev;
    LINKED_LIST_TYPE data; 
} LINKED_LIST_NAME;

#define LINKED_LIST_NEW_NODE_FUNC PPCAT(LINKED_LIST_NAME, _new_node)

LINKED_LIST_NAME * PPCAT(LINKED_LIST_NAME, _new_node)(LINKED_LIST_TYPE data) {
  LINKED_LIST_NAME *new_node = malloc(sizeof(LINKED_LIST_NAME));
  new_node->data = data;
  new_node->p_next = NULL;
  new_node->p_prev = NULL;
  
  return new_node;
}

void PPCAT(LINKED_LIST_NAME, _push_front)(LINKED_LIST_NAME **start_node,
                                    LINKED_LIST_TYPE data) {
  LINKED_LIST_NAME *new_node = LINKED_LIST_NEW_NODE_FUNC(data);

  if ((*start_node) == 0) {
    (*start_node) = new_node;
  } else {
    (*start_node)->p_prev = new_node;
    new_node->p_next = (*start_node);
  }
}

LINKED_LIST_TYPE PPCAT(LINKED_LIST_NAME, _pop_front)(LINKED_LIST_NAME **start_node) {
  LINKED_LIST_TYPE value = (*start_node)->data;

  LINKED_LIST_NAME* prev = (*start_node);
  (*start_node) = prev->p_next;
  free(prev);
  
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
