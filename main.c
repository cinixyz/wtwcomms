#include <kipr/botball.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* 
 * File Name          : main.c
 * Last Modified Date : 09/12/2018 (mm/dd/yyyy)
 * Purpose            : Wallaby-to-Wallaby Communication
 * Author(s)          : Cini (Justin Mok)
 * Notes              : 
 */

struct hostent *host_ent;

int main()
{
    printf("Hello World!\n");

    system("wpa_cli ter"); // initializes wpa_cli
    system("killall hostapd"); // kills wifi host
    system("wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant-wlan0.conf"); // init wpa supplicant for wpa_cli

    system("wpa_cli remove_network 0"); // removes existing network

    system("wpa_cli add_network"); // init network creation
    system("wpa_cli set_network 0 ssid \'\"1904-wallaby\"\'"); // pass network ssid (in this case, the server ssid)
    system("wpa_cli set_network 0 psk \'\"90bbc900\"\'"); // pass network password for above network
    system("wpa_cli enable_network 0"); // enabling network
    system("wpa_cli save"); // save config

    int netSocket;
    netSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8001);
    server_addr.sin_addr.s_addr = "192.168.125.1";

    if (bind(netSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
    	printf("error in socket binding");
        exit(0);
    }
    printf("Goodbye World!\n");
    return 0;
}