#include "context.h"

#define UDP_DB

void get_device_wifi_info(char *device_wifi_info){

	unsigned char mac_str[20];

	esp_base_mac_addr_get(mac_str);
	// if(wifi_if_mode == STA_MODE){
	// 	tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
	// }else {
	// 	tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &ipInfo);
	// }
	
	uint32_t tempIpSlice = get_device_ip_info_int();

	int ip1 = tempIpSlice % 256;
	if(ip1 < 0){
		ip1 += 256;
	}
	tempIpSlice /= 256;
	int ip2 = tempIpSlice % 256;
	if(ip2 < 0){
		ip2 += 255;
	}
	tempIpSlice /= 256;
	int ip3 = tempIpSlice % 256;
	if(ip3 < 0){
		ip3 += 255;
	}
	tempIpSlice /= 256;
	int ip4 = tempIpSlice % 256;
	if(ip4 < 0){
		ip4 += 255;
	}
    
	sprintf(device_wifi_info , "%d.%d.%d.%d,%02X%02X%02X%02X%02X%02X" , \
					            ip1 , ip2 , ip3 , ip4 , \
					            mac_str[0], mac_str[1], mac_str[2], mac_str[3], mac_str[4], mac_str[5]);
    
}

void udp_get_info_response(int sock , int ip4 , int account_index , char *rx_buffer){

    char device_wifi_info[64];
    get_device_wifi_info(device_wifi_info);
#ifdef UDP_DB 	
	printf("udp_get_info_response\n");
#endif
	response_required = true;
	
	send_data_len = sprintf(send_data , "%s,%s,%s" , device_wifi_info, FIRMWARE_VERSION , HARDWARE_VERSION);

	strcat(send_data , "\r\n");
	send_data_len += 2;

#ifdef UDP_DB 
	printf(send_data);
#endif

}
