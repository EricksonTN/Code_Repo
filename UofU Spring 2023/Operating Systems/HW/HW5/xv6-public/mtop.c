#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) 
{
    printf(1, "available memory: %d\n", memtop());
    exit();
} 