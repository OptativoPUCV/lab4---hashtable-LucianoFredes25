#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"
#include <stdbool.h>

typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  int aux = hash(key,map->capacity);

  while(true){
    if(map->buckets[aux] == NULL || map->buckets[aux]->key == NULL){
      map->buckets[aux] = createPair(key, value);
      break;
    } 
    else{
      aux++;
      if(aux == map->capacity)
        aux=0;
    }
  }
  map->current = aux;
  map->size++;
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  
  Pair ** aux = map->buckets;
  map->capacity *= 2;
  HashMap * mapa = createMap(map->capacity);
  map->buckets = mapa->buckets;
  map->size = 0;
  for(size_t i = 0 ; i < map->capacity / 2 ; i++){
    if(aux[i] != NULL)
      insertMap(map, aux[i]->key, aux[i]->value);
  }
}

HashMap * createMap(long capacity) {
  HashMap * mapa = (HashMap *) malloc(sizeof(HashMap));
  mapa->buckets = (Pair **) calloc(capacity, sizeof(Pair *));
  mapa->size = 0;
  mapa->capacity = capacity;
  mapa->current = -1;
  return mapa;
}

void eraseMap(HashMap * map,  char * key) {    
  int aux = hash(key,map->capacity);
  while(true){
    if(map->buckets[aux] == NULL)
      break;
    
    if(is_equal(map->buckets[aux]->key , key)){
      map->buckets[aux]->key = NULL;
      map->size--;
      break;
    }
    else{
      aux++;
      if(aux > map->capacity)
        aux = 0;
    }
  }
}

Pair * searchMap(HashMap * map,  char * key) {   
  int aux = hash(key,map->capacity);
  while(true){
    if(map->buckets[aux] == NULL || map->buckets[aux]->key == NULL)
      return NULL;

    if(is_equal(map->buckets[aux]->key , key)){
      map->current = aux;
      return map->buckets[aux];
    }
    else{
      aux++;
      if(aux > map->capacity)
        aux = 0;
    }
  }
}

Pair * firstMap(HashMap * map) {
  for(int i = 0 ; i < map->capacity ; i++){
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      map->current = i;
      return map->buckets[i];
    }  
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {
  for(int i = map->current + 1 ; i < map->capacity ; i++){
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      map->current = i;
      return map->buckets[i];
    }  
  }
  return NULL;
}
