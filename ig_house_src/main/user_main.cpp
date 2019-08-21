/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdint.h>
#include <strings.h>

// #include "sdkconfig.h"

// #include "esp_misc.h"
// #include "esp_sta.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <sys/socket.h>

#include "cJSON.h"

#include "server.h"

#define	SERVER_PORT	1002

static const char *TAG = "simple wifi";

QueueHandle_t server_queue;


void indication_task(void *param)
{
    while(true);
    vTaskDelete(NULL);
}

void wifi_manenger_task(void *param)
{
    while(true){

    wifi_ap_record_t apRecord;
    auto status = esp_wifi_sta_get_ap_info(&apRecord)
    ;
    printf("rssi: %d; status: %d  ", apRecord.rssi, status);
    // printf("heap: %d\n", system_get_free_heap_size());


     /* Block for 500ms. */
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

    //printf("stak: %d\n", uxHighWuxTaskGetStackHighWaterMark( NULL ));
    vTaskDelay(xDelay);

    }
    
    vTaskDelete(NULL);
}

void main_server_task(void *param)
{
    printf("server task is running!\n");
    
    JsonServer server(false, server_queue);
    
    for(;;)
    {  
        if(server.start(1500)){
            perror("server start error!\n");
        }

        if(server.run()){
            perror("server runtime error!\n");
        }

        server.stop();
    }
    vTaskDelete(NULL);
}

void user_conn_init(void)
{
    server_queue = xQueueCreate(10, sizeof(cJSON*));

    xTaskCreate(indication_task, "indication_task", 64, NULL, 2, NULL);
    xTaskCreate(wifi_manenger_task, "wifi_manenger_task", 256, NULL, 2, NULL);
    xTaskCreate(main_server_task, "net_server_json", 256, NULL, 2, NULL);

/*    ret = xTaskCreate(openssl_server_thread,
                      OPENSSL_SERVER_THREAD_NAME,
                      OPENSSL_SERVER_THREAD_STACK_WORDS,
                      NULL,
                      OPENSSL_SERVER_THREAD_PRORIOTY,
                      &openssl_handle);

    if (ret != pdPASS)  {
        printf("create thread %s failed\n", OPENSSL_SERVER_THREAD_NAME);
        return ;
    }*/
}


static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "got ip:%s",
                 ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        user_conn_init();
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        ESP_LOGI(TAG, "station:"MACSTR" join, AID=%d",
                 MAC2STR(event->event_info.sta_connected.mac),
                 event->event_info.sta_connected.aid);
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        ESP_LOGI(TAG, "station:"MACSTR"leave, AID=%d",
                 MAC2STR(event->event_info.sta_disconnected.mac),
                 event->event_info.sta_disconnected.aid);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        esp_wifi_connect();
        break;
    default:
        break;
    }
    return ESP_OK;
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
//  void user_init(void)
// {
//     printf("SDK version:%s\n", system_get_sdk_version());
//     printf("SDK version:%s %d\n", system_get_sdk_version(), system_get_free_heap_size());
    
//     wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
//     esp_wifi_init(&config);
//     esp_wifi_set_mode(WIFI_MODE_STA);

//     struct station_config sta_config;
//     bzero(&sta_config, sizeof(struct station_config));


//     sprintf((char*)sta_config.ssid, "Home N");
//     sprintf((char*)sta_config.password, "5Od_9.iktS@");

// //    sprintf(sta_config.ssid, "AndroidAP_1449");
//   //  sprintf(sta_config.password, "12345678");

//     // wifi_station_set_config(&sta_config);
//     // wifi_set_event_handler_cb(wifi_event_hand_function);

// }


void wifi_init_sta()
{
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL) );

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .sta = {
            {.ssid = "Home N"},
            {.password = "5Od_9.iktS@"}
        }
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");
}

extern "C" void app_main()
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_sta();

}