#include <_inttypes.h>
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define HEAP_CAPACITY 640000
#define CHUNK_LIST_CAPACITY 1024

typedef struct {
  char *start;
  size_t size;
} Chunk;

typedef struct {
  Chunk chunks[CHUNK_LIST_CAPACITY];
  size_t count;
} Chunk_List;

char heap[HEAP_CAPACITY] = {0};

Chunk_List allocated_chunks = {0};
Chunk_List freed_chunks = {
    .count = 1, .chunks = {[0] = {.start = heap, .size = sizeof(heap)}}};
Chunk_List tmp_chunks = {0};

void chunk_list_dump(const Chunk_List *list) {
  printf("Chunks (%zu): \n", list->count);
  for (size_t i = 0; i < list->count; ++i) {
    printf(" start: %p, size: %zu\n", list->chunks[i].start,
           list->chunks[i].size);
  }
}

int chunk_start_compare(const void *a, const void *b) {
  const Chunk *a_chunk = a;
  const Chunk *b_chunk = b;

  return a_chunk->start - b_chunk->start;
}

int chunk_list_find(const Chunk_List *list, void *ptr) {

  for (size_t i = 0; i < list->count; ++i) {
    if (list->chunks[i].start == ptr) {
      return (int)i;
    }
  }

  return -1;

  // Chunk key = {.start = ptr};
  //
  // Chunk *result = bsearch(&key, list->chunks, list->count,
  //                         sizeof(list->chunks[0]), chunk_start_compare);
  // if (result != 0) {
  //   assert(list->chunks <= result);
  //   return (result - list->chunks) / sizeof(list->chunks[0]);
  // } else {
  //   return -1;
  // }
}

void chunk_list_insert(Chunk_List *list, void *start, size_t size) {
  assert(list->count < CHUNK_LIST_CAPACITY);
  list->chunks[list->count].start = start;
  list->chunks[list->count].size = size;

  for (size_t i = list->count;
       i > 0 && list->chunks[i].start < list->chunks[i - 1].start; --i) {
    Chunk temp = list->chunks[i];
    list->chunks[i] = list->chunks[i - 1];
    list->chunks[i - 1] = temp;
  }

  list->count += 1;
  // assert(false && "TODO: chunk_list_insert not implemented yet");
}

void chunk_list_remove(Chunk_List *list, size_t index) {
  assert(index < list->count);

  for (size_t i = index; i < list->count - 1; ++i) {
    list->chunks[i] = list->chunks[i + 1];
  }
  list->count -= 1;
}

void chunk_list_merge(Chunk_List *list, Chunk_List *tmp) {

  tmp->count = 0;

  for (size_t i = 0; i < list->count; ++i) {
    const Chunk chunk = list->chunks[i];

    if (tmp->count > 0) {
      Chunk *top_chunk = &tmp->chunks[tmp->count - 1];
      if (top_chunk->start + top_chunk->size == chunk.start) {
        top_chunk->size += chunk.size;
      } else {
        chunk_list_insert(tmp, chunk.start, chunk.size);
      }
    } else {
      chunk_list_insert(tmp, chunk.start, chunk.size);
    }
  }
}

void *heap_alloc(size_t size) {

  if (size > 0) {

    // chunk_list_merge(&freed_chunks, &tmp_chunks);
    // freed_chunks = tmp_chunks;
    //
    for (size_t i = 0; i < freed_chunks.count; ++i) {

      Chunk chunk = freed_chunks.chunks[i];
      if (chunk.size >= size) {
        chunk_list_remove(&freed_chunks, i);
        void *ptr = chunk.start;
        size_t tail_size = chunk.size - size;
        chunk_list_insert(&allocated_chunks, chunk.start, size);

        if (tail_size > 0) {
          chunk_list_insert(&freed_chunks, chunk.start + size, tail_size);
        }

        return ptr;
      }
    }
  }
  return NULL;
}

void heap_free(void *ptr) {
  // for (size_t i = 0; i < allocated_size; ++i) {
  //   if (allocated_chunk[i].start == ptr) {
  //
  //   }
  // }

  if (ptr != NULL) {

    int index = chunk_list_find(&allocated_chunks, ptr);

    assert(ptr == allocated_chunks.chunks[index].start);

    chunk_list_insert(&freed_chunks, allocated_chunks.chunks[index].start,
                      allocated_chunks.chunks[index].size);
    chunk_list_remove(&allocated_chunks, (size_t)index);
  }
}
void heap_collect(void) {
  assert(false && "TODO: heap_collect not implemented");
}

#define N 10
void *ptrs[N] = {0};

int main(void) {
  for (int i = 0; i <= N; ++i) {
    ptrs[i] = heap_alloc(i);
  }

  for (int i = 0; i <= N; ++i) {
    if (i % 2 == 0) {
      heap_free(ptrs[i]);
    }
  }

  heap_alloc(10);

  chunk_list_dump(&allocated_chunks);
  chunk_list_dump(&freed_chunks);
  // heap_dump_allocated_chunks();

  return 0;
}
