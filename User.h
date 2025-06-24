#pragma once
#include "UserBase.h"
#include "ISerializable.h"
#include "List.hpp"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

constexpr uint8_t CODE_SIZE = 6;

enum class UserRole
{
    ADMIN,
    MEMBER
};

class User : public UserBase, public ISerializable, public ISerializableDebug
{
    struct ChatHash
    {
        uint8_t hash[HASH_SIZE];

        ChatHash();
        ChatHash(const uint8_t hash[HASH_SIZE]);
    };

    mutable String chats_filename_;

    UserRole role_;
    uint8_t password_hash_[HASH_SIZE];
    List<ChatHash> chats_;

    void generate_chats_filename(bool debug) const;

protected:
    String code_;
    void generate_hash() final;

public:
    User();
    User(const String& username, const String& password, UserRole role);

    UserRole getRole() const;
    const String& getCode() const;

    bool checkPassword(const String& password) const;
    void change_password(const String& old_password, const String& new_password);

    bool chat_present(const ChatHash& chat);

    const uint8_t* operator[](size_t index) const;
    size_t chats_count() const;

    void add_chat(const ChatHash& chat);
    void remove_chat(const ChatHash& chat);

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const User& user);
};
