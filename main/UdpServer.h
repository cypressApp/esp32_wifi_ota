#include "lwip/sockets.h"

#define UDP_DB

#define GET_INFO "GET_INFO\r\n"

extern int  pairing_step;
extern int  ap_step;
extern char temp_ssid    [128];
extern char temp_password[128];
extern char send_data[3072];
extern int  temp_ssid_len;
extern int  temp_password_len;
extern int  send_data_len;
extern bool response_required;
extern long int  random_code;

void processData(int sock, int ip4 , char *rx_buffer , struct sockaddr *sourceAddr , int len);
void udp_server_task(void *pvParameters);


