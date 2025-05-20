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

bool User::chat_present(const ChatPtr& chat)
{
    for (size_t i = 0; i < chats_.getSize(); i++)
        if (*chats_[i] == *chat)
            return true;

    return false;
}

const Chat& User::operator[](size_t index) const
{
    return *chats_[index];
}

Chat& User::operator[](size_t index)
{
    return *chats_[index];
}

void User::add_chat(const ChatPtr& chat)
{
    chats_.add(chat);
}

void User::remove_chat(const ChatPtr& chat)
{
    for (size_t i = 0; i < chats_.getSize(); i++)
        if (*chats_[i] == *chat)
            chats_.removeAt(i);
}

void User::serialize(std::ofstream& ofs) const
{
    size_t temp = name_.length();
    ofs.write((const char*)&temp, sizeof(size_t));
    ofs.write(name_.c_str(), temp + 1);

    ofs.write((const char*)password_hash_, HASH_SIZE);

    // Function for serializing chat hashes for user
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
}

void User::serialize_debug(std::ofstream& ofs) const
{
    ofs << name_.length() << '\n';
    ofs << name_ << '\n';

    HashUtility::serialize_hash_text(ofs, password_hash_);

    // Function to serialize chat hashes
}

void User::deserialize_debug(std::ifstream& ifs)
{
    ifs >> name_;
    HashUtility::deserialize_hash_text(ifs, password_hash_);
}

std::ostream& operator<<(std::ostream& os, const User& user)
{
    return os;
}
