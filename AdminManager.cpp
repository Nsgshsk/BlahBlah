#include "AdminManager.h"
#include <iostream>

#include "Chat.h"
#include "ChatManager.h"
#include "DataRepository.h"
#include "User.h"

AdminManager::AdminManager(User* user, DataRepository* data) : MemberManager(user, data)
{
}

void AdminManager::delete_user_command(const String& user) const
{
    try
    {
        User& temp = data_->getUser(user);
        if (temp.getRole() == UserRole::ADMIN)
            throw std::logic_error("You don't have permission to do that!");

        for (size_t i = 0; i < temp.chats_count(); i++)
        {
            Chat& chat = data_->getChat(temp[i]);
            chat.removeParticipant(temp);
            if (!chat.getParticipantsCount())
                data_->removeChat(chat);
            else
            {
                const UserBase& participant = chat.getParticipants()[0];
                chat.setOwner(participant);
            }
        }
        data_->removeUser(temp);

        std::cout << user << " removed successfully!\n";
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void AdminManager::delete_chat_command(size_t index) const
{
    try
    {
        if (!index)
            throw std::invalid_argument("Invalid index");

        const Chat chat = data_->getAllChats()[index - 1];
        const List<UserBase>& participants = chat.getParticipants();
        for (size_t i = 0; i < participants.getSize(); i++)
            data_->getUser(participants[i].getHash()).remove_chat(chat.getHash());
        data_->removeChat(chat);
        std::cout << chat.getName() << " chat successfully deleted!\n";
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void AdminManager::view_all_chats_command() const
{
    std::cout << "Chats in system:\n";
    const List<Chat> chats = data_->getAllChats();
    if (chats.getSize() == 0)
    {
        std::cout << "No chats found!\n";
        return;
    }
    for (size_t i = 0; i < chats.getSize(); i++)
        std::cout << (i + 1) << ") " << chats[i] << '\n';
}

void AdminManager::delete_user_input() const
{
    String user;
    std::cin >> user;

    delete_user_command(user);
}

void AdminManager::delete_chat_input() const
{
    size_t index = 0;
    std::cin >> index;

    delete_chat_command(index);
}

void AdminManager::help_command()
{
    std::cout << "User ";
    MemberManager::help_command();
    std::cout << "Admin Commands:\n";
    std::cout << "\t*) delete_user <username>\n";
    std::cout << "\t*) delete_chat <chat_id shown in list>\n";
    std::cout << "\t*) view_all_chats\n";
}

void AdminManager::login()
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
            else if (input == "delete_user")
                delete_user_input();
            else if (input == "delete_chat")
                delete_chat_input();
            else if (input == "view_all_chats")
                view_all_chats_command();
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
