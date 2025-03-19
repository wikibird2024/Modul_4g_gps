#include <stdio.h>
#include "sim_4g.h"
#include "uart_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    uart_driver_init();
    vTaskDelay(pdMS_TO_TICKS(1000)); // Đợi 1 giây trước khi gửi lệnh

    sim_4g_send_command("AT");
}

