#include "context.h"

int  pairing_step = 0;
int  ap_step = 0;
char temp_ssid[128] = "";
char temp_password[128] = "";
char send_data[3072];
int  temp_ssid_len = 0;
int  temp_password_len = 0;
int  send_data_len = 0;
bool response_required = false;
long int  random_code = 111111;

void processData(int sock, int ip4 , char *rx_buffer , struct sockaddr *sourceAddr , int len){
	
	response_required = false;
	 	
	if(memcmp(rx_buffer , GET_INFO , 10) == 0){     
		udp_get_info_response(sock, ip4 ,0 , rx_buffer);
	} 	
	
}

void udp_server_task(void *pvParameters)
{
    char rx_buffer[128];
    char addr_str [128];
    int addr_family = (int)pvParameters;
	
    int ip_protocol = 0;
    struct sockaddr_in6 dest_addr;

    while (1) {

        if (addr_family == AF_INET) {
            struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
            dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
            dest_addr_ip4->sin_family = AF_INET;
            dest_addr_ip4->sin_port = htons(UDP_PORT);
            ip_protocol = IPPROTO_IP;
        } else if (addr_family == AF_INET6) {
            bzero(&dest_addr.sin6_addr.un, sizeof(dest_addr.sin6_addr.un));
            dest_addr.sin6_family = AF_INET6;
            dest_addr.sin6_port = htons(UDP_PORT);
            ip_protocol = IPPROTO_IPV6;
        }

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
#ifdef UDP_DB			
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
#endif			
            break;
        }
#ifdef UDP_DB 		
        ESP_LOGI(TAG, "Socket created");
#endif

#if defined(CONFIG_EXAMPLE_IPV4) && defined(CONFIG_EXAMPLE_IPV6)
        if (addr_family == AF_INET6) {
            // Note that by default IPV6 binds to both protocols, it is must be disabled
            // if both protocols used at the same time (used in CI)
            int opt = 1;
            setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
            setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
        }
#endif

        int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err < 0) {
#ifdef UDP_DB 			
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
#endif			
        }

        while (1) {
#ifdef UDP_DB 	
			ESP_LOGI(TAG, "Waiting for data");
#endif			
			struct sockaddr_storage source_addr; 
			socklen_t socklen = sizeof(source_addr);
			int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

			// Error occurred during receiving
			if (len < 0) {
#ifdef UDP_DB 				
				ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
#endif				
				break;
			}
			// Data received
			else {
				// Get the sender's ip address as string
				if (source_addr.ss_family == PF_INET) {
					inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
				} else if (source_addr.ss_family == PF_INET6) {
					inet6_ntoa_r(((struct sockaddr_in6 *)&source_addr)->sin6_addr, addr_str, sizeof(addr_str) - 1);
				}

				if(rx_buffer[0] == 'p' && rx_buffer[1] == 'n' && rx_buffer[2] == 'g'){
					char temp10[32];
					int png_len = sprintf(temp10 , "A%d.%d.%d.%d\r\n" , ip1 , ip2 , ip3 , ip4);
					sendto(sock, temp10 , png_len, 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
				}else{
					int temp_ip4 = get_ip4(addr_str , sizeof(addr_str));

					rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string...
#ifdef UDP_DB 				
					unsigned char mac_str[20];
					ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
					ESP_LOGI(TAG, "%s", rx_buffer);
					esp_base_mac_addr_get(mac_str);
					ESP_LOGI(TAG, "BASE: %02X %02X %02X %02X %02X %02X", mac_str[0], mac_str[1], mac_str[2], mac_str[3], mac_str[4], mac_str[5]);
					esp_efuse_mac_get_default(mac_str);				
					ESP_LOGI(TAG, "EFUSE: %02X %02X %02X %02X %02X %02X", mac_str[0], mac_str[1], mac_str[2], mac_str[3], mac_str[4], mac_str[5]);
#endif
					if(temp_ip4 < 256){
						processData(sock , temp_ip4 , rx_buffer , (struct sockaddr *)&source_addr , len);
					}
		
					if(response_required){

						printf(send_data);	
						
						int err = sendto(sock, send_data, send_data_len, 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
						
						if (err < 0) {
	#ifdef UDP_DB 						
							ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
	#endif						
							break;
						}

					}
				}



			}

        }

        if (sock != -1) {
#ifdef UDP_DB 			
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
#endif			
            shutdown(sock, 0);
            close(sock);
        }
    }

    vTaskDelete(NULL);

	vTaskDelay(1000 / portTICK_PERIOD_MS);
	xTaskCreate(udp_server_task, "udp_server", UDP_SERVER_TASK_STACK_DEPTH, (void*)AF_INET, 5, NULL);
	
}