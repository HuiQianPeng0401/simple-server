#pragma once

class InetAddress;
class Socket
{
public:
    Socket();
    Socket(int);
    ~Socket();
    void Bind(InetAddress *);
    void Listen();
    void Setnonblocking();
    int Accept(InetAddress *);
    int Getfd();

private:
    int fd_;
};

