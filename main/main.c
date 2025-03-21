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
     uart_driver_init();
    vTaskDelay(pdMS_TO_TICKS(1000)); // Đợi 1 giây trước khi gửi lệnh

    sim_4g_send_command("AT");       // Gửi lệnh kiểm tra
    vTaskDelay(pdMS_TO_TICKS(1000)); // Chờ phản hồi
    sim_4g_send_command("AT+CGMI");  // Gửi lệnh đọc nhà sản xuất
    sim_4g_send_command("AT+CGMR");    // Lấy phiên bản firmware
    vTaskDelay(pdMS_TO_TICKS(1000));

    sim_4g_send_command("AT+CGSN");    // Lấy số IMEI
    vTaskDelay(pdMS_TO_TICKS(1000));

    sim_4g_send_command("AT+CSQ");     // Kiểm tra cường độ tín hiệu
    vTaskDelay(pdMS_TO_TICKS(1000));

    sim_4g_send_command("AT+COPS?");   // Kiểm tra nhà mạng
    vTaskDelay(pdMS_TO_TICKS(1000));

    sim_4g_send_command("AT+QGPS=1");  // Bật GPS
    vTaskDelay(pdMS_TO_TICKS(2000));   // Chờ GPS khởi động

    sim_4g_send_command("AT+QGPSLOC?"); // Lấy vị trí GPS hiện tại
    vTaskDelay(pdMS_TO_TICKS(1000));                                 // 
}

