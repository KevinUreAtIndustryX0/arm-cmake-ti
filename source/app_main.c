#include "app_main.h"
#include "FreeRTOS.h"
#include "os_task.h"
#include "HL_gio.h"
#include "common.h"

xTaskHandle xTask1Handle;
xTaskHandle xTask2Handle;

void vTask1(void *pvParameters)
{
    for(;;)
    {
        gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
        vTaskDelay(100);
    }
}
/* Task2 */
void vTask2(void *pvParameters)
{
    for(;;)
    {
        gioSetBit(gioPORTB, 7, gioGetBit(gioPORTB, 7) ^ 1);
        vTaskDelay(500);
    }
}

void app_main(void){
    gioInit();


    /* Create Task 1 */
    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
    {
        /* Task could not be created */
        return;
    }

    /* Create Task 2 */
    if (xTaskCreate(vTask2,"Task2", configMINIMAL_STACK_SIZE, NULL, 1, &xTask2Handle) != pdTRUE)
    {
        /* Task could not be created */
        return;
    }

    /* Start Scheduler */
    vTaskStartScheduler();

    return;
    /* Run forever */
}

void MOCKABLE block_main_thread_forever() {
    while(1);
}