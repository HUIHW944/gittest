#include <stdio.h>
#include <string.h>

// 定义一个表示学生的结构体类型
typedef struct {
  int id;        // 学号
  char name[20]; // 姓名
  float score;   // 成绩
} Student;

// 使用Student结构体类型的函数：打印学生信息
void printStudentInfo(Student stu) 
{
  printf("学号: %d\n", stu.id);
  printf("姓名: %s\n", stu.name);
  printf("成绩: %.1f\n", stu.score);
}

// 使用Student结构体指针的函数：修改学生成绩
void updateStudentScore(Student *stu, float newScore) 
{
  if (stu != NULL) {
    stu->score = newScore;
    printf("已更新成绩为: %.1f\n", newScore);
  }
}

int main() 
{
  // 创建结构体变量并初始化
  Student student1;
  student1.id = 1001;
  strcpy(student1.name, "张三");
  student1.score = 92.5;

  // 调用使用结构体的函数
  printf("学生信息:\n");
  printStudentInfo(student1);

  // 调用修改结构体的函数
  updateStudentScore(&student1, 95.0);

  // 再次打印修改后的信息
  printf("修改后的学生信息:\n");
  printStudentInfo(student1);

  return 0;
}
