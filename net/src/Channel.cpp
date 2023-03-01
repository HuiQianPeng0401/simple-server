#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *loop, int fd) : loop_(loop), fd_(fd),
    events_(0), revents_(0), inepoll_(false) {}

Channel::~Channel() {}

void Channel::HandleEvent() {
    callback_();
}

void Channel::EnableReading() {
    events_ = EPOLLIN | EPOLLET;
    loop_->UpdateChannel(this);
}

int Channel::Getfd() {
    return fd_;
}

uint32_t Channel::GetEvents() {
    return events_;
}

uint32_t Channel::GetRevents() {
    return revents_;
}

bool Channel::GetInEpoll() {
    return inepoll_;
}

void Channel::SetInEpoll() {
    inepoll_ = true;
}

void Channel::SetRevents(uint32_t ev) {
    revents_ = ev;
}

void Channel::SetCallback(std::function<void()> cb) {
    callback_ = cb;
}
