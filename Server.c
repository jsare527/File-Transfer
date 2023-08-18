#include "headerFiles.h"

int receiveFile(int sockid, struct sockaddr_in clntAddr, int clntAddrLen, char *destFile){
    int length;
    char buffer[256];
    FILE *fp;
    fp = fopen(destFile, "w");
    for(;;){
        if(fp == NULL){
            perror("Error writing to file\n");
            return -1;
        }
        length = recvfrom(sockid, (char*)buffer, sizeof(buffer), 0, (struct sockaddr*)&clntAddr, &clntAddrLen);
        buffer[length] = '\0';
        if(strncmp("\nEOF", buffer, 4) == 0){
            printf("File recieved and placed into destination: %s\n", destFile);
            sendto(sockid, (char*)"ACK", strlen("ACK"), 0, (struct sockaddr*)&clntAddr, sizeof(clntAddr));
            break;
        }
        fprintf(fp, "%s", buffer);
    }
    fclose(fp);
}

int main(int argc, char *argv[]){
    int sockid, len, clntAddrLen;
    char buffer[256];
    char *destFile = argv[2];
    struct sockaddr_in servAddr, clntAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));
    sockid = socket(PF_INET, SOCK_DGRAM, 0);
    if(sockid < 0){
        perror("Error: socket failed\n");
        exit(1);
    }
    if(bind(sockid, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
   if(argc != 3){
    printf("Two arguments required\n");
    exit(1);
   }
   clntAddrLen = sizeof(clntAddr);
   int flag = 0;
   char response[15];
   for(;;){
    printf("Waiting for file...\n");
    receiveFile(sockid, clntAddr, clntAddrLen, destFile);
    sleep(5);
    //printf("Do you wish to exit the program?[yes][no]: ");
    //fgets(response, 5, stdin);
    //for(int i=0; response[i]; i++){
    //    response[i] = tolower(response[i]);
    //}
    //if(strstr(response, "yes") != NULL){
    //    printf("Exiting program...\n");
    //    exit(0);
    //}
   }
}