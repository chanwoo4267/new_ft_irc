#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>

// mode command is only for operators of channel
// mode #ch [+|-] [i|t] -> toggle invite-only, toggle topic changable only by opers
// mode #ch +k password / mode #ch -k -> set password
// mode #ch +l user_limit / mode #ch -l -> set user limit
// mode #ch [+|-] o nick -> give/remove oper privileges

class Channel
{
    private:
        std::string                 _channel_name;
        std::vector<std::string>    _channel_member_list;
        std::vector<std::string>    _channel_operator_list;
        std::string                 _channel_topic;

        std::string                 _channel_password;
        int                         _channel_user_limit;
        bool                        _channel_mode_i;
        bool                        _channel_mode_t;
        bool                        _channel_mode_l;
        bool                        _channel_mode_k;
    
    public:
            
            Channel(std::string channel_name);

            void                        addChannelMember(std::string nickname);
            void                        deleteChannelMember(std::string nickname);
            void                        addChannelOperator(std::string nickname);
            void                        deleteChannelOperator(std::string nickname);
            bool                        isMemberExist(std::string nickname);
            bool                        isOperatorExist(std::string nickname);
    
            /* get methods */
            std::string                 getChannelName() const;
            std::string                 getChannelTopic() const;
            std::string                 getChannelPassword() const;
            int                         getChannelUserLimit() const;
            bool                        getChannelModeI() const;
            bool                        getChannelModeT() const;
            bool                        getChannelModeL() const;
            bool                        getChannelModeK() const;
    
            /* set methods */
            void                        setChannelName(std::string channel_name);
            void                        setChannelTopic(std::string channel_topic);
            void                        setChannelPassword(std::string channel_password);
            void                        setChannelUserLimit(int channel_user_limit);
            void                        setChannelModeI(bool channel_mode_i);
            void                        setChannelModeT(bool channel_mode_t);
            void                        setChannelModeL(bool channel_mode_l);
            void                        setChannelModeK(bool channel_mode_k);
};

#endif