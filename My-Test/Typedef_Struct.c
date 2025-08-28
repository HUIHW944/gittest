#include <stdio.h>
#include <string.h>
#include "Platform_Types.h"
typedef struct {
  uint16 u16id;
  sint8 as8name[20];
  float32 f32score;
} Typedef_Struct_TSstudentType;

void Typedef_Struct_PrintStudentInfo(Typedef_Struct_TSstudentType stu) 
{
  printf("id: %d\n", stu.u16id);
  printf("name: %s\n", stu.as8name);
  printf("score: %.1f\n", stu.f32score);
}

void Typedef_Struct_UpdateStudentScore(Typedef_Struct_TSstudentType *stu, float32 f32newScore) 
{
  if (stu != NULL) {
    stu->f32score = f32newScore;
    //printf("new score: %.1f\n", f32newScore);
  }
}

int main() 
{
  Typedef_Struct_TSstudentType student1;
  student1.u16id = 1001;
  strcpy((char*)student1.as8name, "Zhang San"); /* strcpy apply char not unsigned char */
  student1.f32score = 92.5;

  printf("student infomation:\n");
  Typedef_Struct_PrintStudentInfo(student1);

  Typedef_Struct_UpdateStudentScore(&student1, 95.0);

  printf("modify student infomation:\n");
  Typedef_Struct_PrintStudentInfo(student1);

  return 0;
}
