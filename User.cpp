#include "User.h"
#include "HashUtility.h"

void User::generate_hash()
{
    String represent = getName();

    const uint8_t* temp = HashUtility::hash_message(represent.c_str());
    HashUtility::copy_hash(hash_, temp);
    delete[] temp;
}

User::User() = default;

User::User(const String& username, const String& password) : UserBase(username)
{
    const uint8_t* temp = HashUtility::hash_password(password.c_str());
    HashUtility::copy_hash(hash_, temp);
    User::generate_hash();
}

bool User::chat_present(const Chat& chat)
{
    for (size_t i = 0; i < chats_.getSize(); i++)
        if (chats_[i] == chat)
            return true;

    return false;
}

const Chat& User::operator[](size_t index) const
{
    return chats_[index];
}

Chat& User::operator[](size_t index)
{
    return chats_[index];
}

void User::create_chat(const User& participant)
{
}

void User::create_chat(const SerializableList<User>& participants)
{
}

void User::remove_chat(const Chat& chat)
{
}

void User::serialize(std::ofstream& ofs) const
{
    UserBase::serialize(ofs);
}

void User::deserialize(std::ifstream& ifs)
{
    UserBase::deserialize(ifs);
}

void User::serialize_debug(std::ofstream& ofs) const
{
    UserBase::serialize_debug(ofs);
}

void User::deserialize_debug(std::ifstream& ifs)
{
    UserBase::deserialize_debug(ifs);
}

std::ostream& operator<<(std::ostream& os, const User& user)
{
    return os;
}
