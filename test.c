#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
int main(void) {
    int **pVar =(int **)(&pVar);
    printf("%x\n", pVar);
    printf("%x\n",&pVar);
    printf("%x\n", *pVar);
    printf("%x\n", **pVar);system("pause");
    return 0;
}

#define TOUCH_PANEL_DRIVER 1
#ifdef TOUCH_PANEL_DRIVER
    void tp_driver(void) {
        printf("tp_driver_call\n");
    }
#endif
int TP_DRIVER(void) {
    #ifdef TOUCH_PANEL_DRIVER
        tp_driver();
    #endif
    return 0;
}

#define FUNc(a,b) do{((a)=(a)*(b));a++;} while()
int a = 10;
int MUITIPLICATION(void) {
    FUNC(a, 10);
    printf("a is %d\n",a);
    return 0;
}
*/

/*
struct abc {
    int a;
    char b;
    float c;
}c;

union def {
    int d;
    char e;
    float f;
}f;

enum ghi {
    g = 100,
    H = 200,
    I = 300
};

//数据类型别名
typedef struct ABC {
    int a;
    char b;
    float c;
} ABC_t;
typedef int len_t;
typedef unsigned char unit8_t;
typedef void (*func)(void);//函数指针类型

int main(void) {
    c.a = 10;
    c.b = 'b';
    c.c = 2.5;
    printf("a is %d,b is %c,c is %f\n",c.a,c.b,c.c);
    int z = 10;
    printf("z is %d,addr of z is %p\n"， Z, &z);

    f.d = 2;
    printf("d is %d\n",f.d);
    f.e = 'f';
    printf("e is %c\n",f.e);
    f.f = 5.0;
    printf("f is %f\n",f.f);
    printf("g is %d,h is %d,i is %d\n",g,H,I);

    int m = 10;
    int *p = &m;
    printf("symbol visit:A is %d\n",m);
    printf("addr visit:A is %d\n",*p);

    len_t A = 100;
    unit8_t B;
    ABC_t C;
    printf("A is %d,B is %d,C is %d\n",A,B,C);
    return 0;
}

static int a =10;//在别的文件中无法extern，只作用当前文件
int func1(void) {
    static int b = 10;//执行完不销毁
    b++;
    return b;
}

int func2(void) {
    int c = 20;//执行完销毁
    c++;
    return c;
}
int main(void) {
    const int z = 10;
    int const x = 20;
    //z=20//无法修改
    printf("a = %d,b = %d,c = %d\n", a, func1(), func2());
    printf("a = %d,b = %d,c = %d\n", a, func1(), func2());
    printf("z = %d,x = %d\n", Z， x);
}

int main() {
    for (int i = θ; i < 5; i++)
    if (1 == i) {
    continue;//直接开启下一次循环
    }
     else if (4 == i) {
    break;//直接跳出本循环
    }
    printf("i = %d\n", i);
    return 0;
}
*/

/*
struct abc {
    int a;
    int b;
    int c;
}d;
int main() {
    d.a = 1;
    d.b = 2;
    d.c = 3;
    struct abc *p = &d;
    printf("%d %d %d\n", d.a, d.b, d.c);
    printf("%d %d %d\n", p->a, p->b, p->c);

    int a[3] = {1,2,3};
    int *p1 = &a[0];
    printf("%d %d %d\n", a[0], a[1], a[2]);
    printf("%d %d %d\n",(*p1)，(*(p1+1))，(*(p1+2)));
    return 0;
}
*/

/*
int func(int *a, int *b) {
    if (!a || !b) {
    return -1;
    }
    *a = 100 * 2;
    *b = 200 * 2;
    return 0;
}
void main(void) {
    int ret1 = 0;
    int ret2 = 0;
    func(&ret1,&ret2);
    printf("%d, %d\n", ret1, ret2);
}
*/

