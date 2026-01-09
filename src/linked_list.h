#include "macro.h"

#ifndef LINKED_LIST_TYPE
#define LINKED_LIST_TYPE char
#endif

#define LINKED_LIST_NAME PPCAT(linked_list_, LINKED_LIST_TYPE)

struct LINKED_LIST_NAME;


#ifndef LINKED_LIST_PTR_TYPE 
#define LINKED_LIST_PTR_TYPE void*
#endif

typedef struct LINKED_LIST_NAME {
    LINKED_LIST_PTR_TYPE p_next;
    LINKED_LIST_TYPE data; 
} LINKED_LIST_NAME;

#define LINKED_LIST_NEW_NODE_FUNC PPCAT(LINKED_LIST_NAME, _new_node)


#ifdef LINKED_LIST_USE_CUSTOM_MALLOC
#ifndef LINKED_LIST_MALLOC_ARGUMENT
#define LINKED_LIST_MALLOC_ARGUMENTS , void* (*malloc)(unsigned long size)
#endif

#ifndef LINKED_LIST_MALLOC_PASS
#define LINKED_LIST_MALLOC_PASS , malloc
#endif

#undef NULL
#define NULL ((LINKED_LIST_PTR_TYPE)0)
#else
#define LINKED_LIST_MALLOC_ARGUMENT
#define LINKED_LIST_MALLOC_PASS
#include <stdlib.h>
#endif

#ifndef LINKED_LIST_MALLOC_CALL
#define LINKED_LIST_MALLOC_CALL(size) malloc(size)
#endif

#ifndef LINKED_LIST_FREE_CALL
#define LINKED_LIST_FREE_CALL(ptr) free(ptr)
#endif

#ifndef LINKED_LIST_READ_PTR
#define LINKED_LIST_READ_PTR(ptr) (ptr)
#endif

LINKED_LIST_PTR_TYPE PPCAT(LINKED_LIST_NAME, _new_node)(LINKED_LIST_TYPE data LINKED_LIST_MALLOC_ARGUMENT) {
  LINKED_LIST_PTR_TYPE new_node = LINKED_LIST_MALLOC_CALL(sizeof(LINKED_LIST_NAME));
  ((LINKED_LIST_NAME *)LINKED_LIST_READ_PTR(new_node))->data = data;
  ((LINKED_LIST_NAME *)LINKED_LIST_READ_PTR(new_node))->p_next = NULL;
  
  return new_node;
}

void PPCAT(LINKED_LIST_NAME, _push_front)(LINKED_LIST_PTR_TYPE *start_node,
                                    LINKED_LIST_TYPE data LINKED_LIST_MALLOC_ARGUMENT) {
  LINKED_LIST_PTR_TYPE new_node = LINKED_LIST_NEW_NODE_FUNC(data LINKED_LIST_MALLOC_PASS);

  ((LINKED_LIST_NAME *)LINKED_LIST_READ_PTR(new_node))->p_next = (*start_node);
  (*start_node) = new_node;
}

LINKED_LIST_TYPE PPCAT(LINKED_LIST_NAME, _pop_front)(LINKED_LIST_PTR_TYPE *start_node LINKED_LIST_MALLOC_ARGUMENT) {
  LINKED_LIST_TYPE value = ((LINKED_LIST_NAME *)LINKED_LIST_READ_PTR(*start_node))->data;

  LINKED_LIST_PTR_TYPE first = (*start_node);
  (*start_node) = ((LINKED_LIST_NAME *)LINKED_LIST_READ_PTR(first))->p_next;
  LINKED_LIST_FREE_CALL(first);
  
  return value;
}

void PPCAT(LINKED_LIST_NAME, _destroy)(LINKED_LIST_PTR_TYPE start_node LINKED_LIST_MALLOC_ARGUMENT) {
  LINKED_LIST_PTR_TYPE iter = start_node;

  while (iter) {
    LINKED_LIST_PTR_TYPE next = ((LINKED_LIST_NAME *)LINKED_LIST_READ_PTR(iter))->p_next;
    LINKED_LIST_FREE_CALL(iter);
    iter = next;
  }
}

#define NULL ((void*)0)