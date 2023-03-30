#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ts_hashmap.h"

/**
 * Creates a new thread-safe hashmap. 
 *
 * @param capacity initial capacity of the hashmap.
 * @return a pointer to a new thread-safe hashmap.
 */

//Locking mechainism
 pthread_mutex_t lock;

ts_hashmap_t *initmap(int capacity) {
  
  //Creasts a map pointer
  ts_hashmap_t *map = (ts_hashmap_t*) malloc(sizeof(ts_hashmap_t) * 1);

  //Populates the map with ts_entry_t slots based on the capacity
  map->table = (ts_entry_t**) malloc(sizeof(ts_entry_t*) * capacity);

  //Initializes entries to NULL
  for(int i = 0; i < capacity; i++){
    map->table[i] = NULL;
  }

  //Sets capacity of this specific map, and returns the ts_hashmap_t
  map->capacity = capacity;
  return map;
}

/**
 * Obtains the value associated with the given key.
 * @param map a pointer to the map
 * @param key a key to search
 * @return the value associated with the given key, or INT_MAX if key not found
 */

int get(ts_hashmap_t *map, int key) {

  //Spinlocks
  while(pthread_mutex_init(&lock, NULL) != 0){
    //Spin
  }

  //Obtains lock
  pthread_mutex_lock(&lock);

  //Obtains correct index for map
  int index = key % map->capacity;

  //Accesses current entry
  ts_entry_t *entry = map->table[index];

  //Returns associated value if the given key exists, otherwise returns INT_MAX
  if(entry != NULL){
    pthread_mutex_unlock(&lock);
    return entry->value;
  }
  pthread_mutex_unlock(&lock);
  return INT_MAX;
}

/**
 * Associates a value associated with a given key.
 * @param map a pointer to the map
 * @param key a key
 * @param value a value
 * @return old associated value, or INT_MAX if the key was new
 */
int put(ts_hashmap_t *map, int key, int value) {

  //Spin locks
  while(pthread_mutex_init(&lock, NULL) != 0){
    //Spin
  }

  //Obtains lock
  pthread_mutex_lock(&lock);

  //Obtains correct index for map
  int index = key % map->capacity;
  
  //Accesses current entry
  ts_entry_t *entry = map->table[index];  

  //If the entry does not exist, creates a new entry, and populates with new key/value
  if(entry == NULL){
    ts_entry_t *newEntry = (ts_entry_t*) malloc(sizeof(ts_entry_t) * 1);
    newEntry->key = key;
    newEntry->value = value;
    map->table[index] = newEntry;
  }

  //Iterates through values until a null value is located, searching for given key
  while(entry != NULL){
    if(entry->key == key){
      int oldVal = entry->value;
      int* val = (int*) malloc(sizeof(int) * 1);
      free(entry);
      
      entry->value = *val;
      pthread_mutex_unlock(&lock);
      return oldVal;
    }
    entry = entry->next;
  }
  pthread_mutex_unlock(&lock);
  return INT_MAX;
}

/**
 * Removes an entry in the map
 * @param map a pointer to the map
 * @param key a key to search
 * @return the value associated with the given key, or INT_MAX if key not found
 */
int del(ts_hashmap_t *map, int key) {

  //Spin locks
  while(pthread_mutex_init(&lock, NULL) != 0){
    //Spin
  }

  pthread_mutex_lock(&lock);

  //Obtains correct index for map
  int index = key % map->capacity;

  //Accesses current entry
  ts_entry_t *entry = map->table[index];

  //If there is a value assiciated with key, removes entry and returns old value
  if(entry != NULL){
    int oldVal = entry->value;
    map->table[index] = NULL;
    free(entry);
    pthread_mutex_unlock(&lock);
    return oldVal;
  }
  pthread_mutex_unlock(&lock);
  return INT_MAX;
}


/**
 * @return the load factor of the given map
 */
double lf(ts_hashmap_t *map) {
  return (double) map->size / map->capacity;
}

/**
 * Prints the contents of the map
 */
void printmap(ts_hashmap_t *map) {
  for (int i = 0; i < map->capacity; i++) {
    printf("[%d] -> ", i);
    ts_entry_t *entry = map->table[i];
    while (entry != NULL) {
      printf("(%d,%d)", entry->key, entry->value);
      if (entry->next != NULL)
        printf(" -> ");
      entry = entry->next;
    }
    printf("\n");
  }
}