/*
   int add_int(int a, int b) {
   return a + b;
   }
   double add_double(double a, double b) {
   return a + b;
   }
   int main() {
   int int_result = add_int(1, 2);
   float float_result = add_double(1.5, 2.5); 
   printf("results are %d and %f\n", int_result, float_result);
   return 0;
   }
 */

/*
   int global_inited = 10;//全局初始化变量    
   int global_uninited;// 全局未初始化变量   
   static int static_global_inited = 10;// 静态全局初始化变量   
   static int static_global_uninited;// 静态全局未初始化变量   
   int main(void) {
   int local_var = 20;  //局部变量 
   char *str = "Hello"; //字符串变量
   static int static_local_inited = 30; //静态局部c初始化量
   static int static_local_uninited;//静态局部未初始化变量
   int *heap_var = (int *)malloc(sizeof(int)); //动态分配内存
*/
/*
int global_inited = 10;// 全局初始化变量  
int global_uninited;//全局未初始化变量  
static int static_global_inited = 10;// 静态全局初始化变量   
static int static_global_uninited;// 静态全局未初始化变量   
int main(void) {
    int local_var = 20;  //局部变量 
    char *str = "Hello"; //字符串变量
    static int static_local_inited = 30; //静态局部c初始化量
    static int static_local_uninited;//静态局部未初始化变量
    int *heap_var = (int *)malloc(sizeof(int)); //动态分配内存

    printf("address of code:                       %p\n", main);//代码段（只读）
    printf("address of str:                        %p\n", str);//只读数据段

    printf("address of global_inited:              %p\n", (void *)&global_inited);//全局数据初始化段（DATA）
    printf("address of static_global_inited:       %p\n", (void *)&static_global_inited);//DATA
    printf("address of static_local_inited:        %p\n", (void *)&static_local_inited);//DATA
    printf("address of global_uninited:            %p\n", (void *)&global_uninited);//全局数据未初始化段（BSS）
    printf("address of static_global_uninited:     %p\n", (void *)&static_global_uninited);//BSS
    printf("address of static_local_uninited:      %p\n", (void *)&static_local_uninited);//BSS
    printf("address of global_inited:    %p\n", (void *)&global_inited);//全局数据初始化段（DATA）
    printf("address of static_var:       %p\n", (void *)&static_global_inited);//DATA
    printf("address of static_var:       %p\n", (void *)&static_local_inited);//DATA
    printf("address of global_uninited:  %p\n", (void *)&global_uninited);//全局数据未初始化段（BBS）
    printf("address of static_var:       %p\n", (void *)&static_global_uninited);//BBS
    printf("address of static_var:       %p\n", (void *)&static_local_uninited);//BBS

    printf("address of heap_var:                   %p\n", (void *)heap_var);//堆空间，往上生长
    printf("address of local_var:                  %p\n", (void *)&local_var);//栈空间，往下生长
    free(heap_var); //释放动态分配的内存
    return 0;
}
   */

/* int main () {
   int a = 0x12345678;
   int b = 0x10203040;
   int *p1 = &a;//整个地址打印
   char *p2 = (char *)&b;//从地址的低位开始，一位一位打印
   printf("the p1+1 is %x, %x, %x, %x\n", *p1, *(p1+1), p1[1], *p1+1);
   printf("the p2+1 is %x, %x, %x, %x\n", *p2, *(p2+1), p2[1], *p2+1);
   } */

/* void func(void) {
   printf("hello func\n");
   }
   int main(void) {
   printf("func addr %p\n",func);
   void (*p)(void)  = func;// try to access
   p();//=func();
   int *p1 = (int *)func;//try to read
   printf("func addr %d\n",*p1); 
 *p1 = 1;//try to write
 printf("%s addr: %p\n", "hello", "hello");
//const char *s = "hallo";//=char *s
//s[1] = 'e';
return 0;
} */

