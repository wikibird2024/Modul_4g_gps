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
}

