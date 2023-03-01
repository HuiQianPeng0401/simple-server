#pragma once

class Epoll;
class Channel;
class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
    void Loop();
    void UpdateChannel(Channel *);

private:
    Epoll *ep_;
    bool quit_;
};

