# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)/components/esp8266/include $(IDF_PATH)/components/esp8266/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/esp8266 -lesp8266 -L$(IDF_PATH)/components/esp8266/lib -lairkiss -lcrypto -lespnow -lgcc -lhal -lcore -lnet80211 -lphy -lpp -lpwm -lsmartconfig -lssc -lwpa -lwps -L $(IDF_PATH)/components/esp8266/ld -T esp8266_out.ld -Wl,--no-check-sections -u call_user_start -T esp8266.common.ld -T esp8266.rom.ld
COMPONENT_LINKER_DEPS += $(IDF_PATH)/components/esp8266/lib/libairkiss.a $(IDF_PATH)/components/esp8266/lib/libcrypto.a $(IDF_PATH)/components/esp8266/lib/libespnow.a $(IDF_PATH)/components/esp8266/lib/libgcc.a $(IDF_PATH)/components/esp8266/lib/libhal.a $(IDF_PATH)/components/esp8266/lib/libcore.a $(IDF_PATH)/components/esp8266/lib/libnet80211.a $(IDF_PATH)/components/esp8266/lib/libphy.a $(IDF_PATH)/components/esp8266/lib/libpp.a $(IDF_PATH)/components/esp8266/lib/libpwm.a $(IDF_PATH)/components/esp8266/lib/libsmartconfig.a $(IDF_PATH)/components/esp8266/lib/libssc.a $(IDF_PATH)/components/esp8266/lib/libwpa.a $(IDF_PATH)/components/esp8266/lib/libwps.a $(IDF_PATH)/components/esp8266/ld/esp8266.common.ld $(IDF_PATH)/components/esp8266/ld/esp8266.rom.ld
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += esp8266
component-esp8266-build: 
