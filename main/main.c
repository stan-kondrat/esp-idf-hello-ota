/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#define LWJSON_IGNORE_USER_OPTS

#include "esp_log.h"
#include "esp_tls.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "nvs_flash.h"
#include "ota_github.h"
#include "web.h"
#include "wifi.h"
#include <stdio.h>

static const char *TAG = "MAIN";

extern const uint8_t root_ca_pem_start[] asm("_binary_root_ca_pem_start");
extern const uint8_t root_ca_pem_end[] asm("_binary_root_ca_pem_end");

void app_main(void) {
    ESP_LOGI(TAG, "Example app");

    /* Configure the peripheral according to the LED type and Start blinking */
    // configure_led();
    // xTaskCreate(led_task, "led_task", 4096, NULL, 10, NULL);

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Init Root CA
    esp_err_t esp_ret = ESP_FAIL;
    ESP_LOGI(TAG, "https_request using global ca_store");
    esp_ret = esp_tls_set_global_ca_store(root_ca_pem_start, root_ca_pem_end - root_ca_pem_start);
    if (esp_ret != ESP_OK) {
        ESP_LOGE(
            TAG,
            "Error in setting the global ca store: [%02X] (%s),could not complete the https_request using "
            "global_ca_store",
            esp_ret, esp_err_to_name(esp_ret)
        );
        return;
    }

    wifi_init_sta();

    web_server_start();

    // ota_github_install_latest((ota_github_config_t){0});
}