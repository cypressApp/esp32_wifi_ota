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

    reset_handler();

	flash_read_boot_partition();

    flash_read_wifi_router_info();

    if(memcmp(flash_wifi_mode , WIFI_STA_MODE , 6) == 0){

        sprintf(ssid_arg , flash_router_ssid);
        sprintf(pass_arg , flash_router_password);

        xTaskCreate(wifi_init_sta, "wifi_init_sta_task", WIFI_STA_MODE_TASK_STACK_DEPTH, NULL, 5, NULL);		
        wifi_if_mode = STA_MODE; 

    }else{

        wifi_init_accesspoint_mode();
        wifi_if_mode = AP_MODE;
        
        xTaskCreate(udp_server_task, "udp_server", UDP_SERVER_TASK_STACK_DEPTH, (void*)AF_INET, 5, NULL);
        xTaskCreate(tcp_server_task, "tcp_server", TCP_SERVER_TASK_STACK_DEPTH, (void*)AF_INET, 5, NULL);

    }

}
