#include "ChannelHolder.hpp"


ChannelHolder::ChannelHolder()
{

}

ChannelHolder::~ChannelHolder()
{

}

Channel* ChannelHolder::getChannel(std::string ChannelName)
{
    if (Channels.contains(ChannelName) == false)
    {
        Channels[ChannelName] = std::make_shared<Channel>(ChannelName);
    }

    return Channels[ChannelName].get();
}