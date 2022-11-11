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
    iResult = send(sclient, sendbuf, sizeof(MsgHeader), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(sclient);
        WSACleanup();
        return 0;
    }
    printf("Bytes Sent: %ld\n", iResult);
    return 1;
}

int send_file_info_to_server(SOCKET sclient, char *sendbuf){
    int iResult;
    printf("1. before send sendbuf: %d %p\n", sizeof(sendbuf), sendbuf);
    MsgHeader *header = (MsgHeader *)sendbuf;
    iResult = send(sclient,sendbuf, sizeof(MsgHeader), 0 );
    printf("2. after send Bytes Sent: %s iResult : %d\n", header->info.fileData.buffer, iResult);
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
    iResult = recv(sclient, recvbuf, sizeof(MsgHeader), 0);
    if(iResult == SOCKET_ERROR){
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(sclient);
        WSACleanup();
        return 0;
    }
    return 1;
}


int send_file_to_server(SOCKET data_client, char *filename){
    MsgHeader msgHeader;
    memset(&msgHeader, 0, sizeof(msgHeader));
    int enter_count = 0;
    do{
        msgHeader.info.fileData.enter_count = enter_count;
        generate_file_info(filename, &msgHeader.info.fileData,
                            msgHeader.info.fileData.file_order,
                            &enter_count);
        msgHeader.msgType = MSGTYPE_PUT;
        msgHeader.msgID = MSG_SEND;
        send_file_info_to_server(data_client, (char* )&msgHeader);
        recv_file_info_from_server(data_client,&msgHeader);
        if(!(msgHeader.msgType == MSGTYPE_PUT && (msgHeader.msgID == MSG_SUCCESSED || msgHeader.msgID == MSG_SEND))){
            printf("send file error !\n");
            return 0;
        };
        msgHeader.info.fileData.file_order++;
    }while(msgHeader.msgID == MSG_SEND);
    close_socket(data_client);
    return 1;
}

// TODO: recv file
int recv_file_info_from_server(SOCKET sclient, MsgHeader *recvbuf){
    int iResult;
    iResult = recv(sclient, (char *)recvbuf, sizeof(MsgHeader), 0);
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