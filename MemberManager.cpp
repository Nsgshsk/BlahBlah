#include "MemberManager.h"
#include <iostream>
#include "User.h"
#include "Chat.h"
#include "DataRepository.h"
#include "DirectChat.h"
#include "GroupChat.h"

void MemberManager::select_chat_command(size_t chat_index)
{
}

void MemberManager::create_chat_command(const String& input)
{
    try
    {
        List<String> arguments = input.split(" ");

        bool hasName = !data_->hasUser(arguments[0]);
        if (!hasName && arguments.getSize() < 2)
        {
            const User& participant = data_->getUser(arguments[0]);
            DirectChat chat(participant, *user_);
            data_->addChat(chat);
            *user_->add_chat(chat.getHash());
        }
        else
        {
            List<UserBase> participants;
            for (size_t i = hasName; i < arguments.getSize(); i++)
            {
                const User& participant = data_->getUser(arguments[i]);
                participants.add(participant);
            }
            GroupChat chat(participants, *user_, hasName ? arguments[0] : "");
            data_->addChat(chat);
        }

        std::cout << "Chat " << arguments[0] << " created successfully!\n";
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void MemberManager::view_chats_command()
{
}

void MemberManager::select_chat_input()
{
    size_t index = 0;
    std::cin >> index;

    select_chat_command(index);
}

void MemberManager::create_chat_input()
{
    String input;
    std::cin.ignore();
    String::getline(std::cin, input);

    create_chat_command(input);
}

void MemberManager::help_command()
{
    std::cout << "Commands:\n";
    std::cout << "\t*) help\n";
    std::cout << "\t*) view_chats\n";
    std::cout << "\t*) select_chat\n";
    std::cout << "\t*) create_chat\n";
    std::cout << "\t*) change_password\n";
    std::cout << "\t*) info\n";
    std::cout << "\t*) logout\n";
}

void MemberManager::info_command()
{
    std::cout << *user_ << '\n';
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
