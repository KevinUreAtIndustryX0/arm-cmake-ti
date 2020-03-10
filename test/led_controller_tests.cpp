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
FAKE_VOID_FUNC(block_main_thread_forever);

class AppTestFixture : public testing::Test {
    void TearDown() override{
        RESET_FAKE(vTaskDelay);
        RESET_FAKE(gioSetBit);
        RESET_FAKE(gioGetBit);
        RESET_FAKE(gioInit);
        RESET_FAKE(xTaskCreate);
        RESET_FAKE(vTaskStartScheduler);
        RESET_FAKE(block_main_thread_forever);
    }
};

TEST_F(AppTestFixture, calls_start_scheduler_if_tasks_created_successfully){
    xTaskCreate_fake.return_val = 1;

    app_main();

    EXPECT_EQ(vTaskStartScheduler_fake.call_count, 1);
    EXPECT_EQ(block_main_thread_forever_fake.call_count, 1);
}


TEST_F(AppTestFixture, does_not_call_start_scheduler_if_tasks_did_not_create){
    xTaskCreate_fake.return_val = 0;

    app_main();

    EXPECT_EQ(vTaskStartScheduler_fake.call_count, 0);
    EXPECT_EQ(block_main_thread_forever_fake.call_count, 1);
}

