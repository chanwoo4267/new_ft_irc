#ifndef CHANNELMANAGER_HPP
#define CHANNELMANAGER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "../includes/Channel.hpp"
class Channel;

class ChannelManager
{
    private:
        std::vector<Channel>    _channel_list;

    public:
        void                    addChannel(std::string channel_name, std::string oper_nick);
        void                    deleteChannel(std::string channel_name);
        bool                    isChannelExist(std::string channel_name);
        Channel&                getChannel(std::string channel_name);
};

#endif