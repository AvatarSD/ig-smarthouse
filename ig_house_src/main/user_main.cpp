/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdint.h>
#include <strings.h>

extern "C"{
#include "sdkconfig.h"

#include "esp_misc.h"
#include "esp_sta.h"
#include "esp_system.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <sys/socket.h>

#include "cJSON.h"
}

#include "server.h"

#define	SERVER_PORT	1002



QueueHandle_t server_queue;


void indication_task(void *param)
{
    while(true);
    vTaskDelete(NULL);
}

void wifi_manenger_task(void *param)
{
    while(true){

    int8_t rssi = wifi_station_get_rssi();
    STATION_STATUS status = wifi_station_get_connect_status();
    printf("rssi: %d; status: %d  ", rssi, status);
    printf("heap: %d\n", system_get_free_heap_size());


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


/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
extern "C" uint32_t user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32_t rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;

        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

static void wifi_event_hand_function(System_Event_t *evt)
{
    switch (evt->event_id)	
    {
    case EVENT_STAMODE_CONNECTED:
        printf("connect	to	ssid	%s,	channel	%d\n",	
                evt->event_info.connected.ssid,	
                evt->event_info.connected.channel);
    break;

    case EVENT_STAMODE_DISCONNECTED:
        printf("disconnect	from	ssid	%s,	reason	%d\n",	
                evt->event_info.disconnected.ssid,	
                evt->event_info.disconnected.reason);
    break;

    case EVENT_STAMODE_AUTHMODE_CHANGE:
        printf("mode:	%d	->	%d\n",	
                evt->event_info.auth_change.old_mode,	
                evt->event_info.auth_change.new_mode);
    break;

    case EVENT_STAMODE_GOT_IP:
        printf("ip:"	IPSTR	",mask:"	IPSTR	",gw:"	IPSTR,
                IP2STR(&evt->event_info.got_ip.ip),
                IP2STR(&evt->event_info.got_ip.mask),
                IP2STR(&evt->event_info.got_ip.gw));
        printf("\n");
        printf("sta got ip , creat task %d\n", system_get_free_heap_size());
        user_conn_init();
    break;

    case EVENT_SOFTAPMODE_STACONNECTED:
        printf("station:	"	MACSTR	"join,	AID	=	%d\n",	
                MAC2STR(evt->event_info.sta_connected.mac),	
                evt->event_info.sta_connected.aid);
    break;

    case EVENT_SOFTAPMODE_STADISCONNECTED:
        printf("station:	"	MACSTR	"leave,	AID	=	%d\n",
                MAC2STR(evt->event_info.sta_disconnected.mac),	
                evt->event_info.sta_disconnected.aid);
    break;

    default:
    break;
    }

}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
extern "C" void user_init(void)
{
    printf("SDK version:%s\n", system_get_sdk_version());
    printf("SDK version:%s %d\n", system_get_sdk_version(), system_get_free_heap_size());
    
    wifi_set_opmode(STATION_MODE);

    struct station_config sta_config;
    bzero(&sta_config, sizeof(struct station_config));


    sprintf((char*)sta_config.ssid, "Home N");
    sprintf((char*)sta_config.password, "5Od_9.iktS@");

//    sprintf(sta_config.ssid, "AndroidAP_1449");
  //  sprintf(sta_config.password, "12345678");

    wifi_station_set_config(&sta_config);
    wifi_set_event_handler_cb(wifi_event_hand_function);

}
