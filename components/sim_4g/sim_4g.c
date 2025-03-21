#include "sim_4g.h"
#include "uart_driver.h"
#include "driver/uart.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "SIM_4G";

void sim_4g_send_command(const char *cmd)
{
    int len = strlen(cmd);
    uart_write_bytes(UART_NUM_1, cmd, len);
    uart_write_bytes(UART_NUM_1, "\r\n", 2);
    ESP_LOGI(TAG, "Sent command: %s", cmd);

    // Đọc phản hồi từ module SIM
    uint8_t data[100];  // Buffer để chứa dữ liệu nhận được
    int length = uart_read_bytes(UART_NUM_1, data, sizeof(data) - 1, pdMS_TO_TICKS(500));
    if (length > 0) {
        data[length] = '\0';  // Đảm bảo kết thúc chuỗi hợp lệ
        ESP_LOGI(TAG, "Received: %s", (char *)data);
    } else {
        ESP_LOGW(TAG, "No response from SIM module");
    }
}

