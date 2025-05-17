#pragma once
#include "Chat.h"
#include "SerializableList.hpp"
#include "UserBase.h"

class User : public UserBase
{
    uint8_t password_hash_[HASH_SIZE];
    SerializableList<Chat> chats_;

public:
    User();
    User(const String& username, const String& password);

    void create_chat(const String& username, const String& password);

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const User& user);
};
