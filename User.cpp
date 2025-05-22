#include "User.h"
#include "Chat.h"
#include "HashUtility.h"

void User::generate_hash()
{
    const String represent = getName();

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

const char* User::getCode() const
{
    return nullptr;
}

bool User::chat_present(const ChatHash& chat)
{
    for (size_t i = 0; i < chats_.getSize(); i++)
        if (HashUtility::compare_hash(chats_[i], chat))
            return true;

    return false;
}

const ChatHash& User::operator[](size_t index) const
{
    return chats_[index];
}

void User::add_chat(const ChatHash& chat)
{
    chats_.add(chat);
}

void User::remove_chat(const ChatHash& chat)
{
    for (size_t i = 0; i < chats_.getSize(); i++)
        if (HashUtility::compare_hash(chats_[i], chat))
            chats_.removeAt(i);
}

void User::serialize(std::ofstream& ofs) const
{
    size_t temp = name_.length();
    ofs.write((const char*)&temp, sizeof(size_t));
    ofs.write(name_.c_str(), temp + 1);

    ofs.write((const char*)password_hash_, HASH_SIZE);

    for (size_t i = 0; i < chats_.getSize(); i++)
        ofs.write((const char*)&chats_[i], HASH_SIZE);
}

void User::deserialize(std::ifstream& ifs)
{
    size_t temp;
    ifs.read((char*)&temp, sizeof(size_t));
    char* str = new char[temp + 1];
    ifs.read(str, temp + 1);
    name_ = str;
    delete[] str;

    ifs.read((char*)password_hash_, HASH_SIZE);

    for (size_t i = 0; i < chats_.getSize(); i++)
    {
        ChatHash chat;
        ifs.read((char*)&chat, HASH_SIZE);
        chats_.add(chat);
    }

    generate_hash();
}

void User::serialize_debug(std::ofstream& ofs) const
{
    ofs << name_.length() << '\n';
    ofs << name_ << '\n';

    HashUtility::serialize_hash_text(ofs, password_hash_);

    for (size_t i = 0; i < chats_.getSize(); i++)
        HashUtility::serialize_hash_text(ofs, chats_[i]);
}

void User::deserialize_debug(std::ifstream& ifs)
{
    ifs >> name_;
    HashUtility::deserialize_hash_text(ifs, password_hash_);

    for (size_t i = 0; i < chats_.getSize(); i++)
    {
        ChatHash chat;
        HashUtility::deserialize_hash_text(ifs, chat);
        chats_.add(chat);
    }

    generate_hash();
}

std::ostream& operator<<(std::ostream& os, const User& user)
{
    throw std::exception("Not implemented");
    return os;
}
