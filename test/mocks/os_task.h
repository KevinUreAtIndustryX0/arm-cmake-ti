#ifndef MOCK_OS_TASK_H
#define MOCK_OS_TASK_H

#include "fff.h"
#include <stdint.h>

typedef long BaseType_t;
typedef void (*TaskFunction_t)( void * );
typedef unsigned long UBaseType_t;
typedef void * TaskHandle_t;
#define pdTRUE			( ( BaseType_t ) 1 )
DECLARE_FAKE_VALUE_FUNC(BaseType_t, xTaskCreate, TaskFunction_t, const char* const, const uint16_t, void * const, UBaseType_t, TaskHandle_t * const)
DECLARE_FAKE_VOID_FUNC(vTaskStartScheduler)
#endif //MOCK_OS_TASK_H
