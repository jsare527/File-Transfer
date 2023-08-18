#include "headerFiles.h"


void receiveACK(int sockid, struct sockaddr_in servAddr, int servAddrLen){
    char buffer[256];
    for(;;){
        printf("Waiting for conformation...\n");
        int len = recvfrom(sockid, (char*)buffer, sizeof(buffer), 0, (struct sockaddr*)&servAddr, &servAddrLen);
        buffer[len] = '\0';
        if(strncmp("ACK", buffer, 4) == 0){
            printf("File received successfully\n");
            return;
        }else{
            printf("Failed transfer\n");
        }
    }
}

int sendFile(char *fileName, int sockid, struct sockaddr_in servAddr){
    FILE *fp;
    char str[256];
    char *message = "\nEOF";
    fp = fopen(fileName, "r");
    if(fp == NULL){
        perror("Error opening file\n");
        return -1;
    }
    while(fgets(str, 255, fp) != NULL){
        sendto(sockid, (char*)str, strlen(str), 0, (struct sockaddr*) &servAddr, sizeof(servAddr));
    }
    printf("File sent successfully: %s\n", fileName);
    sendto(sockid, (char*)message, strlen(message), 0, (struct sockaddr*)&servAddr, sizeof(servAddr));
    return 1;
}


int main(int argc, char *argv[]){
    int sockid, len, servPort, servAddrLen;
    char *servName, *fileName;
    char buffer[256+1];
    struct sockaddr_in servAddr;
    if(argc != 4){
        printf("Error: three arguments are needed\n");
        exit(1);
    }
    servName = argv[1];
    servPort = atoi(argv[2]);
    fileName = argv[3];
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(servPort);
    inet_pton(AF_INET, servName, &servAddr.sin_addr);
    sockid = socket(PF_INET, SOCK_DGRAM, 0);
    if(sockid < 0){
        perror("Error: socket failed\n");
        exit(1);
    }
    servAddrLen = sizeof(servAddr);
    sendFile(fileName, sockid, servAddr);
    receiveACK(sockid, servAddr, servAddrLen);
    close(sockid);
    return 0;
}