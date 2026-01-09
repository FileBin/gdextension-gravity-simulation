#ifndef FFC0557D_012E_4C61_A6C8_1EA8674F5C5D
#define FFC0557D_012E_4C61_A6C8_1EA8674F5C5D

#include <stddef.h>
#include <stdint.h>

typedef int32_t offset_t;

typedef struct {
  void *memory;
  uint32_t allocated_bytes, used_bytes;
} chunk_allocator;

void init_chunk(chunk_allocator *chunk_allocator);
void init_chunk_with_size(chunk_allocator *chunk_allocator,
                          uint32_t initial_size);
offset_t allocate_memory_in_chunk(chunk_allocator *chunk_allocator,
                            uint32_t allocation_bytes);

void* ptr_from_chunk_offset(chunk_allocator *chunk_allocator,
                            offset_t offset);
void free_all_allocations_in_chunk(chunk_allocator *chunk_allocator);
void free_chunk_memory(chunk_allocator *chunk_allocator);

#endif /* FFC0557D_012E_4C61_A6C8_1EA8674F5C5D */
