#include "lwip/sockets.h"

#define UDP_DB

#define GET_INFO "GET_INFO\r\n"

#define START_PAIRING "StartP"
#define SET_ROUTER_SSID_PREFIX "ssid:"
#define SET_ROUTER_SSID_SUFFIX "rr"
#define SET_ROUTER_PASS_PREFIX "pass:"
#define SET_ROUTER_PASS_SUFFIX "majid"
#define FINISH_PAIRING "FinishP"
#define PAIR_ACK_RESPONSE "pAck"

#define START_PAIRING_IND   0
#define SET_ROUTER_SSID_IND 1
#define SET_ROUTER_PASS_IND 2
#define FINISH_PAIRING_IND  3

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


