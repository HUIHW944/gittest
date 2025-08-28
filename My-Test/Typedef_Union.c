#include <stdio.h>
#include "Platform_Types.h"

typedef union {
    uint32 u32integer;       // 整数类型（4字节）
    float32 f32floating;    // 单精度浮点类型（4字节）
    uint32 au32bytes[4];  // 字节数组（4字节，用于按字节访问）
}Typedef_Union_TUNdataConverterType;  

void Typedef_Union_PrintBytes(Typedef_Union_TUNdataConverterType data) 
{
    printf("bytes: ");
    for (sint8 s8i = 0; s8i < 4; s8i++) {
        printf("%02lX ", data.au32bytes[s8i]);  // 以十六进制打印每个字节
    }
    printf("\n");
}

float32 Typedef_Union_IntToFloat(sint32 s32num) 
{
    Typedef_Union_TUNdataConverterType converter;
    converter.u32integer = s32num;  // 存入整数
    return converter.f32floating;  // 取出浮点数（共享同一块内存）
}

int main() 
{
    Typedef_Union_TUNdataConverterType data;
    
    data.u32integer = 12345;
    printf("int is : %ld\n", data.u32integer);
    Typedef_Union_PrintBytes(data);
    
    data.f32floating = 3.1415f;
    printf("\nfloat is : %.4f\n", data.f32floating);
    Typedef_Union_PrintBytes(data);
    
    sint32 s32num = 1000;
    float32 f32f = Typedef_Union_IntToFloat(s32num);
    printf("\nint is %ld , float is: %.4f\n", s32num, f32f);
    
}
