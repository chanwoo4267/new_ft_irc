#include "../includes/Utility.hpp"

/**
 * @brief 메시지 출력
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
 * @brief 메시지 출력
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
        std::cerr << RED << "[ERROR] : " << CYAN << "Client " << client_socket << " : " << DEFAULT << message << std::endl;
    }
    else if (mode == 1)
    {
        std::cout << YELLOW << "[WARNING] : " << CYAN << "Client " << client_socket << " : " << DEFAULT << message << std::endl;
    }
    else if (mode == 2)
    {
        std::cout << GREEN << "[LOG] : " << CYAN << "Client " << client_socket << " : " << DEFAULT << message << std::endl;
    }
    else
    {
        std::cerr << BLUE << "[UNKNOWN] : " << CYAN << "Client " << client_socket << " : " << DEFAULT << message << std::endl;
    }
}