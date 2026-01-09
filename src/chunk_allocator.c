#include "chunk_allocator.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN_CHUNK_SIZE 2
#define CHUNK_CENTRAL_OFFSET 0x10000000

#define MAX(a, b) a > b ? a : b

uint32_t nextPowerOfTwo(uint32_t n) {
  if (n <= 1)
    return 1;
  if ((n & (n - 1)) == 0)
    return n; // Already a power of 2
  return 1u << (32 - __builtin_clz(n));
}

void init_chunk(chunk_allocator *chunk_allocator) {
  init_chunk_with_size(chunk_allocator, MIN_CHUNK_SIZE);
}

void init_chunk_with_size(chunk_allocator *chunk_allocator,
                          uint32_t initial_size) {
  chunk_allocator->used_bytes = 0;
  chunk_allocator->allocated_bytes = nextPowerOfTwo(MAX(initial_size, MIN_CHUNK_SIZE));
  chunk_allocator->memory = malloc(chunk_allocator->allocated_bytes);
}

bool reallocate_chunk_memory(chunk_allocator* chunk_allocator, uint32_t new_allocated_bytes) {
  void *temp = realloc(chunk_allocator->memory, new_allocated_bytes);
  if (temp) {
    chunk_allocator->memory = temp;
    chunk_allocator->allocated_bytes = new_allocated_bytes;
    return true;
  } else {
    // reallocaton failed (system out of memory)
    return false;
  }
}

offset_t allocate_memory_in_chunk(chunk_allocator *chunk_allocator,
                            uint32_t allocation_bytes) {
  uint32_t new_used_bytes = chunk_allocator->used_bytes + allocation_bytes;

  if (new_used_bytes > chunk_allocator->allocated_bytes) {
    if (!reallocate_chunk_memory(chunk_allocator, nextPowerOfTwo(new_used_bytes))){
      return 0;
    }
  }

  offset_t allocation_offset = chunk_allocator->used_bytes + CHUNK_CENTRAL_OFFSET;
  chunk_allocator->used_bytes = new_used_bytes;

  return allocation_offset;
}

void *ptr_from_chunk_offset(chunk_allocator *chunk_allocator, offset_t offset) {
  if (offset == 0) {
    return NULL;
  }
  return chunk_allocator->memory + offset - CHUNK_CENTRAL_OFFSET;
}

void free_all_allocations_in_chunk(chunk_allocator *chunk_allocator) {
  chunk_allocator->used_bytes = 0;
}

void free_chunk_memory(chunk_allocator *chunk_allocator){
  reallocate_chunk_memory(chunk_allocator, MIN_CHUNK_SIZE);
}
