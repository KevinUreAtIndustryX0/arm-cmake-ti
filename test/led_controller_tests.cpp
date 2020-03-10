#include "gtest/gtest.h"
extern "C" {
    #include "app_main.h"
    #include "FreeRTOS.h"
    #include "HL_gio.h"
    #include "os_task.h"

}
#include "fff.h"

DEFINE_FFF_GLOBALS;
DEFINE_FAKE_VOID_FUNC(vTaskDelay, int);
DEFINE_FAKE_VOID_FUNC(gioSetBit, gioPORT_t *, uint32, uint32);
DEFINE_FAKE_VALUE_FUNC(uint32, gioGetBit, gioPORT_t *, uint32);
DEFINE_FAKE_VOID_FUNC(gioInit);
DEFINE_FAKE_VALUE_FUNC(BaseType_t, xTaskCreate, TaskFunction_t, char*, uint16_t, void *, UBaseType_t, TaskHandle_t *);
DEFINE_FAKE_VOID_FUNC(vTaskStartScheduler);

TEST(MyTestSuite, set_color_works_for_color){
    app_main();

}

