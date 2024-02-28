#include "context.h"

esp_err_t init_nvs_flash(){
  esp_err_t ret = nvs_flash_init();
  ESP_ERROR_CHECK(ret);
  return ret;
}

void app_main(){
 
    init_mac_address();

    init_gpio_pins();

	if(init_nvs_flash() != ESP_OK){
		printf("#\r\nError NVS flash\r\n#\r\n");
	}

	flash_read_boot_partition();

#ifdef STA_MODE
    sprintf(ssid_arg , ROUTER_SSID);
    sprintf(pass_arg , ROUTER_PASS);
    xTaskCreate(wifi_init_sta, "wifi_init_sta_task", WIFI_STA_MODE_TASK_STACK_DEPTH, NULL, 5, NULL);
#elif defined AP_MODE
    wifi_init_accesspoint_mode();
    
    xTaskCreate(tcp_server_task, "tcp_server", TCP_SERVER_TASK_STACK_DEPTH, (void*)AF_INET, 5, NULL);
    xTaskCreate(udp_server_task, "udp_server", UDP_SERVER_TASK_STACK_DEPTH, (void*)AF_INET, 5, NULL);
#endif

}
