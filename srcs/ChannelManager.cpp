#include "../includes/ChannelManager.hpp"

/**
 * @brief 채널 목록에 채널 추가, 주어진 닉을 유저 및 오퍼레이터로 추가
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
 * @brief 주어진 채널에 주어진 클라이언트 추가
 * 
 * @param channel_name 추가할 채널의 이름
 * @param nickname 추가할 클라이언트의 닉네임
 * 
 * @warning 채널이 존재하는지 확인 후 사용해야한다
*/
void ChannelManager::addClientToChannel(std::string channel_name, std::string nickname)
{
    Channel& channel = getChannel(channel_name);
    channel.addChannelMember(nickname);
}

/**
 * @brief 주어진 채널에 주어진 클라이언트를 오퍼레이터로 추가
 * 
 * @param channel_name 추가할 채널의 이름
 * @param nickname 추가할 클라이언트의 닉네임
 * 
 * @warning 채널이 존재하는지 확인 후 사용해야한다
*/
void ChannelManager::addOperToChannel(std::string channel_name, std::string nickname)
{
    Channel& channel = getChannel(channel_name);
    channel.addChannelOperator(nickname);
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
 * @brief 주어진 채널에서 주어진 클라이언트 삭제
 * 
 * @param channel_name 삭제할 채널의 이름
 * @param nickname 삭제할 클라이언트의 닉네임
 * 
 * @warning 채널이 존재하는지 확인 후 사용해야한다
*/
void ChannelManager::deleteClientFromChannel(std::string channel_name, std::string nickname)
{
    Channel& channel = getChannel(channel_name);
    channel.deleteChannelMember(nickname);
}

/**
 * @brief 주어진 채널에서 주어진 클라이언트를 오퍼레이터에서 삭제
 * 
 * @param channel_name 삭제할 채널의 이름
 * @param nickname 삭제할 클라이언트의 닉네임
 * 
 * @warning 채널이 존재하는지 확인 후 사용해야한다
*/
void ChannelManager::deleteOperFromChannel(std::string channel_name, std::string nickname)
{
    Channel& channel = getChannel(channel_name);
    channel.deleteChannelOperator(nickname);
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

/**
 * @brief 주어진 채널에 주어진 클라이언트가 존재하는지 확인
 * 
 * @param channel_name 확인할 채널의 이름
 * @param nickname 확인할 클라이언트의 닉네임
 * 
 * @return 존재하면 true 없으면 false
 * 
 * @warning 채널이 존재하는지 확인 후 사용
*/
bool ChannelManager::isClientInChannel(std::string channel_name, std::string nickname)
{
    Channel& channel = getChannel(channel_name);
    return channel.isMemberExist(nickname);
}

/**
 * @brief 주어진 채널에 주어진 클라이언트가 오퍼레이터인지 확인
 * 
 * @param channel_name 확인할 채널의 이름
 * @param nickname 확인할 클라이언트의 닉네임
 * 
 * @return 오퍼레이터이면 true 아니면 false
 * 
 * @warning 채널이 존재하는지 확인 후 사용
*/
bool ChannelManager::isClientOperatorInChannel(std::string channel_name, std::string nickname)
{
    Channel& channel = getChannel(channel_name);
    return channel.isOperatorExist(nickname);
}

/**
 * @brief 주어진 채널의 멤버 목록 가져오기
 * 
 * @param channel_name 멤버 목록을 가져올 채널의 이름
 * 
 * @return 멤버 목록
 * 
 * @warning 채널이 존재하는지 확인 후 사용
*/
std::vector<std::string> ChannelManager::getChannelMemberList(std::string channel_name)
{
    Channel& channel = getChannel(channel_name);
    return channel.getChannelMemberList();
}

std::string ChannelManager::getChannelMemberListString(std::string channel_name)
{
    Channel& channel = getChannel(channel_name);
    return channel.getChannelMemberListString();
}

/**
 * @brief 주어진 채널의 모드 가져오기
 * 
 * @param channel_name 모드를 가져올 채널의 이름
 * @param mode 가져올 모드
 * 
 * @return 모드
 * 
 * @warning 채널이 존재하는지 확인 후 사용
 * @note mode는 I, K, L, T 중 하나여야 한다
*/
bool ChannelManager::getChannelMode(std::string channel_name, std::string mode)
{
    Channel& channel = getChannel(channel_name);
    if (mode == "I")
        return channel.getChannelModeI();
    else if (mode == "K")
        return channel.getChannelModeK();
    else if (mode == "L")
        return channel.getChannelModeL();
    else if (mode == "T")
        return channel.getChannelModeT();
    else
        return false;
}

/**
 * @brief 주어진 채널의 모드 설정하기
 * 
 * @param channel_name 모드를 설정할 채널의 이름
 * @param mode 설정할 모드
 * @param value 설정할 값
 * 
 * @warning 채널이 존재하는지 확인 후 사용
 * @note mode는 I, K, L, T 중 하나여야 한다
*/
void ChannelManager::setChannelMode(std::string channel_name, std::string mode, bool value)
{
    Channel& channel = getChannel(channel_name);
    if (mode == "I")
        channel.setChannelModeI(value);
    else if (mode == "K")
        channel.setChannelModeK(value);
    else if (mode == "L")
        channel.setChannelModeL(value);
    else if (mode == "T")
        channel.setChannelModeT(value);
}

/**
 * @brief 주어진 채널의 비밀번호 가져오기
 * 
 * @param channel_name 비밀번호를 가져올 채널의 이름
 * 
 * @return 비밀번호
 * 
 * @warning 채널이 존재하는지 확인 후 사용
*/
std::string ChannelManager::getChannelPassword(std::string channel_name)
{
    Channel& channel = getChannel(channel_name);
    return channel.getChannelPassword();
}

/**
 * @brief 주어진 채널의 유저 제한 가져오기
 * 
 * @param channel_name 유저 제한을 가져올 채널의 이름
 * 
 * @return 유저 제한
 * 
 * @warning 채널이 존재하는지 확인 후 사용
*/
unsigned long ChannelManager::getChannelUserLimit(std::string channel_name)
{
    Channel& channel = getChannel(channel_name);
    return channel.getChannelUserLimit();
}

/**
 * @brief 주어진 채널의 비밀번호 설정하기
 * 
 * @param channel_name 비밀번호를 설정할 채널의 이름
 * @param password 설정할 비밀번호
 * 
 * @warning 채널이 존재하는지 확인 후 사용
*/
void ChannelManager::setChannelPassword(std::string channel_name, std::string password)
{
    Channel& channel = getChannel(channel_name);
    channel.setChannelPassword(password);
}

/**
 * @brief 주어진 채널의 유저 제한 설정하기
 * 
 * @param channel_name 유저 제한을 설정할 채널의 이름
 * @param user_limit 설정할 유저 제한
 * 
 * @warning 채널이 존재하는지 확인 후 사용
*/
void ChannelManager::setChannelUserLimit(std::string channel_name, unsigned long user_limit)
{
    Channel& channel = getChannel(channel_name);
    channel.setChannelUserLimit(user_limit);
}

std::string ChannelManager::getChannelTopic(std::string channel_name)
{
    Channel& channel = getChannel(channel_name);
    return channel.getChannelTopic();
}

void ChannelManager::setChannelTopic(std::string channel_name, std::string topic)
{
    Channel& channel = getChannel(channel_name);
    channel.setChannelTopic(topic);
}

/**
 * @brief 주어진 클라이언트를 모든 채널에서 삭제
 * 
 * @param nickname 삭제할 클라이언트의 닉네임
 * 
 * @note 클라이언트를 채널의 멤버목록, 오퍼레이터목록에서 삭제하고, 채널의 멤버목록이 비어있으면 채널을 삭제한다
*/
void ChannelManager::deleteClientFromAllChannel(std::string nickname)
{
    std::vector<Channel>::iterator it = _channel_list.begin();
    for (; it != _channel_list.end(); )
    {
        if (it->isMemberExist(nickname))
        {
            it->deleteChannelMember(nickname);
            it->deleteChannelOperator(nickname);

            if (it->getChannelMemberList().size() == 0)
            {
                printServerMessage(2, "Channel " + it->getChannelName() + " has been deleted, because it is empty");
                it = _channel_list.erase(it); // iteartor가 erase() 함수의 반환값을 가리키도록 한다
            }
            else
                ++it;
        }
        else
            ++it;
    }
}

void ChannelManager::changeNicknameInAllChannel(std::string old_nickname, std::string new_nickname)
{
    std::vector<Channel>::iterator it = _channel_list.begin();
    for (; it != _channel_list.end(); ++it)
    {
        if (it->isMemberExist(old_nickname))
        {
            it->deleteChannelMember(old_nickname);
            it->addChannelMember(new_nickname);
        }
        if (it->isOperatorExist(old_nickname))
        {
            it->deleteChannelOperator(old_nickname);
            it->addChannelOperator(new_nickname);
        }
    }
}