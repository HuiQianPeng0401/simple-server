#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll
{
public:
    Epoll();
    ~Epoll();
    void Addfd(int fd, uint32_t op);
    void UpdateChannel(Channel *);
    std::vector<Channel *> Poll(int timeout = -1);

private:
    int epfd_;
    struct epoll_event *events_;
};

