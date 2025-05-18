#pragma once
#include "SerializableListSpecializations.hpp"

class User : public UserBase
{
    uint8_t password_hash_[HASH_SIZE];
    SerializableList<Chat> chats_;

protected:
    void generate_hash() override;

public:
    User();
    User(const String& username, const String& password);

    bool chat_present(const Chat& chat);

    const Chat& operator[](size_t index) const;
    Chat& operator[](size_t index);

    void create_chat(const User& participant);
    void create_chat(const SerializableList<User>& participants);
    void remove_chat(const Chat& chat);

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const User& user);
};
