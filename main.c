#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>   
typedef struct {
    uint16_t freq_hz;   
    uint8_t duty_pct;  
} blink_param_t;


void GPIO_Config(void);
void USART1_Config(void);
void USART1_SendString(char *str);
void vTaskBlink(void *pvParameters);
void vTaskGenerate(void *pvParameters);


QueueHandle_t xBlinkQueue;  


int main(void)
{
    SystemInit();          
    GPIO_Config();         
    USART1_Config();       

   
    xBlinkQueue = xQueueCreate(5, sizeof(blink_param_t));

   
    xTaskCreate(vTaskBlink, "Blink", 128, NULL, 2, NULL);
    xTaskCreate(vTaskGenerate, "Gen", 128, NULL, 1, NULL);

    
    srand(1234);

    
    vTaskStartScheduler();

    while(1);
}


void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;            
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_13);                    
}


void USART1_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}


void USART1_SendString(char *str)
{
    while(*str)
    {
        USART_SendData(USART1, *str++);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}


void vTaskBlink(void *pvParameters)
{
    blink_param_t params = {1, 50}; 
    while(1)
    {
        
        xQueueReceive(xBlinkQueue, &params, 0);

        uint32_t period_ms = 1000 / params.freq_hz;                 
        uint32_t on_time_ms = (period_ms * params.duty_pct) / 100;  
      
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        vTaskDelay(pdMS_TO_TICKS(on_time_ms));

       
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        vTaskDelay(pdMS_TO_TICKS(period_ms - on_time_ms));
    }
}


void vTaskGenerate(void *pvParameters)
{
    blink_param_t params;
    uint16_t freq_list[] = {1, 2,4,8, 10}; 
    uint8_t i = 0;
char msg[50];

    while(1)
    {
        params.freq_hz = freq_list[i];
        params.duty_pct = (rand() % 81) + 10; 

        
        xQueueSend(xBlinkQueue, &params, 0);

       
        sprintf(msg, "Tanso: %d Hz | Duty: %d%%\r\n", params.freq_hz, params.duty_pct);
        USART1_SendString(msg);

        
        i = (i + 1) % (sizeof(freq_list)/sizeof(freq_list[0]));

        
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
