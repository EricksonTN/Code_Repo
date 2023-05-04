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

static void* coalesce(void *bp);

static void* free_block_picker(size_t bp);

static void add_to_free_list(void * ptr);

static void remove_from_free_list (void * ptr);

void checkHeap();

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

#define IS_PROLOG(p) (GET(p) & 0x4)

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
  struct pageList * prev;
  struct pageList * next;
} pageList;

typedef struct freeList {
  struct freeList *prev;
  struct freeList *next;
} freeList;
///////////////////////////////////////////////

int pageCount = 0;

void *first_ever_page = NULL;
freeList *freeBlocks[10];
freeList *freeHead = NULL;

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
  
  //Set up Default Variables
  pageCount = 0;
  freeHead = NULL;
  
  first_ever_page = NULL;
  
  
  //Sets up the first page
  first_ever_page = extend(mem_pagesize()); //Points the very beginning of the first page.
  pageCount++;
  if (first_ever_page == NULL)
  {
    return -1;
  }
  
  ///void *first_ever_block = first_ever_page + sizeof(pageList) + sizeof(PADDING) + sizeof(block_header);
  //((freeList *)first_ever_block) -> prev = NULL;
  //((freeList *)first_ever_block) -> next = NULL;
  
  //freeHead = first_ever_block;
  
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
  
  void *block_iter = first_ever_page + sizeof(pageList) + sizeof(PADDING) + sizeof(block_header);
  void *curr_pageHead = first_ever_page;

  ///////////////////////////////////DOUBLY LINKED EXPLICIT FREE LIST IMPLEMENTATION/////////////////////////////
  /* void *freeList_iter = freeHead; */
  /* while (1) */
  /* { */
  /*   // Checks to see if the current free block can fit the newsize */
  /*   if (GET_SIZE(HDRP(freeList_iter)) >= newsize) */
  /*   { */
  /*     p = set_allocated(freeList_iter, newsize); */
  /*     remove_from_free_list(freeList_iter); */
  /*     break; */
  /*   } */
  /*   // If current free block is the last in the linked list, make a new page */
  /*   if (((freeList *)freeList_iter) -> next == NULL) */
  /*   { */
  /*     void *newPage_listTrav = extend(newsize); */
  /*     pageCount++; */
	
  /*     // pointer to the first payload in new page */
  /*     void *newPagePay = newPage_listTrav + sizeof(pageList) + sizeof(PADDING) + sizeof(block_header); */
  /*     void *lastPage = NULL; */

  /*     while(1) */
  /*     { */
  /* 	if (((pageList *)curr_pageHead) -> next == NULL) */
  /* 	{ */
  /* 	  lastPage = curr_pageHead; */
  /* 	  break; */
  /* 	} */
  /* 	curr_pageHead = ((pageList *)curr_pageHead) -> next; */
  /*     } */

  /*     // set up new page in the doubly linked list for pages */
  /*     ((pageList *)lastPage) -> next = newPage_listTrav; */
  /*     ((pageList *)newPage_listTrav) -> prev = lastPage; */

  /*     p = set_allocated(newPagePay, newsize); */
  /*     break; */
  /*   } */
    
  /*   else */
  /*   { */
  /*     freeList_iter = ((freeList *)freeList_iter) -> next; */
  /*   } */
  /* } */

  /* return p; */
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //A pointer pointing at the head of the pageList struct
  void *curr_listTrav = curr_pageHead;
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
      if (((pageList *)curr_listTrav) -> next != NULL)
      {
  	void *headOfNextPage = ((pageList *)curr_listTrav) -> next;
      	block_iter = headOfNextPage + sizeof(pageList) + sizeof(PADDING) + sizeof(block_header);
  	curr_listTrav = ((pageList *)curr_listTrav) -> next;
  	continue;
      }
      
      //There exists no next page. we make a page.
      else if (((pageList *)curr_listTrav) -> next == NULL)
      {
      	// allocates a new page
      	void *newPage_listTrav = extend(newsize);
  	pageCount++;
	
      	// pointer to the first payload in new page
      	void *newPagePay = newPage_listTrav + sizeof(pageList) + sizeof(PADDING) + sizeof(block_header);

      	// set up new page in the doubly linked list for pages
      	((pageList *)curr_listTrav) -> next = newPage_listTrav;
      	((pageList *)newPage_listTrav) -> prev = curr_listTrav;

      	p = set_allocated(newPagePay, newsize);
      	break;
      }

      
    }

    // moves the block iterator to the next block
    block_iter = NEXT_BLKP(block_iter);
  }

  
  //checkHeap();
  
  return p;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
  
  // clears the alloc bit on ptr
  void *newPtr = coalesce(ptr);
  void *prevPage = NULL;
  void *nextPage = NULL;

  int numAlloc = 0;

  void *nextblk = FTRP(newPtr) + sizeof(size_t);
  void *prevblk = HDRP(newPtr) - sizeof(PADDING);

  size_t nextblksize = GET_SIZE(nextblk);
  size_t test = IS_PROLOG(nextblk);
  size_t prevblksize = IS_PROLOG(prevblk);

  

  // if the recently coalesced block is not the only block in the page, then set the next page's prev alloc bit to unalloc
  if (GET_SIZE((FTRP(newPtr) + sizeof(size_t))) != 0)
  {
    // set the next block's alloc bit to 0
    PUT(NEXT_BLKP(newPtr), PACK(GET_SIZE(HDRP(NEXT_BLKP(newPtr))), clear_prev_alloc_bit(HDRP(NEXT_BLKP(newPtr)))));
  }
  
  // if the recently coalesced block is the only block in the page, unmap this page.
  
  else if (GET_SIZE((FTRP(newPtr) + sizeof(size_t))) == 0)
  {
    /* if(IS_PROLOG((HDRP(newPtr) - sizeof(PADDING))) == 4) // the prolog is before this block */
    /* { */
    /*   void *newPtrHead = newPtr - sizeof(block_header) - sizeof(PADDING) - sizeof(pageList); */
    
    /*   prevPage = ((pageList *)newPtrHead) -> prev; */
    /*   void * pp_Pay = prevPage + sizeof(pageList) + sizeof(PADDING) + sizeof(block_header); */
    /*   size_t pp_Size = GET_SIZE(HDRP(pp_Pay)); */
      
    /*   nextPage = ((pageList *)newPtrHead) -> next; */

    /*   // Case 1: page we want to free is the very first page in the linked pagelist */
    /*   if ((prevPage == NULL) && (nextPage != NULL)) */
    /*   { */
    /* 	((pageList *)nextPage) -> prev = NULL; */
    /* 	first_ever_page = nextPage; */
    /*   } */
    
    /*   // Case 2: page we want to free is somewhere in the middle of the linked pagelist */
    /*   else if ((prevPage != NULL) && (nextPage != NULL)) */
    /*   { */
    /* 	((pageList *)prevPage) -> next = ((pageList *)newPtrHead) -> next; */
    /* 	((pageList *)nextPage) -> prev = ((pageList *)newPtrHead) -> prev; */
    /*   } */
    
    /*   // Case 3: page we want to free is the very last page in the linked pagelist */
    /*   else if ((prevPage != NULL) && (nextPage == NULL)) */
    /*   { */
    /* 	((pageList *)prevPage) -> next = NULL; */
    /*   } */

    /*   ((pageList *)newPtrHead) -> next = NULL; */
    /*   ((pageList *)newPtrHead) -> prev = NULL; */
    /*   size_t unMapSize = PAGE_ALIGN(GET_SIZE(HDRP(newPtr))); */
    /*   mem_unmap(newPtrHead, unMapSize); */
    /* } */
  }
}

