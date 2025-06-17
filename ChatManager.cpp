#include "ChatManager.h"
#include <iostream>
#include "Chat.h"
#include "DataRepository.h"
#include "User.h"

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
    }
    catch (...)
    {
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
        if (isOwner_)
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
        std::cout << "Left " << chat_->getName() << "chat successfully!\n";
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void ChatManager::invites_status_command() const
{
    std::cout << "Invites are "
        << (chat_->invitation_control_status() ? "" : "not")
        << "controlled by Owner.\n";
}

void ChatManager::toggle_invites_command() const
{
    try
    {
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
    std::cout << "Viewing invites...\n";
    const List<UserBase> invites = chat_->get_pending_invitations();
    if (invites.getSize() == 0)
        std::cout << "No pending invites.\n";

    for (size_t i = 0; i < invites.getSize(); i++)
        std::cout << "*) " << invites[i] << " - pending\n";

    std::cout << '\n';
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
    std::cout << "\t*) sent_message\n";
    std::cout << "\t*) invite\n";
    std::cout << "\t*) leave_chat\n";
    std::cout << "\t*) invites_status\n";
    if (isOwner_)
    {
        std::cout << "\t*) kick\n";
        std::cout << "\t*) transfer_ownership\n";
        std::cout << "\t*) toggle_invites\n";
        if (chat_->invitation_control_status())
        {
            std::cout << "\t*) view_invites\n";
            std::cout << "\t*) resolve_invite\n";
        }
    }
    std::cout << "\t*) info\n";
    std::cout << "\t*) logout\n";
}

void ChatManager::info_command()
{
    std::cout << "You are " << (isOwner_ ? "Owner" : "Member") << " of ";
    std::cout << *chat_ << " chat.\n";
    std::cout << "Participants: ";
    const List<UserBase>& participants = chat_->getParticipants();
    for (size_t i = 0; i < participants.getSize(); i++)
    {
        if (chat_->isOwner(participants[i]))
            std::cout << "(Owner) ";
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
}
