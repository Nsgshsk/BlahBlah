#include "User.h"
#include "HashUtility.h"

void User::generate_hash()
{
    char timebuff[DATE_TIME_MAX_SIZE]{'\0'};
    std::time_t now = time(nullptr);
    ctime_s(timebuff, DATE_TIME_MAX_SIZE, &now);

    String represent = timebuff;
    represent += getName();
    represent += HashUtility::hash_to_str(password_hash_);

    const uint8_t* temp = HashUtility::hash_message(represent.c_str());
    HashUtility::copy_hash(hash_, temp);
    delete[] temp;
}

void User::serialize_connection(std::ofstream& ofs)
{
}

void User::deserialize_connection(std::ifstream& ifs)
{
}

void User::serialize_connection()
{
}

void User::deserialize_connection()
{
}

void User::serialize_connection_debug(std::ofstream& ofs)
{
}

void User::deserialize_connection_debug(std::ifstream& ifs)
{
}

void User::serialize_connection_debug()
{
}

void User::deserialize_connection_debug()
{
}

User::User()
{
}

User::User(const String& username, const String& password)
{
}

bool User::chat_present(const Chat& chat)
{
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
