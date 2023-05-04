//mm_init for first page


// Set up Initial heap space (?) First Page?
  
  /* void * p = mem_map(mem_pagesize()); */
  /* if (p == NULL) */
  /* { */
  /*   return -1;					 */
  /* } */

  /* // Set the PageList */
  /* p = p + sizeof(size_t); // PADDING FOR ALIGNMENT */
  /* ((pageList *)p) -> prev = NULL; */
  /* ((pageList *)p) -> next = NULL; */

  /* current_avail = p + (sizeof(pageList)) + sizeof(size_t); //Places the pointer after the padding and page traversals */

  /* // Set the Header */
  /* p = p + sizeof(pageList); */
  /* PUT((block_header *)p, PACK(4064, 2)); */

  /* current_avail = p + sizeof(size_t); //Places the pointer after the Header */
  
  /* // Set the Footer */
  /* p = p + (507 * sizeof(size_t)); */
  /* (*(term *)p) = 4064; */
  
  /* //PUT((block_footer *)p, PACK(4064, 2));  */
  
  /* // Set the Terminating block */
  /* p = p + sizeof(size_t); */
  /* (*(term *)p) = 0; */

  
  /* // Set the current available size */
  /* current_avail_size = mem_pagesize() - sizeof(pageList) - (3 * sizeof(size_t)); */

//PRINT STATEMENT SET_ALLOCATED

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


/* if(firstpage) */
  /* { */
  /*   firstpage = 0;   */
  /*   current_avail = p + sizeof(PADDING) + sizeof(pageList) + sizeof(block_header); */
  /*   current_avail_size = INIT_BLOCK_SIZE(pageLength); */
  /* } */

// ORIGINAL IMPLEMENTATION OF MALLOC

if (current_avail_size < newsize) {
    current_avail_size = PAGE_ALIGN(newsize);
    current_avail = mem_map(current_avail_size);
    if (current_avail == NULL)
      return NULL;
  }

  p = current_avail;
  current_avail += newsize;
  current_avail_size -= newsize;
