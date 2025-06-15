#include "MemberManager.h"
#include <iostream>
#include "User.h"
#include "Chat.h"

void MemberManager::help_command()
{
    std::cout << "Commands:\n";
    std::cout << "\t1) help\n";
    std::cout << "\t2) view_chats\n";
    std::cout << "\t3) select_chat\n";
    std::cout << "\t4) create_chat\n";
    std::cout << "\t5) change_password\n";
    std::cout << "\t6) info\n";
    std::cout << "\t7) logout\n";
}

void MemberManager::info_command()
{
}

MemberManager::MemberManager(User* user, DataRepository* data) : BaseManager(user, data)
{
}

void MemberManager::login()
{
    std::cout << "Logging in...\n";
    std::cout << *user_ << '\n';
    String input;
    while (true)
    {
        try
        {
            std::cout << "> ";
            std::cin >> input;

            if (input == "logout")
                break;

            if (input == "help")
                help_command();
            else if (input == "change_password")
                change_password_input();
            else if (input == "info")
                info_command();
            else if (input == "view_chats")
                view_chats_command();
            else if (input == "create_chat")
                create_chat_input();
            else if (input == "select_chat")
                select_chat_input();
            else
                std::cout << "Invalid command!\n";
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << '\n';
        }
    }
    std::cout << "Logging out...\n";
}