static void *set_allocated(void *b, size_t size)
{
  void *allocd_Block = b;
  size_t new_BlockSize = (GET_SIZE(HDRP(b)) - size);
  
  // Allocates the block and Splits it if applicable
  if (new_BlockSize >= (4 * SMALLEST_BLOCKSIZE))
  {
    
    // sets the current block header's alloc bit
    PUT(HDRP(b), PACK(size, set_alloc_bit(HDRP(b))));

    
    // Split the bloc
    // Sets up the new footer for the allocated block
    PUT(FTRP(b), PACK(size, set_alloc_bit(HDRP(b))));
    

    // Set up the new split block's header.
    PUT(HDRP(NEXT_BLKP(b)), PACK(new_BlockSize, 2));
    

    // Set up the new split block's footer.
    PUT(FTRP(NEXT_BLKP(b)), PACK(new_BlockSize, 2));

    // Add the newly split unalloc block to the free list
    //add_to_free_list(NEXT_BLKP(b));
  }

  // Set's the allocated bit for the current block and DOESNT SPLIT
  else
  {
    // sets up the current block's alloc bit
    PUT(HDRP(b), PACK(GET_SIZE(HDRP(b)), set_alloc_bit(HDRP(b))));

    // set next block pointer's previous alloc bit     
    PUT(NEXT_BLKP(b), PACK(GET_SIZE(HDRP(NEXT_BLKP(b))), set_prev_alloc_bit(HDRP(NEXT_BLKP(b)))));
  }

  return allocd_Block;
}

