#pragma once
#include "BaseManager.h"

class Chat;

class ChatManager : public BaseManager
{
    bool isOwner_;
    Chat* chat_;

    void sent_message_command(const String& message) const;
    void invite_command(const String& user) const;
    void kick_command(const String& user) const;
    void transfer_ownership_command(const String& user);
    void resolve_invite_command(const String& invite, const String& accepted) const;

protected:
    void help_command() override;
    void info_command() override;
    void view_messages_command() const;
    void leave_chat_command() const;
    void invites_status_command() const;
    void toggle_invites_command() const;
    void view_invites_command() const;

    void sent_message_input() const;
    void invite_input() const;
    void kick_input() const;
    void transfer_ownership_input();
    void resolve_invite_input() const;

public:
    ChatManager(User* user, Chat* chat, DataRepository* data);
    void login() override;
};
