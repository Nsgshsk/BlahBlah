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
    SerializableList<User> participants;
    participants.add(*this);
    participants.add(participant);
    chats_.add(Chat(participants));
}

void User::create_chat(const SerializableList<User>& participants)
{
    SerializableList<User> temp;
    temp.add(*this);
    temp.append(participants);
    chats_.add(Chat(participants));
}

void User::remove_chat(const Chat& chat)
{
    for (size_t i = 0; i < chats_.getSize(); i++)
        if (chats_[i] == chat)
            chats_.removeAt(i);
}

void User::serialize(std::ofstream& ofs) const
{
    size_t temp = name_.length();
    ofs.write((const char*)&temp, sizeof(size_t));
    ofs.write(name_.c_str(), temp + 1);

    ofs.write((const char*)hash_, HASH_SIZE);
    chats_.serialize(ofs);
}

void User::deserialize(std::ifstream& ifs)
{
    size_t temp;
    ifs.read((char*)&temp, sizeof(size_t));
    char* str = new char[temp + 1];
    ifs.read(str, temp + 1);
    name_ = str;
    delete[] str;

    ifs.read((char*)hash_, HASH_SIZE);
}

void User::serialize_debug(std::ofstream& ofs) const
{
    ofs << name_.length() << '\n';
    ofs << name_ << '\n';

    HashUtility::serialize_hash_text(ofs, password_hash_);
    chats_.serialize_debug(ofs);
}

void User::deserialize_debug(std::ifstream& ifs)
{
    ifs >> name_;
    HashUtility::deserialize_hash_text(ifs, password_hash_);
    chats_.deserialize_debug(ifs);
}

std::ostream& operator<<(std::ostream& os, const User& user)
{
    return os;
}
