deps_config := \
	/opt/esp/ESP8266_RTOS_SDK/components/freertos/Kconfig \
	/opt/esp/ESP8266_RTOS_SDK/components/log/Kconfig \
	/opt/esp/ESP8266_RTOS_SDK/components/lwip/Kconfig \
	/opt/esp/ESP8266_RTOS_SDK/components/newlib/Kconfig \
	/opt/esp/ESP8266_RTOS_SDK/components/ssl/Kconfig \
	/opt/esp/ESP8266_RTOS_SDK/components/tcpip_adapter/Kconfig \
	/opt/esp/ESP8266_RTOS_SDK/components/esptool_py/Kconfig.projbuild \
	/opt/esp/ESP8266_RTOS_SDK/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
