#include <stdio.h>

int main ()
{
  unsigned int bits = 0xAABBCCDD;

  unsigned char MSB = bits >> 24;

  printf("%x\n", MSB);

  unsigned char shifted = bits >> 20;

  printf("%x\n", shifted);

  shifted = shifted & 0x3f;

  printf("%x\n", shifted);

  unsigned short answer = bits >> 16;

  printf("%x\n", answer);

  unsigned char ans = bits & 0x3;

  printf("%x\n", ans);

  unsigned char ans2 = (bits >> 7) & 0x7;

  printf("%x\n", ans2);

  
  

}
