# libzfifo

Simple C fifo, not thread SAFE.

## Initialize
```cpp
#include "fifo.h"

int main()
{
    // init fifo
    fifo_t* fifo = fifo_init();

    // destroy fifo
    fifo_destroy(&fifo);
}
```

## Push element and store it
```cpp
// store an int
int i = 1;
fifo->push(fifo, &i, sizeof(int));

// store a string
char* simple = "this is a string"
fifo->push(fifo, (void*)simple, strlen(simple) + 1);
```

## Push element by reference
```cpp
// store int reference
int i = 1;
fifo->push(fifo, &i, 0);

// store a string by reference
char* simple = "this is a string"
fifo->push(fifo, (void*)simple, 0);
```

## Pop & free element
```cpp
fifo_elm_t* elm = fifo->pop(fifo);
if (elm) elm->free(&elm);
```

## Get number of elements
```cpp
fifo->count(fifo);
```

## Clear fifo
```cpp
fifo->clear(fifo);
```

## License
MIT

## Project status
Draft
