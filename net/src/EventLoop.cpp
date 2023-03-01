#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>

EventLoop::EventLoop() : ep_(nullptr), quit_(false) {
    ep_ = new Epoll();
}

void EventLoop::Loop() {
    while(!quit_) {
        std::vector<Channel *> channels;
        channels = ep_->Poll();
        for(auto it = channels.begin(); it != channels.end(); ++it) {
            (*it)->HandleEvent();
        }
    }
}

void EventLoop::UpdateChannel(Channel *channel) {
    ep_->UpdateChannel(channel);
}
