﻿#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "array.h"


// Construction / Destruction

Array array_new(long capacity){
    assert(capacity > 0);
    return (Array){malloc(sizeof(long)*capacity), 0, 0, capacity};
}

void array_destroy(Array a){
    free(a.data);
}


// Primitives

long array_empty(Array a){
    return a.back <= a.front;
}

long array_front(Array a){
    return a.data[a.front];
}

long array_back(Array a){
    return a.data[a.back - 1];
}

void array_popFront(Array* a){
    a->front++;
}

void array_popBack(Array* a){
    a->back--;
}

Array array_save(Array a){
    return (Array){a.data, a.front, a.back, a.capacity};
}


// Iteration

void array_foreach(Array a, void fn(long)){
    for(Array b = array_save(a); !array_empty(b); array_popFront(&b)){
        fn(array_front(b));
    }
}

void array_foreachReverse(Array a, void fn(long)){
    for(Array b = array_save(a); !array_empty(b); array_popBack(&b)){
        fn(array_back(b));
    }
}

static void _array_printSingleLongHelper(long i){
    printf(", %ld", i);
}

void array_print(Array a){
    printf("Array:{");
    if(!array_empty(a)){
        printf("%ld", array_front(a));
        array_popFront(&a);
    }
    array_foreach(a, _array_printSingleLongHelper);
    printf("}\n");
}


// Capacity

long array_length(Array a){
    return a.back - a.front;
}

void array_reserve(Array* a, long capacity){

    long* newArray = malloc(sizeof(long)*capacity);
    for(int i = a->front, j =0; i<= a->back && j< capacity; ++i, ++j){
        newArray[j] = a->data[i];
    }
    free(a->data);
    a->data = newArray;
    a->capacity = capacity;
}


// Modifiers

void array_insertBack(Array* a, long stuff){
    const double expandFactor = EXPAND_FACTOR;
    if(a-> back >= a-> capacity -1){
        // printf("RESERVE %ld\n", a->capacity*expandFactor);
        array_reserve(a ,a->capacity *expandFactor+1);// Amortized O(1), rubish for real-time
        // Massive task to move the data
        // Exponential use in memory, which does not shrink
    }
    a->data[a->back] = stuff;
    a->back +=1;

}


