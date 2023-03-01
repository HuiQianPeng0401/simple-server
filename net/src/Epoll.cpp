#include "Epoll.h"
#include "util.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>

#define MAX_EVENTS 1000

Epoll::Epoll() : epfd_(-1), events_(nullptr) {
    epfd_ = epoll_create1(0);
    errif(epfd_ == -1, "epoll create error!");
    events_ = new epoll_event[MAX_EVENTS];
    bzero(events_, sizeof(*events_) * MAX_EVENTS);
}

Epoll::~Epoll() {
    if(epfd_ != -1) {
        close(epfd_);
        epfd_ = -1;
    }
    delete[] events_;
}

void Epoll::Addfd(int fd, uint32_t op) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    errif(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error!");
}

std::vector<Channel *> Epoll::Poll(int timeout) {
    std::vector<Channel *> activechannels;
    int nfds = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
    for(int i = 0; i < nfds; ++i) {
        Channel *ch = (Channel*)events_[i].data.ptr;
        ch->SetRevents(events_[i].events);
        activechannels.push_back(ch);
    }
    return activechannels;
}

void Epoll::UpdateChannel(Channel *channel) {
    int fd = channel->Getfd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->GetEvents();
    if(!channel->GetInEpoll()) {
        errif(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error!");
        channel->SetInEpoll();
    } else {
        errif(epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll mod error!");
    }
}
