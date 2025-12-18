#include <stdio.h>
#include "../Platform_Types.h"
void test(uint16 *a);
void test2(uint16 a);
int main(void)
{
    uint16 a = 10; //变量a，存放数据
    uint16 *p; //定义指针变量p(地址），存放变量的地址,（uint16 *）是数据类型，其实这行是定义p

    p = &a; //&a是a的地址，将地址存到指针变量p
    printf("a = %d\n", a);
    printf("*p = %d\n", *p);
    printf("p = %p\n", p);
    printf("&a = %p\n", &a);
    // a是变量值
    // *p是地址中对应的值=a的值
    // p是地址=&a
    
   *p = 20; //*p是p中存的值，通过直接修改指针里的值，间接修改a的值
    printf("a = %d\n", a);
    printf("*p = %d\n", *p);
    printf("p = %p\n", p);
    printf("&a = %p\n", &a);
    
    test(p);    //传入指针变量，修改指针变量指向的值
    test(&a);   //传入地址
  //test(*p);   //传入指针变量指向的值，对应不上
    test2(*p);  //传入指针变量指向的值
    test2(a);   //传入变量值
    
    uint16 b[3] = {1, 2, 3};
    p = b;         //数组名b就是数组首元素的地址
    p = &b[0];     //等价于上面一行
    test(p);       //传入指针变量，修改指针变量指向的值
    test(&b[0]);   //传入地址
    test2(*p);     //传入指针变量指向的值
    test2(b[0]);   //传入数组值
    return 0;
}

void test(uint16 *a) //传入指针变量
{
    *a = 30;
    printf("a = %d\n", *a);
}
void test2(uint16 a) //无法修改，只能读取
{
    a = 40;
    printf("a = %d\n", a); 
}