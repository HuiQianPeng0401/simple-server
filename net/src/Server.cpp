#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <functional>
#include <cstring>
#include <unistd.h>

#define READ_BUFFER 1024

Server::Server(EventLoop *loop) : loop_(loop) {
    Socket *serv_socket = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_socket->Bind(serv_addr);
    serv_socket->Listen();
    serv_socket->Setnonblocking();
   
    Channel *serv_channel = new Channel(loop_, serv_socket->Getfd());
    std::function<void()> callback = std::bind(&Server::NewConnection, this, serv_socket);
    serv_channel->SetCallback(callback);
    serv_channel->EnableReading();
}

Server::~Server() {}

void Server::HandleReadEvent(int sockfd) {
    char buf[READ_BUFFER];
    while(true) {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0) {
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR) {
            printf("reading continue...");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            printf("finish once reading!");
            break;
        } else if(bytes_read == 0) {
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }
    }
}

void Server::NewConnection(Socket *serv_socket) {
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_socket = new Socket(serv_socket->Accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_socket->Getfd(), inet_ntoa(clnt_addr->addr_.sin_addr), ntohs(clnt_addr->addr_.sin_port));
    clnt_socket->Setnonblocking();
    Channel *clnt_channel = new Channel(loop_, clnt_socket->Getfd());
    std::function<void()> callback = std::bind(&Server::HandleReadEvent, this, clnt_socket->Getfd());
    clnt_channel->SetCallback(callback);
    clnt_channel->EnableReading();
}
