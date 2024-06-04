#pragma once

#include "Channel.hpp"

#include <unordered_map>
#include <memory>

class ChannelHolder
{
public:
    ChannelHolder();
    ~ChannelHolder();

    Channel* getChannel(std::string ChannelName = "Default");

private:
    std::unordered_map<std::string, std::shared_ptr<Channel>> Channels;
};