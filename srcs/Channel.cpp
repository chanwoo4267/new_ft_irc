#include "../includes/Channel.hpp"

/**
 * @brief Channel 객체 초기화 생성자
 * 
 * @param channel_name 채널 이름
*/
Channel::Channel(std::string channel_name)
{
    _channel_name = channel_name;
    _channel_topic = "";
    _channel_password = "";
    _channel_user_limit = 0;
    _channel_mode_i = false;
    _channel_mode_t = false;
    _channel_mode_l = false;
    _channel_mode_k = false;
}

/**
 * @brief 채널에 멤버 추가
 * 
 * @param nickname 추가할 멤버의 닉네임
 * 
 * @warning 중복된 멤버 추가가 가능하므로 중복 체크는 따로 해야한다
*/
void Channel::addChannelMember(std::string nickname)
{
    _channel_member_list.push_back(nickname);
}

/**
 * @brief 채널에서 멤버 삭제
 * 
 * @param nickname 삭제할 멤버의 닉네임
 * 
 * @note 해당 멤버가 없으면 아무것도 하지 않는다
*/
void Channel::deleteChannelMember(std::string nickname)
{
    std::vector<std::string>::iterator it = std::find(_channel_member_list.begin(), _channel_member_list.end(), nickname);
    if (it != _channel_member_list.end())
        _channel_member_list.erase(it);
}

/**
 * @brief 채널에 오퍼레이터 추가
 * 
 * @param nickname 추가할 오퍼레이터의 닉네임
 * 
 * @warning 중복된 오퍼레이터 추가가 가능하므로 중복 체크는 따로 해야한다
*/
void Channel::addChannelOperator(std::string nickname)
{
    _channel_operator_list.push_back(nickname);
}

/**
 * @brief 채널에서 오퍼레이터 삭제
 * 
 * @param nickname 삭제할 오퍼레이터의 닉네임
 * 
 * @note 해당 오퍼레이터가 없으면 아무것도 하지 않는다
*/
void Channel::deleteChannelOperator(std::string nickname)
{
    std::vector<std::string>::iterator it = std::find(_channel_operator_list.begin(), _channel_operator_list.end(), nickname);
    if (it != _channel_operator_list.end())
        _channel_operator_list.erase(it);
}

/**
 * @brief 채널에 멤버가 존재하는지 확인
 * 
 * @param nickname 확인할 멤버의 닉네임
 * 
 * @return 멤버가 있으면 true, 없으면 false
*/
bool Channel::isMemberExist(std::string nickname)
{
    std::vector<std::string>::iterator it = std::find(_channel_member_list.begin(), _channel_member_list.end(), nickname);
    if (it != _channel_member_list.end())
        return (true);
    return (false);
}

/**
 * @brief 채널에 오퍼레이터가 존재하는지 확인
 * 
 * @param nickname 확인할 오퍼레이터의 닉네임
 * 
 * @return 오퍼레이터가 있으면 true, 없으면 false
*/
bool Channel::isOperatorExist(std::string nickname)
{
    std::vector<std::string>::iterator it = std::find(_channel_operator_list.begin(), _channel_operator_list.end(), nickname);
    if (it != _channel_operator_list.end())
        return (true);
    return (false);
}

std::string Channel::getChannelName() const
{
    return (_channel_name);
}

std::string Channel::getChannelTopic() const
{
    return (_channel_topic);
}

std::string Channel::getChannelPassword() const
{
    return (_channel_password);
}

int Channel::getChannelUserLimit() const
{
    return (_channel_user_limit);
}


bool Channel::getChannelModeI() const
{
    return (_channel_mode_i);
}

bool Channel::getChannelModeT() const
{
    return (_channel_mode_t);
}

bool Channel::getChannelModeL() const
{
    return (_channel_mode_l);
}

bool Channel::getChannelModeK() const
{
    return (_channel_mode_k);
}

void Channel::setChannelName(std::string channel_name)
{
    _channel_name = channel_name;
}

void Channel::setChannelTopic(std::string channel_topic)
{
    _channel_topic = channel_topic;
}

void Channel::setChannelPassword(std::string channel_password)
{
    _channel_password = channel_password;
}

void Channel::setChannelUserLimit(int channel_user_limit)
{
    _channel_user_limit = channel_user_limit;
}

void Channel::setChannelModeI(bool channel_mode_i)
{
    _channel_mode_i = channel_mode_i;
}

void Channel::setChannelModeT(bool channel_mode_t)
{
    _channel_mode_t = channel_mode_t;
}

void Channel::setChannelModeL(bool channel_mode_l)
{
    _channel_mode_l = channel_mode_l;
}

void Channel::setChannelModeK(bool channel_mode_k)
{
    _channel_mode_k = channel_mode_k;
}