#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sim_4g.h"
#include "uart_driver.h"
#include <stdio.h>

// Thời gian chờ giữa các lệnh
#define SHORT_DELAY_MS 500
#define LONG_DELAY_MS 5000
#define GPS_DELAY_MS                                                           \
  15000 // Cần nhiều thời gian hơn để module có thể có GPS fix

// Địa chỉ APN của nhà mạng Viettel, bạn có thể thay đổi tùy ý
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
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));

  // 3. Cau hinh APN va kich hoat ket noi du lieu
  printf("\n--- Cau hinh APN va ket noi du lieu ---\n");
  char apn_cmd[64];
  sprintf(apn_cmd, "AT+CGDCONT=1,\"IP\",\"%s\"", APN_VIETTEL);
  sim_4g_send_command(apn_cmd);
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));

  sim_4g_send_command("AT+QIACT=1");
  vTaskDelay(pdMS_TO_TICKS(LONG_DELAY_MS));

  // 4. Bat GPS va lay thong tin
  printf("\n--- Bat GPS va lay toa do (dat module ngoai troi) ---\n");
  sim_4g_send_command("AT+QGPS=1");
  vTaskDelay(pdMS_TO_TICKS(LONG_DELAY_MS));

  // Su dung AGPS de dinh vi nhanh hon
  sim_4g_send_command("AT+QGPSCFG=\"autogps\",1");
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));

  // Lay toa do GPS
  printf("Dang cho GPS fix, viec nay co the mat vai phut...\n");
  sim_4g_send_command("AT+QGPSLOC=2");
  vTaskDelay(pdMS_TO_TICKS(GPS_DELAY_MS));

  // Lay thoi gian UTC tu GPS
  sim_4g_send_command("AT+QGPSTIME");
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));

  // 5. Tat GPS de tiet kiem pin
  printf("\n--- Tat GPS de tiet kiem nang luong ---\n");
  sim_4g_send_command("AT+QGPSEND");
  vTaskDelay(pdMS_TO_TICKS(SHORT_DELAY_MS));

  printf("\n--- Chuong trinh kiem tra hoan tat ---\n");
}
