#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

Socket::Socket() : fd_(-1) {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd_ == -1, "socket create error!");
}

Socket::Socket(int fd) : fd_(fd) {
    errif(fd_ == -1, "socket create error!");
}

Socket::~Socket() {
    if(fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

void Socket::Bind(InetAddress *inetaddress) {
    errif(bind(fd_, (sockaddr*)&inetaddress->addr_, inetaddress->addr_len_) == -1, "socket bind error!");
}

void Socket::Listen() {
    errif(listen(fd_, SOMAXCONN) == -1, "socket listen error!");
}

void Socket::Setnonblocking() {
    fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK);
}

int Socket::Accept(InetAddress *inetaddress) {
    int clnt_sockfd = accept(fd_, (sockaddr*)&inetaddress->addr_, &inetaddress->addr_len_);
    errif(clnt_sockfd == -1, "socket accept error!");
    return clnt_sockfd;
}

int Socket::Getfd() {
    return fd_;
}
