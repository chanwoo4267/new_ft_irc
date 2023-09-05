#include "../includes/Utility.hpp"

/**
 * @brief 메시지 출력, 줄바꿈 자동
 * 
 * @param mode 메시지 모드
 * @param message 출력할 메시지
 * 
 * @note
 * mode 0 : 오류
 * @note
 * mode 1 : 경고
 * @note
 * mode 2 : 로그
*/
void printServerMessage(int mode, std::string message)
{
    if (mode == 0)
    {
        std::cerr << RED << "[ERROR] : " << DEFAULT << message << std::endl;
    }
    else if (mode == 1)
    {
        std::cout << YELLOW << "[WARNING] : " << DEFAULT << message << std::endl;
    }
    else if (mode == 2)
    {
        std::cout << GREEN << "[LOG] : " << DEFAULT << message << std::endl;
    }
    else
    {
        std::cerr << BLUE << "[UNKNOWN] : " << DEFAULT << message << std::endl;
    }
}

/**
 * @brief 메시지 출력, 줄바꿈 자동
 * 
 * @param mode 메시지 모드
 * @param client_socket 클라이언트 소켓
 * @param message 출력할 메시지
 * 
 * @note
 * mode 0 : 치명적 오류
 * @note
 * mode 1 : 명령 실패
 * @note
 * mode 2 : 명령 성공
*/
void printCommandMessage(int mode, int client_socket, std::string message)
{
    if (mode == 0)
    {
        std::cerr << RED << "[CRITICAL ERROR] : " << CYAN << "Client " << client_socket << " : " << DEFAULT << message << std::endl;
    }
    else if (mode == 1)
    {
        std::cout << YELLOW << "[FAIL] : " << CYAN << "Client " << client_socket << " : " << DEFAULT << message << std::endl;
    }
    else if (mode == 2)
    {
        std::cout << GREEN << "[SUCCESS] : " << CYAN << "Client " << client_socket << " : " << DEFAULT << message << std::endl;
    }
    else
    {
        std::cerr << BLUE << "[UNKNOWN] : " << CYAN << "Client " << client_socket << " : " << DEFAULT << message << std::endl;
    }
}

/**
 * @brief 문자열을 delim을 기준으로 분리
 * 
 * @param str 분리할 문자열
 * @param delim 구분자
 * 
 * @return 분리된 문자열
*/
std::vector<std::string> splitString(std::string str, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string tmp;

    while (getline(ss, tmp, delim))
        result.push_back(tmp);
    return (result);
}

/**
 * @brief 문자열을 CRLF를 기준으로 분리
 * 
 * @param str 분리할 문자열
 * 
 * @return 분리된 문자열
 * 
 * @note 줄바꿈은 제거해준다.
*/
std::vector<std::string> splitStringWithCRLF(std::string str)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string tmp;

    while (getline(ss, tmp, '\n'))
    {
        if (tmp[tmp.length() - 1] == '\r')
            tmp.erase(tmp.length() - 1);
        result.push_back(tmp);
    }
    return (result);
}