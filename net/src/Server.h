#pragma once

class EventLoop;
class Socket;
class Server
{
public:
    Server(EventLoop *);
    ~Server();
    void HandleReadEvent(int); // 处理可读事件
    void NewConnection(Socket *); // 处理新客户端连接

private:
    EventLoop *loop_;
};
