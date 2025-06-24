#include "ChatManager.h"
#include <iostream>
#include "Chat.h"
#include "DataRepository.h"
#include "User.h"

void ChatManager::rename_chat_command(const String& name) const
{
    try
    {
        if (chat_->getType() == ChatType::DIRECT)
            throw std::logic_error("You don't have permission to do that.");
        chat_->setName(name);
        std::cout << "Renaming chat to " << name << '\n';
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void ChatManager::rename_chat_input() const
{
    String name;
    std::cin >> name;

    rename_chat_command(name);
}

void ChatManager::sent_message_command(const String& message) const
{
    try
    {
        chat_->sentMessage(*user_, message);
        std::cout << "Message sent successfully!\n";
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void ChatManager::invite_command(const String& user) const
{
    try
    {
        User& temp = data_->getUser(user);
        if (!chat_->invitation_control_status() || isOwner_)
        {
            chat_->addParticipant(temp);
            temp.add_chat(chat_->getHash());
            std::cout << temp.getName() << " successfully added to " << chat_->getName() << " chat.\n";
        }
        else
        {
            chat_->inviteParticipant(temp);
            std::cout << "Successfully requested " << temp.getName() << " to be added to " << chat_->getName() <<
                " chat.\n";
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void ChatManager::kick_command(const String& user) const
{
    try
    {
        if (chat_->getType() == ChatType::DIRECT)
            throw std::logic_error("You don't have permission to do that.");

        User& temp = data_->getUser(user);
        if (!isOwner_ || chat_->isOwner(temp))
            throw std::logic_error("You don't have permission to kick!");

        chat_->removeParticipant(temp);
        temp.remove_chat(chat_->getHash());
        std::cout << temp.getName() << " successfully removed from " << chat_->getName() << " chat.\n";
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void ChatManager::transfer_ownership_command(const String& user)
{
    try
    {
        if (chat_->getType() == ChatType::DIRECT)
            throw std::logic_error("You don't have permission to do that.");
        if (!isOwner_)
            throw std::logic_error("You must own this chat.");

        const User& temp = data_->getUser(user);
        chat_->setOwner(temp);
        isOwner_ = false;
        std::cout << "Chat " << chat_->getName() << " is now owned by " << temp.getName() << '\n';
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void ChatManager::resolve_invite_command(const String& invite, const String& accepted) const
{
    try
    {
        if (chat_->getType() == ChatType::DIRECT)
            throw std::logic_error("You don't have permission to do that.");

        User& temp = data_->getUser(invite);
        if (!isOwner_)
            throw std::logic_error("You must own this chat.");
        if (!chat_->invitation_control_status())
            throw std::logic_error("This feature is currently disabled!");

        bool status;
        if (accepted.toLower() == "accept")
            status = true;
        else if (accepted.toLower() == "reject")
            status = false;
        else
            throw std::invalid_argument("Invalid invitation review acceptance status!");

        chat_->review_invitation(temp, status);
        if (status)
            temp.add_chat(chat_->getHash());
        std::cout << temp.getName() << " invitation " << accepted.toLower() << "ed\n";
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void ChatManager::view_messages_command() const
{
    std::cout << "Messages:\n";
    std::cout << chat_->getMessages();
}

void ChatManager::leave_chat_command() const
{
    try
    {
        bool last = chat_->getParticipantsCount() == 1;
        if (isOwner_ && !last)
        {
            const List<UserBase>& participants = chat_->getParticipants();
            for (size_t i = 0; i < participants.getSize(); i++)
                if (participants[i] != *user_)
                {
                    chat_->setOwner(participants[i]);
                    break;
                }
        }
        chat_->removeParticipant(*user_);
        user_->remove_chat(chat_->getHash());
        if (last)
            data_->removeChat(*chat_);

        std::cout << "Left " << chat_->getName() << "chat successfully!\n";
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void ChatManager::invites_status_command() const
{
    try
    {
        if (chat_->getType() == ChatType::DIRECT)
            throw std::logic_error("You don't have permission to do that.");

        std::cout << "Invites are"
            << (chat_->invitation_control_status() ? "" : " not")
            << " controlled by Owner.\n";
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void ChatManager::toggle_invites_command() const
{
    try
    {
        if (chat_->getType() == ChatType::DIRECT)
            throw std::logic_error("You don't have permission to do that.");
        if (!isOwner_)
            throw std::logic_error("You must own this chat!");
        chat_->switch_invitation_control();
        invites_status_command();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}

void ChatManager::view_invites_command() const
{
    try
    {
        if (chat_->getType() == ChatType::DIRECT)
            throw std::logic_error("You don't have permission to do that.");
        std::cout << "Viewing invites...\n";
        const List<UserBase> invites = chat_->get_pending_invitations();
        if (invites.getSize() == 0)
            std::cout << "No pending invites.\n";

        for (size_t i = 0; i < invites.getSize(); i++)
            std::cout << "*) " << invites[i] << " - pending\n";

        std::cout << '\n';
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}

void ChatManager::sent_message_input() const
{
    String message;
    String::getline(std::cin, message);

    sent_message_command(message);
}

void ChatManager::invite_input() const
{
    String user;
    std::cin >> user;

    invite_command(user);
}

void ChatManager::kick_input() const
{
    String user;
    std::cin >> user;

    kick_command(user);
}

void ChatManager::transfer_ownership_input()
{
    String user;
    std::cin >> user;

    transfer_ownership_command(user);
}

void ChatManager::resolve_invite_input() const
{
    String invite;
    std::cin >> invite;
    String accepted;
    std::cin >> accepted;

    resolve_invite_command(invite, accepted);
}

void ChatManager::help_command()
{
    std::cout << "Commands:\n";
    std::cout << "\t*) help\n";
    std::cout << "\t*) view_messages\n";
    std::cout << "\t*) sent_message <message>\n";
    std::cout << "\t*) invite <username>\n";
    std::cout << "\t*) leave_chat\n";
    if (isOwner_ && chat_->getType() == ChatType::GROUP)
    {
        std::cout << "\t*) invites_status\n";
        std::cout << "\t*) rename_chat <name>";
        std::cout << "\t*) kick <username>\n";
        std::cout << "\t*) transfer_ownership <username>\n";
        std::cout << "\t*) toggle_invites\n";
        if (chat_->invitation_control_status())
        {
            std::cout << "\t*) view_invites\n";
            std::cout << "\t*) resolve_invite <username shown in list>\n";
        }
    }
    std::cout << "\t*) info\n";
    std::cout << "\t*) logout\n";
}

void ChatManager::info_command()
{
    const List<UserBase>& participants = chat_->getParticipants();
    if (chat_->getType() == ChatType::GROUP)
    {
        std::cout << "You are " << (isOwner_ ? "Owner" : "Member") << " of ";
        std::cout << *chat_ << '\n';
    }
    else
    {
        const UserBase* user = nullptr;
        for (size_t i = 0; i < participants.getSize(); i++)
            if (participants[i] != *user_)
                user = &participants[i];
        if (user != nullptr)
            std::cout << "You are chatting with " << user->getName();
        else
            std::cout << "You are alone in chat.";

        std::cout << *chat_ << '\n';
    }
    std::cout << "Participants: ";
    for (size_t i = 0; i < participants.getSize(); i++)
    {
        bool twoArg = false;
        std::cout << '(';
        if (chat_->isOwner(participants[i]) && chat_->getType() == ChatType::GROUP)
        {
            twoArg = true;
            std::cout << "Owner";
        }
        if (participants[i] == *user_)
            std::cout << (twoArg ? ", " : "") << "You";
        std::cout << ") ";
        std::cout << participants[i] << (i != participants.getSize() - 1 ? ", " : "");
    }
    std::cout << '\n';
}

ChatManager::ChatManager(User* user, Chat* chat, DataRepository* data) : BaseManager(user, data)
{
    chat_ = chat;
    isOwner_ = chat_->isOwner(*user);
}

void ChatManager::login()
{
    std::cout << "Entering " << chat_->getName() << " chat...\n";
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
            else if (input == "view_messages")
                view_messages_command();
            else if (input == "view_invites")
                view_invites_command();
            else if (input == "sent_message")
                sent_message_input();
            else if (input == "invite")
                invite_input();
            else if (input == "leave_chat")
            {
                std::cout << "Are you sure? (Y/(N)) ";
                std::cin >> input;
                input = input.toLower();
                if (input == "yes" || input == "y")
                {
                    leave_chat_command();
                    break;
                }
                std::cout << "Cancelling...\n";
            }
            else if (input == "rename_chat")
                rename_chat_input();
            else if (input == "kick")
                kick_input();
            else if (input == "transfer_ownership")
                transfer_ownership_input();
            else if (input == "invites_status")
                invites_status_command();
            else if (input == "toggle_invites")
                toggle_invites_command();
            else if (input == "resolve_invite")
                resolve_invite_input();
            else if (input == "info")
                info_command();
            else
                std::cout << "Invalid command.\n";
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
    std::cout << "Exiting chat...\n";
}
