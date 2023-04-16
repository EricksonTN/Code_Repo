#include "console.h"

int ptd[1024] __attribute__((aligned (4096)));
int pte1[1024] __attribute__((aligned (4096)));
int pte2[1024] __attribute__((aligned (4096)));

static inline void lcr3(unsigned int val)
{   
  asm volatile("movl %0,%%cr3" : : "r" (val));
}

static inline void halt(void)
{
    asm volatile("hlt" : : );
}

int main(void)
{
    int i; 
    int sum = 0;

    // Initialize the console
    uartinit(); 

    printk("Hello from C\n");

    // Create your page table here

    ptd[0] = (int) (&pte1) | 0b11;
    ptd[1] = (int) (&pte2) | 0b11;
    
    for (int i = 0; i < 1024; i++)
    {
        pte1[i] = (int)(4096 * i) | 0b11;
    }

    int shift = 1024 * 4096;

    for (int j = 0; j < 1024; j++)
    {
        pte2[j] = (int)(shift + (4096 * j)) | 0b11;
    }

    lcr3((unsigned int) &ptd);

    for (i = 0; i < 32 /*64*/; i++) {
        int *p = (int *)(i * 4096 * 32);
        sum += *p; 
                
        printk("page\n"); 
    }
    halt(); 
    return sum; 
}


