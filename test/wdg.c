#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include <stdio.h>

// 定义软件看门狗定时器句柄
TimerHandle_t xWatchdogTimer;

// 软件看门狗定时器回调函数
void vWatchdogTimerCallback(TimerHandle_t xTimer)
{
    // 这里可以执行更复杂的恢复操作，如复位系统等
    // 简单示例，打印提示信息
    printf("软件看门狗超时，系统可能出现异常！\n");
}

// 喂狗函数
void feedWatchdog(void)
{
    // 重新启动软件看门狗定时器，重置计时
    xTimerReset(xWatchdogTimer, 0);
}

// 模拟应用任务，在任务中定期喂狗
void vApplicationTask(void *pvParameters)
{
    (void)pvParameters;
    while(1)
    {
        // 模拟执行一些任务
        vTaskDelay(pdMS_TO_TICKS(500)); 

        // 进行喂狗操作
        feedWatchdog(); 

        // 这里可以根据实际情况，偶尔不喂狗，模拟系统异常
        // 比如每10次循环，有1次不喂狗
        static int count = 0;
        count++;
        if(count % 10 == 0)
        {
            // 不喂狗，模拟系统异常
            vTaskDelay(pdMS_TO_TICKS(2000)); 
        }
    }
}

int main(void)
{
    // 创建软件看门狗定时器
    xWatchdogTimer = xTimerCreate(
                        "WatchdogTimer",      // 定时器名字
                        pdMS_TO_TICKS(1500),  // 定时周期，1500毫秒
                        pdFALSE,              // 一次性定时器，超时后不自动重启
                        NULL,                 // 没有传递给回调函数的参数
                        vWatchdogTimerCallback// 回调函数
                    );

    if(xWatchdogTimer != NULL)
    {
        // 启动软件看门狗定时器
        xTimerStart(xWatchdogTimer, 0); 

        // 创建应用任务
        xTaskCreate(
                        vApplicationTask,     // 任务函数
                        "AppTask",            // 任务名字
                        configMINIMAL_STACK_SIZE, // 任务堆栈大小
                        NULL,                 // 没有传递给任务函数的参数
                        tskIDLE_PRIORITY + 1, // 任务优先级
                        NULL                  // 任务句柄
                    );

        // 启动FreeRTOS调度器
        vTaskStartScheduler(); 
    }

    // 如果程序执行到这里，说明调度器启动失败
    while(1); 
}