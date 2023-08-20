#include "../includes/ChannelManager.hpp"

/**
 * @brief 채널 목록에 채널 추가
 * 
 * @param channel_name 추가할 채널의 이름
 * @param oper_nick 추가할 채널의 오퍼레이터 닉네임
 * 
 * @warning 중복된 채널 추가가 가능하므로 중복 체크는 따로 해야한다
*/
void ChannelManager::addChannel(std::string channel_name, std::string oper_nick)
{
    Channel new_channel(channel_name);
    new_channel.addChannelMember(oper_nick);
    new_channel.addChannelOperator(oper_nick);
    _channel_list.push_back(new_channel);
}

/**
 * @brief 채널 목록에서 채널 삭제
 * 
 * @param channel_name 삭제할 채널의 이름
 * 
 * @note 해당 채널이 없으면 아무것도 하지 않는다
*/
void ChannelManager::deleteChannel(std::string channel_name)
{
    std::vector<Channel>::iterator it = _channel_list.begin();
    for (; it != _channel_list.end(); ++it)
    {
        if (it->getChannelName() == channel_name)
        {
            _channel_list.erase(it);
            break;
        }
    }
}

/**
 * @brief 채널 목록에 채널이 존재하는지 확인
 * 
 * @param channel_name 확인할 채널의 이름
 * 
 * @return 존재하면 true 없으면 false
*/
bool ChannelManager::isChannelExist(std::string channel_name)
{
    std::vector<Channel>::iterator it = _channel_list.begin();
    for (; it != _channel_list.end(); ++it)
    {
        if (it->getChannelName() == channel_name)
            return true;
    }
    return false;
}

/**
 * @brief 채널 목록에서 채널 가져오기
 * 
 * @param channel_name 가져올 채널의 이름
 * 
 * @return 가져올 채널
 * 
 * @warning 채널이 존재하는지 확인 후 사용해야한다
*/
Channel& ChannelManager::getChannel(std::string channel_name)
{
    std::vector<Channel>::iterator it = _channel_list.begin();
    for (; it != _channel_list.end(); ++it)
    {
        if (it->getChannelName() == channel_name)
            return *it;
    }
    return *it;
}