#include <stdio.h>
#include "../Platform_Types.h"
void test(uint16 *pu16a);
void test2(uint16 pu16a);
int main(void)
{
    uint16 u16a = 10; //变量a，存放数据
    uint16 *pu16addr; //指针变量p(地址），存放变量的地址

    pu16addr = &u16a; //&a是a的地址，将地址存到指针变量p
    printf("a = %d\n", u16a);
    printf("*p = %d\n", *pu16addr);
    printf("p = %p\n", pu16addr);
    printf("&a = %p\n", &u16a);
    // a是变量值
    // *p是地址中对应的值=a的值
    // p是地址=&a
    
   *pu16addr = 20; //*p是p中存的值，通过直接修改指针里的值，间接修改a的值
    printf("a = %d\n", u16a);
    printf("*p = %d\n", *pu16addr);
    printf("p = %p\n", pu16addr);
    printf("&a = %p\n", &u16a);
    
    test(pu16addr); //传入指针变量，修改指针变量指向的值
                    //test(*pu16addr),传入指针变量指向的值，对应不上
    test(&u16a);   //传入地址
    test2(*pu16addr); //传入指针变量指向的值
    
    return 0;
}

void test(uint16 *pu16a) //传入指针变量
{
    *pu16a = 30;
    printf("a = %d\n", *pu16a);
}
void test2(uint16 pu16a) //无法修改，只能读取
{
    printf("a = %d\n", pu16a); 
}