/*
int a = 10;
int b;

int func1(void) {
    static int d = 10;
    b++;
    return ++d;
}

int func2(void) {
    int *heap_var = (int *)malloc(sizeof(int));
    if (NULL == *heap_var) {
      return -1;
    }
    *heap_var = 10;
    printf("heap addr = %p,heap_var = %d\n", heap_var, *heap_var);
    free(heap_var);
}

char *func3(void) {
    char *s = "hello";
    char buff1[] = "hello";
    static char buff2[] = "hello";
    printf("func3 :%s\n",s);
    printf("func3 :%s\n",buff1);
    printf("func3 :%s\n",buff2);
    return buff2;
}

int main(void) {
    static int c;
    printf("%p, %p, %p\n",&a, &b, &c);
    a = 20;
    b = 30;
    c = 40;
    printf("a = %d,b = %d,c = %d\n", a, b, c);//data bss 可读可写
    printf("b = %d,d = %d,d = %d\n", b, func1(), func1());

    func2();

    char *p = func3();
    printf("main : %s\n",p);
    return 0; 
}*/

/*
   struct abc {
   int a;
   int b;
   char c;
   }data;
   int arr[3] = {4,5,6};
   int main(void) {
   data.a = 1;
   data.b = 2;
   data.c = 3;
   printf("size of struct abc = %lld\n",sizeof(struct abc));
   struct abc *p = &data;
   printf("a=%d,b=%d,c=%d\n",p->a,p->b,p->c);
   int *p1 = (int *)&data;
   printf("struct p1[0] = %d\n", p1[0]);
   printf("struct p1[1] = %d\n", p1[1]);
   printf("struct p1[2] = %d\n", p1[2]);
   int *p2 = arr;
   printf("struct p2[0] = %d\n", p2[0]);
   printf("struct p2[1] = %d\n", p2[1]);
   printf("struct p2[2] = %d\n", p2[2]);
   return 0;
   }
   */


/*
   struct abc {
   int a;
   int b;
   char c;
   }data;
   void find_struct(int *member) {
   unsigned long offset = 0;
   struct abc* p = NULL;
   offset = (uintptr_t)&((struct abc*)0)->a;
   printf("member offset is %ld\n",offset);
   p = (struct abc*)((char *)member - offset);
   printf("a=%d,b=%d,c=%d\n",p->a,p->b,p->c);
   }
   int main(void) {
   data.a = 1;
   data.b = 2;
   data.c = 3;
   struct abc *p = &data;
   printf("a=%d,b=%d,c=%d\n",p->a,p->b,p->c);
   find_struct(&data.a);
   }
   */
/*
int main(void) {
    int array[] = {1,2,3,4,5};
    int *p = array;//int *p = &array[0]
    p = p+2;//p[2]
    printf("*p = %d \n",*p);
    p++;//p[3]
    printf("*p = %d \n",*p);
    return 0;
}
*/

/*
   int main(void) {
   const char *arr[3] = {"welcome","to","SW"};
   const char **s = arr;
   printf("%s:%p\n",s[0],s[0]);
   printf("%s:%p\n",s[1],s[1]);
   printf("%s:%p\n",s[2],s[2]);
   printf("&s[0]:%p\n",&s[0]);
   printf("&s[1]:%p\n",&s[1]);
   printf("&s[2]:%p\n",&s[2]);
   return 0;
   }
   */

/* int main() {
   int (*myshow)(const char *,...);
   printf("hello\n");
   printf("the printf is %p\n",printf);
   myshow = printf;
   myshow("====\n");
   printf("the myshow is %p\n",myshow);
   return 0;
   } */

int add(int a, int b) { 
    return a + b; 
}
int sub(int a, int b) { 
    return a - b; 
}
void process(int (*func)(int, int), int a, int b) {
    int result = func(a, b);  // 调用传入的函数
    printf("Result: %d\n", result);
}
int main(void) {
    process(add, 3, 4);
    process(sub, 3, 4);
}
