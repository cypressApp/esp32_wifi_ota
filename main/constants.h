
#include <stdio.h>

#define TAG "Cypress"

#define  AP_MODE     
#undef  STA_MODE    

#define ESP_AP_WIFI_PASS        "123456789"  
#define DEVICE_MAC_ADDRESS_STR  "C0428A15753C"
#define ESP_AP_WIFI_SSID        "CYPRESS_OTA_15753C"

#define ROUTER_SSID "Xiaomi326589"
#define ROUTER_PASS "majid4321"
#define TCP_PORT    1234
#define UDP_PORT    1234

#define WIFI_STA_MODE_TASK_STACK_DEPTH       20480
#define CHECK_TCP_TIMEOUT_TASK_STACK_DEPTH   4096

#define HARDWARE_VERSION        "01.20"
#define FIRMWARE_VERSION        "01.20"

#define BOOT_PARTION_TYPE         0x59
#define BOOT_PARTITION_SUBTYPE    0x0B