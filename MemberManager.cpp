#include "MemberManager.h"
#include <iostream>
#include "User.h"
#include "Chat.h"
#include "ChatManager.h"
#include "DataRepository.h"
#include "DirectChat.h"
#include "GroupChat.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

void MemberManager::select_chat_command(size_t chat_index) const
{
    try
    {
        size_t chats_count = user_->chats_count();
        if (chat_index < 1 || chat_index > chats_count)
            throw std::invalid_argument("There isn't such chat!");

        chat_index--;
        Chat& chat = data_->getChat((*user_)[chat_index]);
        ChatManager chat_manager(user_, &chat, data_);
        chat_manager.login();
        std::cout << *user_ << '\n';
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void MemberManager::create_chat_command(const String& input) const
{
    try
    {
        List<String> arguments = input.split(" ");

        bool isDirect = false;
        bool hasName = !data_->hasUser(arguments[0]);
        if (!hasName && arguments.getSize() < 2)
        {
            isDirect = true;
            User& participant = data_->getUser(arguments[0]);
            if (participant == *user_)
                throw std::invalid_argument("You can't start chat with yourself!");
            DirectChat chat(participant, *user_);
            data_->addChat(chat);
            user_->add_chat(chat.getHash());
            participant.add_chat(chat.getHash());
        }
        else
        {
            List<UserBase> participants;
            User** temp = new User*[arguments.getSize()];
            for (size_t i = hasName; i < arguments.getSize(); i++)
            {
                User& participant = data_->getUser(arguments[i]);
                if (participant == *user_)
                    throw std::invalid_argument("You can't start chat with yourself!");
                participants.add(participant);
                temp[i] = &participant;
            }
            GroupChat chat(participants, *user_, hasName ? arguments[0] : "");
            data_->addChat(chat);
            for (size_t i = hasName; i < arguments.getSize(); i++)
                temp[i]->add_chat(chat.getHash());
            user_->add_chat(chat.getHash());

            delete[] temp;
        }

        std::cout << "Chat " << (isDirect ? "with " : "") << arguments[0] << " created successfully!\n";
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void MemberManager::view_chats_command() const
{
    std::cout << "Chats:\n";
    try
    {
        size_t chats_count = user_->chats_count();
        if (chats_count == 0)
        {
            std::cout << "You haven't started any chats yet\n";
            return;
        }

        for (size_t i = 0; i < chats_count; i++)
        {
            const Chat& chat = data_->getChat((*user_)[i]);
            std::cout << (i + 1) << ") ";
            if (chat.getType() == ChatType::DIRECT)
            {
                const List<UserBase>& participants = chat.getParticipants();
                for (size_t j = 0; j < participants.getSize(); j++)
                    if (participants[j] != *user_)
                        std::cout << participants[j].getName();
            }
            std::cout << chat;
            std::cout << '\n';
        }

        std::cout << '\n';
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void MemberManager::select_chat_input() const
{
    size_t index = 0;
    std::cin >> index;

    select_chat_command(index);
}

void MemberManager::create_chat_input() const
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
    std::cout << "\t*) select_chat <chat_id shown in list>\n";
    std::cout << "\t*) create_chat <usernames seperated by space (minimum one username)>\n";
    std::cout << "\t*) change_password <old_password> <new_password>\n";
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
    try
    {
        String input;
        while (true)
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
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    std::cout << "Logging out...\n";
}
