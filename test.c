#include <stdio.h>
#include <stdlib.h>

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
int add_int(int a, int b) {
    return a + b;
}
double add_float(double a, double b) {
    return a + b;
}
int main() {
    int int_result = add_int(1, 2);
    float float_result = add_float(1.5, 2.5); 
    printf("result are %d and %f\n", int_result, float_result);
    return 0;
}