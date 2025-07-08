#pragma once
#include "MemberManager.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

class AdminManager : public MemberManager
{
    void delete_user_command(const String& user) const;
    void delete_chat_command(size_t index) const;

    void view_all_chats_command() const;
    void delete_user_input() const;
    void delete_chat_input() const;

protected:
    void help_command() override;

public:
    AdminManager(User* user, DataRepository* data);
    void login() override;
};
