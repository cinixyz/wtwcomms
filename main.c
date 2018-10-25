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
#include <stdbool.h>

/* 
 * File Name          : main.c
 * Last Modified Date : 09/12/2018 (mm/dd/yyyy)
 * Purpose            : Wallaby-to-Wallaby Communication (THIS IS FOR THE HOST**************************88)
 * Author(s)          : Cini (Justin Mok)
 * Notes              : 
 */


/* 
function call sequence:
HOST    initHost();
CLIENT  connectWifi();
CLIENT  initSocket();
HOST    sendData();
CLIENT  listenData();


CURRENT ISSUES:
client does not connect to host in initSocket();

*/
struct hostent *host_ent;
struct sockaddr_in host;
struct sockaddr_in dest;
int hostSocket, clientSocket;
int data;
socklen_t size = sizeof(struct sockaddr_in);
thread listener;
bool connection = false;

void initHostWifi() {
    system("wpa_cli ter");
    system("killall hostapd");
    system("wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant-wlan0.conf");
    system("/usr/bin/python /usr/bin/wifi_configurator.py");
    printf("host wifi initialized\n");
}

void listenData() {
    if (connection) {
        while (1) {
        int received = recv(hostSocket, (int *)data, 999, 0);
        if (received == 0 && received == -1) {
            printf("error receiving socket data\n");
        } else {
            printf("read data: %i\n", data);
        } // error handling
        }
    }
    else {
        printf("cannot listen to data because socket != good\n");
    }
};

void initHost() {
    
    initHostWifi();
    
    int hostSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct hostent *serverStruct = gethostbyname("192.168.125.1");

    host.sin_family = AF_INET;
    host.sin_port = htons(8001);
    bcopy((char *)serverStruct->h_addr, (char *)&host.sin_addr.s_addr, serverStruct->h_length);

    bind(hostSocket, (struct sockaddr *)&host, sizeof(struct sockaddr));
    printf("Initialized Socket Connection\n");
    
    listen(hostSocket, 1);
    printf("Started Listening on port 8001\n");
    
    clientSocket = accept(hostSocket, (struct sockaddr *)&dest, &size);
    if (clientSocket == -1) {
        printf("Socket Connection Failed\n");   
    }
    printf("Socket Connection Success\n");
    
    listener = thread_create(listenData);
}

void connectWifi() {

    /*********************ONLY RUN IF BRAIN IS CLIENT //  YOU WILL NEED TO RESTART WALLABY*********************/
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
    listener = thread_create(listenData);
    /********************* THIS IS FOR CLIENT *********************/
    struct hostent *serverStruct;
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8001);
    // &server_addr.sin_addr.s_addr = "192.168.125.1"; idk why this doesnt work
    serverStruct = gethostbyname("192.168.125.1");
    bcopy((char *)serverStruct->h_addr, (char *)&server_addr.sin_addr.s_addr, serverStruct->h_length); // copy string bytes from h_addr to s_addr

    if (connect(clientSocket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
        printf("Connection failed.\n");
        connection = false;
    } else {
    	printf("Connected Successful.\n");
    }
    
}

void sendData(int data) {
    int packet = send(clientSocket, (int *) data, 999, 0);
    if (packet == -1) {
        printf("sending data failed\n");
    }
}
int main() {
    printf("Hello World!\n");

    connectWifi();

    printf("Goodbye World!\n");
    return 0;
}