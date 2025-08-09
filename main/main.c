#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sim_4g.h"
#include "uart_driver.h"
#include <stdio.h>
#include <string.h>

// Thời gian chờ giữa các lệnh (đơn vị: ms)
#define SHORT_DELAY_MS 500
#define MEDIUM_DELAY_MS 5000
#define LONG_DELAY_MS 15000
#define GPS_FIX_DELAY_MS 180000 // 3 phút là đủ cho lần đầu tiên

// Địa chỉ APN cho nhà mạng Viettel
#define APN_VIETTEL "v-internet"

void app_main(void) {
  printf("--- Khoi tao he thong va module SIM 4G/GPS ---\n");

  // 1. Khoi tao UART
  uart_driver_init();
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));

  // 2. Gui cac lenh AT co ban de kiem tra module
  printf("\n--- Kiem tra ket noi va thong tin module ---\n");
  sim_4g_send_command("AT");
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));
  sim_4g_send_command("ATI");
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));
  sim_4g_send_command("AT+CGSN");
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));
  sim_4g_send_command("AT+CPIN?");
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));
  sim_4g_send_command("AT+CSQ");
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));
  sim_4g_send_command("AT+CREG?");
  vTaskDelay(pdMS_TO_TICKS(MEDIUM_DELAY_MS));

  // 3. Cau hinh APN va kich hoat ket noi du lieu
  printf("\n--- Cau hinh APN va ket noi du lieu ---\n");
  char apn_cmd[64];
  sprintf(apn_cmd, "AT+CGDCONT=1,\"IP\",\"%s\"", APN_VIETTEL);
  sim_4g_send_command(apn_cmd);
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));

  sim_4g_send_command("AT+QIACT=1");
  vTaskDelay(pdMS_TO_TICKS(LONG_DELAY_MS)); // Chờ 15s
  sim_4g_send_command("AT+QIACT?");         // Kiểm tra trạng thái PDP

  // Nếu lệnh AT+QIACT=1 thất bại, thử lại với cấu hình khác
  if (strstr(apn_cmd, "ERROR") || strstr(apn_cmd, "+CME ERROR")) {
    printf("AT+QIACT=1 that bai, thu AT+QICSGP=1,1\n");
    sim_4g_send_command("AT+QICSGP=1,1,\"v-internet\"");
    vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));
    sim_4g_send_command("AT+QIACT=1");
    vTaskDelay(pdMS_TO_TICKS(LONG_DELAY_MS));
    sim_4g_send_command("AT+QIACT?");
  }

  // 4. Bat GPS va lay thong tin
  printf("\n--- Bat GPS va lay toa do (dat module ngoai troi) ---\n");
  sim_4g_send_command("AT+QGPS=1");
  vTaskDelay(pdMS_TO_TICKS(MEDIUM_DELAY_MS));

  // Sau khi AT+QGPS=1 trả về OK, cần chờ rất lâu để module lấy được tín hiệu vệ
  // tinh
  printf("Dang cho GPS fix lan dau, viec nay co the mat vai phut...\n");
  sim_4g_send_command("AT+QGPSLOC=2");
  vTaskDelay(pdMS_TO_TICKS(GPS_FIX_DELAY_MS)); // Chờ 3 phút

  sim_4g_send_command("AT+QGPSLOC=2"); // Thử lại để chắc chắn đã có fix
  vTaskDelay(pdMS_TO_TICKS(MEDIUM_DELAY_MS));
  sim_4g_send_command("AT+QGPSTIME");
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));

  // 5. Tat GPS de tiet kiem nang luong
  printf("\n--- Tat GPS de tiet kiem nang luong ---\n");
  sim_4g_send_command("AT+QGPSEND");
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));

  // 6. Tat ket noi du lieu
  printf("\n--- Tat ket noi du lieu ---\n");
  sim_4g_send_command("AT+QIDEACT=1");
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));

  printf("\n--- Chuong trinh kiem tra hoan tat ---\n");
}
