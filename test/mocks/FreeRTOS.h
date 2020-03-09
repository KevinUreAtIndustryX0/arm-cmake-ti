#ifndef MOCK_FREERTOS_H
#define MOCK_FREERTOS_H
#include "fff.h"
#include <stdint.h>

#define xTaskHandle TaskHandle_t
#define configMINIMAL_STACK_SIZE	  ( ( uint16_t ) 128 )
DECLARE_FAKE_VOID_FUNC(vTaskDelay, int)
#endif //MOCK_FREERTOS_H
