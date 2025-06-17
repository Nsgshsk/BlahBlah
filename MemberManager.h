#pragma once
#include "BaseManager.h"

class User;
class DataRepository;

class MemberManager : public BaseManager
{
    void select_chat_command(size_t chat_index) const;
    void create_chat_command(const String& input) const;

protected:
    void help_command() override;
    void info_command() override;
    void view_chats_command() const;

    void select_chat_input() const;
    void create_chat_input() const;

public:
    MemberManager(User* user, DataRepository* data);
    void login() override;
};
