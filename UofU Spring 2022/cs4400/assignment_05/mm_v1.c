/*
 * mm-naive.c - The least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by allocating a
 * new page as needed.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

// DEFAULT MACROS

/* always use 16-byte alignment */
#define ALIGNMENT 16

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

/* rounds up to the nearest multiple of mem_pagesize() */
#define PAGE_ALIGN(size) (((size) + (mem_pagesize()-1)) & ~(mem_pagesize()-1))

// STUDENT ADDED HELPER FUNCTIONS

static void set_allocated(void *b, size_t size);

static void* extend(size_t s);

// STUDENT ADDED MACROS/structs

// size of the header and footer
#define OVERHEAD (sizeof(block_header) + sizeof(block_footer))

// Given a pointer to a header, get its value
#define GET(p) (*(size_t *)(p))

// Given a pointer to a header, set its value
#define PUT(p, val) (*(size_t *)(p) = (val))

// Combine a size and alloc bit
#define PACK(size, alloc) ((size) | (alloc))

// Given a header pointer, get the alloc bit
#define GET_ALLOC(p) (GET(p) & 0x1)

// Given a header pointer, get the previous block's alloc bit
#define GET_PREV_ALLOC(p) (GET(p) & 0x2)

// Given a header pointer, get the size
#define GET_SIZE(p) (GET(p) & ~0xF)

// Gets the header from the payload pointer
#define HDRP(bp) ((void *)(bp) - sizeof(block_header))

// Get the footer from the payload pointer
#define FTRP(bp) ((void *)(bp) + GET_SIZE(HDRP(bp)) - OVERHEAD)

// Get the next payload pointer
#define NEXT_BLKP(bp) ((void *)(bp) + GET_SIZE(HDRP(bp)))

// Get the previous payload pointer
#define PREV_BLKP(bp) ((void *)(bp) - GET_SIZE((void *)(bp) - OVERHEAD))

#define INIT_BLOCK_SIZE ((mem_pagesize()) - (sizeof(pageList)) - (sizeof(PADDING)) - sizeof(term))

typedef size_t block_header;

typedef size_t block_footer;

typedef size_t term;

typedef size_t PADDING;

typedef struct pageList {
  size_t * prev;
  size_t * next;
} pageList;

typedef struct freeList {
  size_t *prev;
  size_t *next;
} freeList;
///////////////////////////////////////////////

void *current_avail = NULL;
int current_avail_size = 0;

int firstpage = 1;

int printonce = 1;

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
  
  //Set up Default Variables
  current_avail = NULL;
  current_avail_size = 0;
  firstpage = 1;

  //Sets up the first page
  extend(mem_pagesize());
  
  return 0;
}

/* 
 * mm_malloc - Allocate a block by using bytes from current_avail,
 *     grabbing a new page if necessary.
 */
void *mm_malloc(size_t size)
{
  size_t newsize = ALIGN(size);
  void *p;
  
  // Check available blocks to fit new block 
  if (printonce)
  {
    printonce = 0;
    printf("%d\n", GET_SIZE(HDRP(current_avail)));
    printf("%d\n", GET_SIZE((FTRP(current_avail) + sizeof(size_t))));
  }
  /* while(1) */
  /* { */
  /*   //Check current block to see if unalloc, and if the right size */
  /*   printf("In the while loo"); */
  /*   if (GET_SIZE((FTRP(current_avail) + sizeof(size_t))) == 0) */
  /*   { */
  /*     break; */
  /*   } */
  /*   //current_avail down by 1 block */
  /* } */
  
  // if No available blocks, make a new page and call extend
  
  
  
  if (current_avail_size < newsize) {
    current_avail_size = PAGE_ALIGN(newsize);
    current_avail = mem_map(current_avail_size);
    if (current_avail == NULL)
      return NULL;
  }

  p = current_avail;
  current_avail += newsize;
  current_avail_size -= newsize;
  
  return p;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
}

static void set_allocated(void *b, size_t size)
{
}

static void* extend(size_t s)
{
  
  
  // Call mem_map to get new page
  void * p = mem_map(s);
  void * headOfPage = p;
  if (p == NULL)
  {
    return headOfPage;					
  }

  if(firstpage)
  {
    firstpage = 0;
    current_avail = p + sizeof(PADDING) + sizeof(pageList) + sizeof(block_header);
    current_avail_size = INIT_BLOCK_SIZE;
  }
  
  p = p + sizeof(PADDING); // PADDING FOR ALIGNMENT
  
  // Set up the pointer to next page
  ((pageList *)p) -> prev = NULL;

  // Set up the pointer to prev page
  ((pageList *)p) -> next = NULL;
  

  // set up header
  p = p + sizeof(pageList);
  PUT((block_header *)p, PACK(INIT_BLOCK_SIZE, 2));
  p = p + sizeof(block_header);
  
  // set up footer
  p = FTRP(p);
  PUT((block_footer *)p, PACK(INIT_BLOCK_SIZE, 2));
  p = p + sizeof(block_footer);

  // set up terminator block
  PUT((term *)p, PACK(0, 1));

  return headOfPage;
}

/* static void* coalesce(void *bp) */
/* { */
  
/* } */
