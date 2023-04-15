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
  
  HashMap * mapa = createMap(map->capacity);
  mapa->buckets = map->buckets;
  mapa->size = map->size;
  Pair * aux = firstMap(mapa);
  printf("%ld", mapa->size);
  map = createMap(mapa->capacity * 2);
  printf("%ld", map->size);
  while(true){
    if(aux == NULL)
      break;
    insertMap(map, aux->key, aux->value);
    map->size++;
    aux=nextMap(mapa);
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
