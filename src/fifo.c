/*
    MIT License

    Copyright(c)[2022][Ovidiu Ionescu]

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this softwareand associated documentation files(the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions :

    The above copyright noticeand this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <malloc.h>
#include <string.h>
#include "fifo.h"

#define PRIVATE static
#define PUBLIC	FIFO_API

typedef
struct fifo_node_t
{
    /* public element */
    fifo_elm_t elm;

    /* next ptr */
    struct fifo_node_t* next;	
} fifo_node_t;

typedef
struct fifo_ctx_t
{
    /* public fifo */
    fifo_t		 fifo;

    /* head & tail */
    fifo_node_t* head;

    fifo_node_t* tail;
    
    /* size */
    uint32_t	 size;
} fifo_ctx_t;

static int32_t fifo_clear(IN fifo_t* fifo);
static int32_t fifo_is_empty(IN fifo_t* fifo);
static int32_t fifo_push(IN fifo_t* fifo, IN void* data, IN uint32_t size);
static fifo_elm_t* fifo_pop(IN fifo_t* fifo);
static uint32_t fifo_count(IN fifo_t* fifo);
static int32_t fifo_elm_free(IN fifo_elm_t** elm);

PUBLIC
fifo_t* 
FIFO_CALL
fifo_init()
{	
    fifo_ctx_t* ctx = (fifo_ctx_t*)calloc(1, sizeof(fifo_ctx_t));

    // check if allocation failed
    if (!ctx) return NULL;

    // fill the ops table
    const fifo_t fifo =
    {
        .clear = &fifo_clear,
        .is_empty = &fifo_is_empty,
        .push = &fifo_push,
        .pop = &fifo_pop,
        .count = &fifo_count
    };

    // copy functions table
    memcpy(&ctx->fifo, &fifo, sizeof(fifo_t));

    // initialize fifo vars
    ctx->head = ctx->tail = NULL;
    ctx->size = 0;

    // return initialized fifo
    return (fifo_t*) ctx;
}

PUBLIC
int32_t
FIFO_CALL
fifo_destroy(IN fifo_t** fifo)
{
    if (!fifo || !*fifo) return FIFO_ST_NULLPTR;

    // clear fifo
    (*fifo)->clear(*fifo);

    // free fifo struct
    free(*fifo);

    // make it null
    *fifo = NULL;

    return FIFO_ST_OK;
}

PRIVATE
int32_t fifo_clear(IN fifo_t* fifo)
{
    if (!fifo) return FIFO_ST_NULLPTR;

    for (fifo_elm_t* elm = fifo->pop(fifo); elm; elm = fifo->pop(fifo))
        elm->free(&elm);

    return FIFO_ST_OK;
}

PRIVATE
int32_t fifo_is_empty(IN fifo_t* fifo)
{
    if (!fifo) return FIFO_ST_NULLPTR;

    return fifo->count(fifo);
}

PRIVATE
int32_t fifo_push(IN fifo_t* fifo, IN void* data, IN uint32_t size)
{
    if (!fifo || !data) return FIFO_ST_NULLPTR;

    // get ctx
    fifo_ctx_t* ctx = (fifo_ctx_t*)fifo;

    // allocate fifo node
    fifo_node_t* node = (fifo_node_t*)calloc(1, sizeof(fifo_node_t));

    // check for alloc failure
    if (!node) return FIFO_ST_OOM;

    // fill the ops table
    const fifo_elm_t elm =
    {
        .data = data,
        .size = size,
        .free = &fifo_elm_free,
    };

    // copy functions table
    memcpy(&node->elm, &elm, sizeof(fifo_elm_t));
    
    // check if we need to alloc element memory
    if (size)
    {
        node->elm.data = malloc(size);
        if (!node->elm.data)
        {
            free(node);
            return FIFO_ST_OOM;
        }
        memcpy(node->elm.data, data, size);
    }
        
    // check if queue is empty
    if (!ctx->size)
    {
        ctx->tail = node;
        ctx->head = node;
    }

    // node becomes head
    ctx->head->next = node;
    ctx->head = node;

    // grow size
    ctx->size++;
    
    return FIFO_ST_OK;
}

PRIVATE
fifo_elm_t* fifo_pop(IN fifo_t* fifo)
{
    if (!fifo) return NULL;

    // get ctx
    fifo_ctx_t* ctx = (fifo_ctx_t*)fifo;
    
    // check if empty
    if (!ctx->size) return NULL;

    // get node elm
    fifo_elm_t* elm = (fifo_elm_t*) ctx->tail;

    // move tail
    ctx->tail = ctx->tail->next;

    // adjust size
    ctx->size--;

    // check if empty
    if (!ctx->size)
    {
        ctx->head = NULL;
    }

    return elm;
}

PRIVATE
uint32_t fifo_count(IN fifo_t* fifo)
{
    if (!fifo) return 0;

    // get ctx
    fifo_ctx_t* ctx = (fifo_ctx_t*)fifo;

    return ctx->size;
}

PRIVATE
int32_t fifo_elm_free(IN fifo_elm_t** elm)
{
    if (!elm || !*elm) return FIFO_ST_NULLPTR;

    fifo_node_t* node = (fifo_node_t*)*elm;

    // check if we allocated memory
    if (node->elm.size)
        free(node->elm.data);

    // free node
    free(node);

    // make it null
    *elm = NULL;

    return FIFO_ST_OK;
}
