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

static void *set_allocated(void *b, size_t size);

static int set_alloc_bit(void *b);

static int set_prev_alloc_bit(void *b);

static int clear_alloc_bit(void *b);

static int clear_prev_alloc_bit(void *b);

static void *extend(size_t s);

// STUDENT ADDED MACROS/structs

// smallest possible block size (header + payload + footer)
#define SMALLEST_BLOCKSIZE 32

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

#define INIT_BLOCK_SIZE(s) ((s) - (sizeof(pageList)) - (sizeof(PADDING)) - sizeof(term))

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
void *first_ever_block = NULL;
int current_avail_size = 0;

int firstpage = 1;

int printonce = 1;
int printonce2 = 1;

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
  
  //Set up Default Variables
  current_avail = NULL;
  first_ever_block = NULL;
  current_avail_size = 0;
  firstpage = 1;

  //Sets up the first page
  first_ever_block = extend(mem_pagesize()); //Points the very first ever payload
  
  return 0;
}

/* 
 * mm_malloc - Allocate a block by using bytes from current_avail,
 *     grabbing a new page if necessary.
 */
void *mm_malloc(size_t size)
{
  size_t newsize = ALIGN(size + OVERHEAD);
  void *p;
  // Check available blocks to fit new block 
  if (printonce)
  {
    printonce = 0;
    printf("%d\n", GET_SIZE(HDRP(first_ever_block)));
    printf("%d\n", GET_SIZE((FTRP(first_ever_block) + sizeof(size_t))));
  }

  void *block_iter = first_ever_block;
  void *curr_pageHead = first_ever_block;

  while(1)
  {
    //Check current block to see if unalloc, and if the right size (DONE SO FIRST FIT STYLE)
    // If the current block is unalloc and the right size (FIRST FIT) then allocate that block, and break
    if (GET_ALLOC(HDRP(block_iter)) == 0 && GET_SIZE(HDRP(block_iter)) >= newsize)
    {
      p = set_allocated(block_iter, newsize);
      break;
    }

    if (GET_SIZE((FTRP(block_iter) + sizeof(size_t))) == 0)
    {
      //Move to the next page if available, if not call extend to make a new page
      
      break;
    }

    // moves the block iterator to the next block
    block_iter = NEXT_BLKP(block_iter);
  }
  
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

static void *set_allocated(void *b, size_t size)
{
  void *allocd_Block = b;
  size_t new_BlockSize = (GET_SIZE(HDRP(b)) - size);
  
  // Allocates the block and Splits it if applicable
  if (new_BlockSize >= SMALLEST_BLOCKSIZE)
  {
    /* if (printonce2) */
    /* { */
    /*   printf("inside of the split block if\n"); */
    /*   printf("size of b before alloc is: %d\n", GET_SIZE(HDRP(b))); */
    /*   printf("pre current alloc bit is: %d\n", GET_ALLOC(HDRP(b))); */
    /*   printf("pre prev alloc bit is: %d\n", GET_PREV_ALLOC(HDRP(b))); */
    /* } */
    
    // sets the current block header's alloc bit
    PUT(HDRP(b), PACK(size, set_alloc_bit(HDRP(b))));

    /* if (printonce2) */
    /* { */
    /*   printf("size of b after alloc is: %d\n", GET_SIZE(HDRP(b))); */
    /*   printf("current alloc bit is: %d\n", GET_ALLOC(HDRP(b))); */
    /*   printf("prev alloc bit is: %d\n", GET_PREV_ALLOC(HDRP(b))); */
    /* } */
    
    // Split the block
    b = FTRP(b);            // pointing at the posiion of the new footer

    // Sets up the new footer for the allocated block
    PUT(b, PACK(size, set_alloc_bit(b)));

    /* if (printonce2) */
    /* { */
    /*   printf("size of b's footer after alloc is: %d\n", GET_SIZE(b)); */
    /* } */
    
    b = b + sizeof(block_footer);       // pointing at the position of the header of the new split block

    // Set up the new split block's header.
    PUT(b, PACK(new_BlockSize, 2));

    /* if (printonce2) */
    /* { */
    /*   printf("size of newly split block is: %d\n", GET_SIZE(b)); */
    /* } */
    

    b = FTRP(b + sizeof(block_header)); // pointing at the position of the footer of the new split block

    // Set up the new split block's footer.
    PUT(b, PACK(new_BlockSize, 2));

    /* if (printonce2) */
    /* { */
    /*   printf("size of newly split block footer is: %d\n", GET_SIZE(b)); */
    /*   printonce2 = 0; */
    /* } */
  }

  // Set's the allocated bit for the current block and DOESNT SPLIT
  else
  {
    // sets up the current block's alloc bit
    PUT(b, PACK(GET_SIZE(HDRP(b)), set_alloc_bit(HDRP(b))));

    // set next block pointer's previous alloc bit     
    PUT(NEXT_BLKP(b), PACK(GET_SIZE(HDRP(NEXT_BLKP(b))), set_prev_alloc_bit(HDRP(NEXT_BLKP(b)))));
  }

  return allocd_Block;
}

static void *extend(size_t s)
{
  size_t pageLength = PAGE_ALIGN(s);
  
  // Call mem_map to get new page
  void * p = mem_map(pageLength);
  void * headOfPage = p + sizeof(PADDING) + sizeof(pageList) + sizeof(block_header);
  if (p == NULL)
  {
    return headOfPage;					
  }

  if(firstpage)
  {
    firstpage = 0;  
    current_avail = p + sizeof(PADDING) + sizeof(pageList) + sizeof(block_header);
    current_avail_size = INIT_BLOCK_SIZE(pageLength);
  }
  
  p = p + sizeof(PADDING); // PADDING FOR ALIGNMENT
  
  // Set up the pointer to next page
  ((pageList *)p) -> prev = NULL;

  // Set up the pointer to prev page
  ((pageList *)p) -> next = NULL;
  

  // set up header
  p = p + sizeof(pageList);
  PUT((block_header *)p, PACK(INIT_BLOCK_SIZE(pageLength), 2));
  p = p + sizeof(block_header);
  
  // set up footer
  p = FTRP(p);
  PUT((block_footer *)p, PACK(INIT_BLOCK_SIZE(pageLength), 2));
  p = p + sizeof(block_footer);

  // set up terminator block
  PUT((term *)p, PACK(0, 1));

  return headOfPage;
}

/* static void* coalesce(void *bp) */
/* { */
  
/* } */

static int set_alloc_bit(void *b)
{
  // Previous Alloc bit is unalloc
  if(GET_PREV_ALLOC(b) == 0)
  {
    return 1; 
  }

  // Previous Alloc bit is alloc
  else
  {
    return 3;
  }
}

static int set_prev_alloc_bit(void *b)
{
  // Current Alloc bit is unalloc
  if(GET_ALLOC(b) == 0)
  {
    return 2; 
  }

  // Current Alloc bit is alloc
  else
  {
    return 3;
  }
}

static int clear_alloc_bit(void *b)
{
  // Previous Alloc bit is unalloc
  if(GET_PREV_ALLOC(b) == 0)
  {
    return 0; 
  }

  // Previous Alloc bit is alloc
  else
  {
    return 2;
  }
}

static int clear_prev_alloc_bit(void *b)
{
  // Current Alloc bit is unalloc
  if(GET_ALLOC(b) == 0)
  {
    return 0; 
  }

  // Current Alloc bit is alloc
  else
  {
    return 1;
  }
}
