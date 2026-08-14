#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// struct that stores
// the data of type (char *)
// size of the array
// current capacity of array
typedef struct {
  char **data;
  size_t size;
  size_t capacity;
} Vector;


// initialize everything to null or 0
void vector_init(Vector *vector) {
  vector->data = NULL;
  vector->size = 0;
  vector->capacity = 0;
}

bool vector_push(Vector *vector, char *data) {
  // if the array is full i.e. full or capacity 0
  // if capacity is 0 i.e. array is initialized give new_capacity to 4
  // if its full the new_Capacity is double of capacity
  // capacity is number of (char *) that can be stored in a array(char **)
  if (vector->size == vector->capacity) {
    size_t new_capacity;
    if (vector->capacity == 0) {
      new_capacity = 4;
    } else {
      new_capacity = vector->capacity * 2;
    }

    // if its full or empty allocate/reallocate some space
    // if empty then allocate
    // otherwise deallocate but we can use same function for both
    char **new_data = (char **)realloc(vector->data, new_capacity * sizeof(char *));

    // validation for realloc
    if (new_data == NULL) {
      return false;
    }

    // pass the new data and capacity but currently the data is null for new index
    vector->data = new_data;
    vector->capacity = new_capacity;
  }

  // new data is passed here and size is increased
  vector->data[vector->size] = data;
  vector->size++;
  return true;
}

bool vector_poop(Vector *vector, size_t index) {

  // check if vector is empty or index is greater than or equal to size of array
  if (vector == NULL || index >= vector->size) {
      return false;
  }
  // earlier: 6
  // now: 5
  // poop: 5
  // 0->0, 1->1, 2->2, 3->3, 4->4
  for (size_t i = index; i < vector->size - 1; i++) {
    vector->data[i] = vector->data[i + 1];
  }

  // decrease the size and remove the data for that index
  vector->size--;
  vector->data[vector->size] = NULL;
  return true;
}
