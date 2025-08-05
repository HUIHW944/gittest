/*
#include <stdio.h>

int main(void) 
{
    for (int i = 0; i < 5; i++) {
        printf("i = %d\n", i);
    }
    return 0;
}

*/


#include <stdio.h>

static int count = 0;

void counter(void) 
{
    count++;
    printf("%d\n", count);
}

int main(void) 
{
    counter();   /* 输出 1 */
    counter();   /* 输出 2 */
    counter();   /* 输出 3 */
    return 0;
}
