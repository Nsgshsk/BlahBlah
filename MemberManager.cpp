#include "MemberManager.h"
#include <iostream>

void MemberManager::help_command()
{
    std::cout << "Commands:\n";
    std::cout << "\t1) help\n";
    std::cout << "\t2) view_chats\n";
    std::cout << "\t3) new_chat\n";
    std::cout << "\t3) change_password\n";
    std::cout << "\t4) info\n";
    std::cout << "\t5) logout\n";
}

void MemberManager::info_command()
{
}

MemberManager::MemberManager(User* user, DataRepository* data) : BaseManager(user, data)
{
}

void MemberManager::login()
{
    String input;
    while (true)
    {
        try
        {
            std::cout << "> ";
            std::cin >> input;

            if (input == "exit")
                break;

            if (input == "help")
                help_command();
            else if (input == "change_password")
                change_password_input();
            else if (input == "info")
                info_command();
            else
                std::cout << "Invalid command!\n";
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << '\n';
        }
    }
}
