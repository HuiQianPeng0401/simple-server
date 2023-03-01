#pragma once
#include <sys/epoll.h>
#include <functional>

class EventLoop;
class Channel
{
public:
    Channel(EventLoop *loop, int fd);
    ~Channel();
    void HandleEvent();
    void EnableReading();
    int Getfd();
    uint32_t GetEvents();
    uint32_t GetRevents();
    bool GetInEpoll();
    void SetInEpoll();
    void SetRevents(uint32_t);
    void SetCallback(std::function<void()>);

private:
    EventLoop *loop_;
    int fd_;
    uint32_t events_;
    uint32_t revents_;
    bool inepoll_;
    std::function<void()> callback_;
};

