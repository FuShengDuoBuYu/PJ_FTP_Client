# include "common.h"
#include "file_util.h"

SOCKET create_tcp_socket(){
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    if(WSAStartup(sockVersion, &wsaData)!=0){
        printf("WSAStartup error!");
        return 0;
    }
    SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return sclient;
}

int bind_socket_local_port(SOCKET sclient, int port){
    struct sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(port);
    clientAddr.sin_addr.S_un.S_addr = 0;
    int iResult;
    iResult = bind(sclient, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
    if(iResult == SOCKET_ERROR){
        printf("bind error ! %d\n", WSAGetLastError());
        closesocket(sclient);
        WSACleanup();
        return 0;
    }
    return 1;
}

int connect_to_server(SOCKET sclient, char *ip, int port){
    int iResult;
    struct sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
    serAddr.sin_addr.S_un.S_addr = inet_addr(ip);
    iResult = connect(sclient, (SOCKADDR *)&serAddr, sizeof(SOCKADDR));
    if(iResult == SOCKET_ERROR){
        printf("connect error ! %d\n", WSAGetLastError());
        closesocket(sclient);
        WSACleanup();
        return 0;
    }
    return 1;
}

int send_data_to_server(SOCKET sclient, char *sendbuf){
    int iResult;
    iResult = send(sclient, sendbuf, MAX_FILE_SIZE, 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(sclient);
        WSACleanup();
        return 0;
    }
    printf("Bytes Sent: %ld\n", iResult);
    return 1;
}

int send_file_info_to_server(SOCKET sclient, FileInfo *sendbuf){
    int iResult;
    iResult = send(sclient, (char *)sendbuf, sizeof(FileInfo), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(sclient);
        WSACleanup();
        return 0;
    }
    printf("Bytes Sent: %ld\n", iResult);
    return 1;
}

int recv_data_from_server(SOCKET sclient, char *recvbuf){
    int iResult;
    iResult = recv(sclient, recvbuf, MAX_FILE_SIZE, 0);
    if(iResult == SOCKET_ERROR){
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(sclient);
        WSACleanup();
        return 0;
    }
    return 1;
}


int send_file_to_server(SOCKET sclient, char *filename){
    // 使用generate_file_info生成FileInfo结构体，然后发送
    int send_buffer_index = 0;
    int enter_count = 0;
    FileInfo *file_info;
    char *send_buffer = (char *)malloc(MAX_FILE_SIZE);
    do {
        file_info = generate_file_info(filename, send_buffer, send_buffer_index, &enter_count);
printf("file_tag: %d file_tag: %d file_tag: %s\n", file_info->file_tag, file_info->file_rmd, file_info->buffer);
        // TODO: 发送文件信息
        if(send_file_info_to_server(sclient, file_info) == 0){
            return 0;
        }
        memset(send_buffer, 0, sizeof(send_buffer));
        // free(file_info);
        send_buffer_index++;
    } while(file_info->file_tag == 0);

    return 1;
}

// TODO: recv file
int recv_file_info_from_server(SOCKET sclient, FileInfo *recvbuf){
    int iResult;
    iResult = recv(sclient, (char *)recvbuf, sizeof(FileInfo), 0);
    if(iResult == SOCKET_ERROR){
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(sclient);
        WSACleanup();
        return 0;
    }
    return 1;
}

int close_socket(SOCKET sclient){
    closesocket(sclient);
    WSACleanup();
    return 1;
}