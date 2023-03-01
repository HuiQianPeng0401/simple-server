#pragma once
#include <arpa/inet.h>

class InetAddress
{
public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();
    struct sockaddr_in addr_;
    socklen_t addr_len_;
};

