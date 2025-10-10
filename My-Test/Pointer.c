#include <stdio.h>
#include "Platform_Types.h"

uint8 Pointer_Main(void)
{
    uint8 u8a = 10;
    uint16 *pu16addr; //指针变量p，存放变量的地址

    pu16addr = &u8a; //&a是a的地址，将地址存到指针变量p
    printf("a = %d\n", u8a);
    printf("*p = %d\n", *pu16addr);
    printf("p = %p\n", pu16addr);
    printf("&a = %p\n", &u8a);

   *pu16addr = 20; //*p是p中存的值，通过直接修改指针里的值，间接修改a的值
    printf("a = %d\n", u8a);
    printf("*p = %d\n", *pu16addr);
    printf("p = %p\n", pu16addr);
    printf("&a = %p\n", &u8a);

    return 0; 
}