/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>

#include <esp_system.h>

#include <esp_wifi.h>
#include <esp_sta.h>

#include <freertos/FreeRTOS.h>

#define	SERVER_PORT	1002


void indication_task(void *param)
{

    vTaskDelete(NULL);
}

void wifi_manenger_task(void *param)
{

    vTaskDelete(NULL);
}

void main_server_task(void *param)
{
    printf("server task is running!\n");

    LOCAL int32_t socket;


    while(true){




    }

    vTaskDelete(NULL);
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
uint32_t user_rf_cal_sector_set(void)
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

static void wifi_event_hand_function(System_Event_t *event)
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
void user_init(void)
{
    printf("SDK version:%s\n", system_get_sdk_version());

    wifi_set_opmode(STATION_MODE);

    struct station_config sta_conf;
    bzero(&sta_config, sizeof(struct station_config));


    sprintf(sta_config.ssid, "Guest N");
    sprintf(sta_config.password, "toshiba36");
    wifi_station_set_config(&sta_config);
    wifi_set_event_handler_cb(wifi_event_hand_function);

    xTaskCreate(main_server_task, "indication_task", 64, NULL, 2, NULL);
    xTaskCreate(main_server_task, "wifi_manenger_task", 128, NULL, 2, NULL);
    xTaskCreate(main_server_task, "net_server_json", 256, NULL, 2, NULL);

}
