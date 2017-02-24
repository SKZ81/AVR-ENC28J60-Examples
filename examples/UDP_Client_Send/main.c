#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "ip_arp_udp_tcp.h"
#include "websrv_help_functions.h"
#include "enc28j60.h"
#include "timeout.h"
#include "net.h"

#include "avr_uart.h"

#define REMOTE_PORT 9600
#define LOCAL_PORT  2000
#define BUFFER_SIZE 650

static uint8_t mac_address[6] = {0x22,0x22,0x22,0x10,0x00,0x33};
static uint8_t local_host[4]  = {192,168,1,191};
static uint8_t remote_host[4] = {192,168,1,155};
static uint8_t gateway[4]     = {192,168,1,1};
static uint16_t local_port    = 2000;
static uint16_t remote_port   = 9600;

static uint8_t buf[BUFFER_SIZE+1];
static uint8_t data[18] = {0x01,0x02,0x03,0x04,0x05};


static void net_init (void);

int main (void) {
    avr_uart_init();
    stdout = &avr_uart_output;
    stdin  = &avr_uart_input_echo;

    net_init();
    printf("UDP send in loop each 1s\n");
    for(;;) {
        send_udp(buf,data,sizeof(data),local_port, remote_host, remote_port);
        printf("Send !\n");
        _delay_ms(1000);
    }
    return (0);
}

static void net_init (void) {
    _delay_loop_1(0);
    printf("enc28j60Init(mac_address);");
    enc28j60Init(mac_address);
    printf("enc28j60clkout(2);");
    enc28j60clkout(2);
    _delay_loop_1(0);
    printf("enc28j60PhyWrite(PHLCON,0x476);");
    enc28j60PhyWrite(PHLCON,0x476);
    _delay_loop_1(0);
    printf("init_ip_arp_udp_tcp(mac_address,local_host,local_port);");
    init_ip_arp_udp_tcp(mac_address,local_host,local_port);
    printf("client_set_gwip(gateway);");
    client_set_gwip(gateway);
}
