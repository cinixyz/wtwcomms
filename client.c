#include <kipr/botball.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* 
 * File Name          : main.c
 * Last Modified Date : 09/12/2018 (mm/dd/yyyy)
 * Purpose            : Wallaby-to-Wallaby Communication
 * Author(s)          : Cini (Justin Mok)
 * Notes              : 
 */

struct hostent *host_ent;

void connectWifi() {

    // Initialize wpa_cli
    system("wpa_cli ter"); // initializes wpa_cli

    // Kills WiFi host
    system("killall hostapd");
    system("echo Killed Wifi");

    // Initialize wpa_supplicant config file in order to use wpa_cli
    system("wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant-wlan0.conf"); // init wpa supplicant for wpa_cli
    system("echo initialized wpa_supplicant");

    // Remove existing networks on wpa_cli
    system("wpa_cli remove_network 0"); //

    // Create network, pass network SSID and password, enable network, and save network config
    system("wpa_cli add_network"); // init network creation
    system("wpa_cli set_network 0 ssid \'\"1904-wallaby\"\'");
    system("wpa_cli set_network 0 psk \'\"90bbc900\"\'"); 
    system("wpa_cli enable_network 0");
    system("wpa_cli save");
    system("echo saved and connected to network");
}

void initSocket() {
    struct hostent *serverStruct;
    int netSocket;
    netSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8001);
    // &server_addr.sin_addr.s_addr = "192.168.125.1"; idk why this doesnt work
    serverStruct = gethostbyname("192.168.125.1");
    bcopy((char *)serverStruct->h_addr, (char *)&server_addr.sin_addr.s_addr, serverStruct->h_length); // copy string bytes from h_addr to s_addr

    if (connect(netSocket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
    {
        printf("Connection failed.\n");
    }
}
int main()
{
    printf("Hello World!\n");


    connectWifi();

    printf("Goodbye World!\n");
    return 0;
}