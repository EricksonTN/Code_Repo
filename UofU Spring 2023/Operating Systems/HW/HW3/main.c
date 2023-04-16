#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <sys/mman.h>


#include <sys/types.h>
#include <unistd.h>

// Format of an ELF executable file


#define ELF_MAGIC 0x464C457FU  // "\x7FELF" in little endian

// File header
struct elfhdr {
  unsigned int magic;  // must equal ELF_MAGIC
  unsigned char elf[12];
  unsigned short type;
  unsigned short machine;
  unsigned int version;
  unsigned int entry;
  unsigned int phoff;
  unsigned int shoff;
  unsigned int flags;
  unsigned short ehsize;
  unsigned short phentsize;
  unsigned short phnum;
  unsigned short shentsize;
  unsigned short shnum;
  unsigned short shstrndx;
};

// Program section header
struct proghdr {
  unsigned int type;
  unsigned int off;
  unsigned int vaddr;
  unsigned int paddr;
  unsigned int filesz;
  unsigned int memsz;
  unsigned int flags;
  unsigned int align;
};

// Values for Proghdr type
#define ELF_PROG_LOAD           1

// Flag bits for Proghdr flags
#define ELF_PROG_FLAG_EXEC      1
#define ELF_PROG_FLAG_WRITE     2
#define ELF_PROG_FLAG_READ      4

int main(int argc, char* argv[]) {
    struct elfhdr elf;
    struct proghdr ph;
    int (*sum)(int a, int b);
    void *entry = NULL;
    int ret; 

    /* Add your ELF loading code here */

    //Open elf file
    int fd = open(argv[1], O_RDONLY);

    //read fromthe elf file to propogate the elfhdr info.
    read(fd, &elf, sizeof(elf));    
    
    //Loop to find the program with the EXEC flag.
    for (int i = 0; i < elf.phnum; i++)
    {
      //Search through all the program headers for the specific type of EXEC.
      lseek(fd, elf.phoff + (i * sizeof(struct proghdr)), SEEK_SET);
      read(fd, &ph, sizeof(struct proghdr));

      if (ph.type == ELF_PROG_FLAG_EXEC)
      {
        // Allocate block of memory for the program data.
        void *src = mmap(NULL, ph.memsz, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);

        //Move to the location of the program
        lseek(fd, ph.off, SEEK_SET);

        //Fill the recently allocated memory block with the program data.
        read(fd, src, ph.memsz);

        //Set a pointer to the beginning of the program.
        entry = src + (elf.entry - ph.vaddr);
      }
    }

    if (entry != NULL) {
        sum = entry; 
        ret = sum(1, 2);
        printf("sum:%d\n", ret); 
    };


}




