#pragma once
#include "UserBase.h"
#include "ISerializable.h"
#include "List.hpp"
#include "SharedPtr.hpp"

class Chat;

typedef SharedPtr<Chat> ChatPtr;

class User : public UserBase, public ISerializable, public ISerializableDebug
{
    uint8_t password_hash_[HASH_SIZE];
    List<ChatPtr> chats_;

protected:
    void generate_hash() override;

public:
    User();
    User(const String& username, const String& password);

    bool chat_present(const ChatPtr& chat);
    
    const Chat& operator[](size_t index) const;
    Chat& operator[](size_t index);

    void add_chat(const ChatPtr& chat);
    void remove_chat(const ChatPtr& chat);

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const User& user);
};
