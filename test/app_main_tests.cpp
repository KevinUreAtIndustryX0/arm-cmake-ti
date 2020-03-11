#include "gtest/gtest.h"
extern "C" {
    #include "app_main.h"
    #include "FreeRTOS.h"
    #include "HL_gio.h"
    #include "os_task.h"
}
#include "fff.h"

#define task_success 1
#define task_failure 0

DEFINE_FFF_GLOBALS;
DEFINE_FAKE_VOID_FUNC(vTaskDelay, int);
DEFINE_FAKE_VOID_FUNC(gioSetBit, gioPORT_t *, uint32, uint32);
DEFINE_FAKE_VALUE_FUNC(uint32, gioGetBit, gioPORT_t *, uint32);
DEFINE_FAKE_VOID_FUNC(gioInit);
DEFINE_FAKE_VALUE_FUNC(BaseType_t, xTaskCreate, TaskFunction_t, char*, uint16_t, void *, UBaseType_t, TaskHandle_t *);
DEFINE_FAKE_VOID_FUNC(vTaskStartScheduler);

class AppTestFixture : public testing::Test {
    void SetUp() override{ }

    void TearDown() override{
        RESET_FAKE(vTaskDelay);
        RESET_FAKE(gioSetBit);
        RESET_FAKE(gioGetBit);
        RESET_FAKE(gioInit);
        RESET_FAKE(xTaskCreate);
        RESET_FAKE(vTaskStartScheduler);
    }
};

TEST_F(AppTestFixture, calls_start_scheduler_if_tasks_created_successfully){
    xTaskCreate_fake.return_val = task_success;

    app_main();

    EXPECT_EQ(vTaskStartScheduler_fake.call_count, 1);
}

TEST_F(AppTestFixture, does_not_call_start_scheduler_if_tasks_did_not_create){
    xTaskCreate_fake.return_val = task_failure;

    app_main();

    EXPECT_EQ(vTaskStartScheduler_fake.call_count, 0);
}