static void *extend(size_t s)
{
  size_t pageScalar = PAGE_ALIGN(s * (pageCount + 1));
  
  size_t pageLength = pageScalar + mem_pagesize();

  // Call mem_map to get new page
  void * p = mem_map(pageLength);
  void * headOfPage = p; // returns the very beginning of the new page.
  if (p == NULL)
  {
    return p;					
  }
    
  // Set up the pointer to next page
  ((pageList *)p) -> prev = NULL;

  // Set up the pointer to prev page
  ((pageList *)p) -> next = NULL;

  p = p + sizeof(pageList); 

  PUT(p, PACK(4, 1));   // storing a prolog in the padding
  p = p + sizeof(PADDING);
  
  // set up header
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

static void* coalesce(void *bp)
{
  void * ptr = NULL;
  size_t sizeAccum = GET_SIZE(HDRP(bp));

  int pCoal = 0;

  //Checks to see if prev block is unalloc
  if (GET_ALLOC(HDRP(PREV_BLKP(bp))) == 0)
  {
    sizeAccum = sizeAccum + GET_SIZE(HDRP(PREV_BLKP(bp)));
    pCoal = 1;
  }

  //Checks to see if next block is unalloc
  if (GET_ALLOC(HDRP(NEXT_BLKP(bp))) == 0)
  {
    //remove_from_free_list(NEXT_BLKP(bp));
    
    sizeAccum = sizeAccum + GET_SIZE(HDRP(NEXT_BLKP(bp)));
  }

  if (pCoal)
  {
    PUT(HDRP(PREV_BLKP(bp)), PACK(sizeAccum, clear_alloc_bit(HDRP(PREV_BLKP(bp)))));
    PUT(FTRP(PREV_BLKP(bp)), PACK(sizeAccum, clear_alloc_bit(HDRP(PREV_BLKP(bp)))));
    ptr = PREV_BLKP(bp);
  }

  else
  {
    PUT(HDRP(bp), PACK(sizeAccum, clear_alloc_bit(HDRP(bp))));
    PUT(FTRP(bp), PACK(sizeAccum, clear_alloc_bit(HDRP(bp))));
    //add_to_free_list(bp);
    ptr = bp;
  }

  return ptr;
}

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

static void* free_block_picker(size_t bp)
{
  void * returnAllocPtr = NULL;
  
  int multOfAlign = bp / 16;

  return returnAllocPtr;
}

static void add_to_free_list(void * ptr)
{
  ((freeList *)ptr) -> prev = NULL;
  ((freeList *)ptr) -> next = freeHead;
  ((freeList *)freeHead) -> prev = ptr;
  
  freeHead = ptr;
}

static void remove_from_free_list (void * ptr)
{
  void *prevBlock = ((freeList *)ptr) -> prev;
  void *nextBlock = ((freeList *)ptr) -> next;

  // case where the remove block is head node of the free list
  if ((prevBlock == NULL) && (nextBlock != NULL))
  {
    ((freeList *)nextBlock) -> prev = NULL;
    freeHead = nextBlock;
  }

  // case where the remove block is in the middle of the linked list
  else if ((prevBlock != NULL) && (nextBlock != NULL))
  {
    ((freeList *)prevBlock) -> next = nextBlock;
    ((freeList *)nextBlock) -> prev = prevBlock;
  }

  // case where the remove block is at the very end of the linked list
  else if ((prevBlock != NULL) && (nextBlock == NULL))
  {
    ((freeList *)prevBlock) -> next = NULL;
  }
}

void checkHeap(){
#ifdef USE_CHECKER
  
  void *startOfHeap = first_ever_page;
  int nextCounter = 0;
  int prevCounter = 0;

  int EON = 0;
  

  // checks the pages linked list next

  void *nextPage = startOfHeap;
  /* while(1) */
  /* { */

  /*   if (!EON) */
  /*   {      */
  /*     nextPage = ((pageList *)nextPage) -> next; */

  /*     if (nextPage == NULL) */
  /*     { */
  /* 	EON = 1; */
  /*     } */
      
  /*     nextCounter++;     */
  /*   } */
    
  /*   else if (EON) */
  /*   { */
  /*     nextPage = ((pageList *)nextPage) -> prev; */

  /*     if (nextPage == NULL) */
  /*     { */
  /* 	prevCounter++; */
  /* 	break; */
  /*     } */
      
  /*     prevCounter++;  */
  /*   } */
  /* } */

  printf("The number of next nodes is %d\n", nextCounter);
  printf("The number of prev nodes is %d\n", prevCounter);

  // ptr & 0xF should be zero if aligned
#endif
}
