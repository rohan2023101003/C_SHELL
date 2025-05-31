#ifndef __iMan_C__
#define __iMan_C__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

void fetch_manpage(const char* command) {
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char request[9000];
    char response[4096];
    int bytes_received;
    int header_end = 0;

    server = gethostbyname("man.he.net");
    if (server == NULL) {
        printf("Error: No such host\n");
        return;
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return;
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); // HTTP port

    // Copy the resolved address (from `server->h_addr`) into the `sin_addr.s_addr` field
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        return;
    }

    // Create HTTP GET request
    int length_of_the_request=snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\nConnection: close\r\n\r\n", command);
    if(length_of_the_request<0){
        perror("Error in creating request");
        close(sockfd);
        return;
    }

    // Send request
    if (send(sockfd, request, strlen(request), 0) < 0) {
        perror("Send failed");
        close(sockfd);
        return;
    }

    // Receive response
    int inside_pre = 0;
    int open_bracket = 0;
    int close_bracket = 0;
    while ((bytes_received = recv(sockfd, response, sizeof(response) - 1, 0)) > 0) {
        response[bytes_received] = '\0';
        for(int i = 0; i < bytes_received; i++) {
            if(response[i]=='<'){open_bracket++;}
            if(response[i]=='>'){close_bracket++;}
            if(open_bracket == close_bracket && open_bracket!=0 && response[i]!='>' && response[i]!='<'){
                printf("%c",response[i]);
            }
        }
        
    }
    if (bytes_received < 0) {
        perror("Receive failed");
    }

    // Close socket
    close(sockfd);
}


#endif