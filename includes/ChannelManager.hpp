#ifndef CHANNELMANAGER_HPP
#define CHANNELMANAGER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "../includes/Channel.hpp"
#include "../includes/Utility.hpp"
class Channel;

class ChannelManager
{
    private:
        std::vector<Channel>        _channel_list;
        Channel&                    getChannel(std::string channel_name);

    public:
        void                        addChannel(std::string channel_name, std::string oper_nick);
        void                        addClientToChannel(std::string channel_name, std::string nickname);
        void                        addOperToChannel(std::string channel_name, std::string nickname);
        void                        deleteChannel(std::string channel_name);
        void                        deleteClientFromChannel(std::string channel_name, std::string nickname);
        void                        deleteOperFromChannel(std::string channel_name, std::string nickname);
        void                        deleteClientFromAllChannel(std::string nickname);

        bool                        isChannelExist(std::string channel_name);
        bool                        isClientInChannel(std::string channel_name, std::string nickname);
        bool                        isClientOperatorInChannel(std::string channel_name, std::string nickname);

        std::vector<std::string>    getChannelMemberList(std::string channel_name);
        std::string                 getChannelMemberListString(std::string channel_name);
        bool                        getChannelMode(std::string channel_name, std::string mode);
        std::string                 getChannelPassword(std::string channel_name);
        unsigned long               getChannelUserLimit(std::string channel_name);
        std::string                 getChannelTopic(std::string channel_name);

        void                        setChannelMode(std::string channel_name, std::string mode, bool value);
        void                        setChannelPassword(std::string channel_name, std::string password);
        void                        setChannelUserLimit(std::string channel_name, unsigned long user_limit);
        void                        setChannelTopic(std::string channel_name, std::string topic);

        void                        changeNicknameInAllChannel(std::string old_nickname, std::string new_nickname);
};

#endif