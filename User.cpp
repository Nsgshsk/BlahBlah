#include "User.h"
#include "Chat.h"
#include "HashUtility.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

constexpr char USER_CHATS_FILENAME_PREFIX[] = "user_";
constexpr char USER_CHATS_FILENAME_POSTFIX[] = "_chats";
constexpr char FILE_EXTENSION[] = ".data";
constexpr char DEBUG_FILE_EXTENSION[] = ".debug.txt";

User::ChatHash::ChatHash() = default;

User::ChatHash::ChatHash(const uint8_t hash[16])
{
    HashUtility::copy_hash(this->hash, hash);
}

void User::generate_chats_filename(bool debug = false) const
{
    chats_filename_ = USER_CHATS_FILENAME_PREFIX;
    chats_filename_ += HashUtility::hash_to_str(getHash());
    chats_filename_ += USER_CHATS_FILENAME_POSTFIX;
    chats_filename_ += debug ? DEBUG_FILE_EXTENSION : FILE_EXTENSION;
}

void User::generate_hash()
{
    String represent = getName();
    if (role_ == UserRole::ADMIN)
        represent += code_;

    const uint8_t* temp = HashUtility::hash_user(represent.c_str());
    HashUtility::copy_hash(hash_, temp);
    delete[] temp;
}

User::User() = default;

User::User(const String& username, const String& password, UserRole role) : UserBase(username)
{
    role_ = role;
    const uint8_t* temp = HashUtility::hash_password(password.c_str());
    HashUtility::copy_hash(password_hash_, temp);
    generate_hash();
}

UserRole User::getRole() const
{
    return role_;
}

const String& User::getCode() const
{
    if (role_ != UserRole::ADMIN)
        throw std::runtime_error("Invalid role specific access");

    return code_;
}

bool User::checkPassword(const String& password) const
{
    const uint8_t* temp = HashUtility::hash_password(password.c_str());
    bool result = HashUtility::compare_hash(temp, password_hash_);
    delete[] temp;
    return result;
}

void User::change_password(const String& old_password, const String& new_password)
{
    if (!checkPassword(old_password))
        throw std::invalid_argument("Incorrect password");

    const uint8_t* temp = HashUtility::hash_password(new_password.c_str());
    HashUtility::copy_hash(password_hash_, temp);
    delete[] temp;
}

bool User::chat_present(const ChatHash& chat)
{
    for (size_t i = 0; i < chats_.getSize(); i++)
        if (HashUtility::compare_hash(chats_[i].hash, chat.hash))
            return true;

    return false;
}

const uint8_t* User::operator[](size_t index) const
{
    return chats_[index].hash;
}

size_t User::chats_count() const
{
    return chats_.getSize();
}

void User::add_chat(const ChatHash& chat)
{
    chats_.add(chat);
}

void User::remove_chat(const ChatHash& chat)
{
    for (size_t i = 0; i < chats_.getSize(); i++)
        if (HashUtility::compare_hash(chats_[i].hash, chat.hash))
            chats_.removeAt(i);
}

void User::serialize(std::ofstream& ofs) const
{
    ofs.write((const char*)&role_, sizeof(UserRole));
    if (role_ == UserRole::ADMIN)
        ofs.write(code_.c_str(), CODE_SIZE + 2);

    name_.serialize(ofs);

    ofs.write((const char*)password_hash_, HASH_SIZE);

    if (chats_filename_.isEmpty())
        generate_chats_filename();

    std::ofstream chats_ofs(chats_filename_.c_str(), std::ios::binary | std::ios::out | std::ios::beg);
    if (!chats_ofs.is_open())
        throw std::runtime_error("Could not open user_chats file");

    size_t temp = chats_.getSize();
    chats_ofs.write((const char*)&temp, sizeof(size_t));
    for (size_t i = 0; i < temp; i++)
        chats_ofs.write((const char*)&chats_[i], sizeof(ChatHash));

    chats_ofs.close();
}

void User::deserialize(std::ifstream& ifs)
{
    ifs.read((char*)&role_, sizeof(UserRole));
    char* str;
    if (role_ == UserRole::ADMIN)
    {
        str = new char[CODE_SIZE + 2];
        ifs.read(str, CODE_SIZE + 2);
        code_ = str;
        delete[] str;
    }

    name_.deserialize(ifs);

    ifs.read((char*)password_hash_, HASH_SIZE);

    generate_hash();

    if (chats_filename_.isEmpty())
        generate_chats_filename();

    std::ifstream chats_ifs(chats_filename_.c_str(), std::ios::binary | std::ios::in | std::ios::beg);
    if (!chats_ifs.is_open())
        throw std::runtime_error("Could not open user_chats file");

    size_t temp;
    chats_ifs.read((char*)&temp, sizeof(size_t));
    for (size_t i = 0; i < temp; i++)
    {
        ChatHash chat;
        chats_ifs.read((char*)&chat, sizeof(ChatHash));
        chats_.add(chat);
    }

    chats_ifs.close();
}

void User::serialize_debug(std::ofstream& ofs) const
{
    ofs << (int)role_ << '\n';
    if (role_ == UserRole::ADMIN)
        ofs << code_ << '\n';

    ofs << name_ << '\n';

    HashUtility::serialize_hash_text(ofs, password_hash_);

    if (chats_filename_.isEmpty())
        generate_chats_filename(true);

    std::ofstream chats_ofs(chats_filename_.c_str(), std::ios::out | std::ios::beg);
    if (!chats_ofs.is_open())
        throw std::runtime_error("Could not open user_chats file");

    chats_ofs << chats_.getSize() << '\n';
    for (size_t i = 0; i < chats_.getSize(); i++)
        HashUtility::serialize_hash_text(chats_ofs, chats_[i].hash);

    chats_ofs.close();
}

void User::deserialize_debug(std::ifstream& ifs)
{
    int role = 0;
    ifs >> role;
    if ((UserRole)role == UserRole::ADMIN)
        role_ = UserRole::ADMIN;
    else if ((UserRole)role == UserRole::MEMBER)
        role_ = UserRole::MEMBER;
    else
        throw std::runtime_error("Could not deserialize user");

    if (role_ == UserRole::ADMIN)
        ifs >> code_;

    ifs >> name_;
    HashUtility::deserialize_hash_text(ifs, password_hash_);

    generate_hash();

    if (chats_filename_.isEmpty())
        generate_chats_filename(true);

    std::ifstream chats_ifs(chats_filename_.c_str(), std::ios::in | std::ios::beg);
    if (!chats_ifs.is_open())
        throw std::runtime_error("Could not open user_chats file");

    size_t temp;
    chats_ifs >> temp;
    for (size_t i = 0; i < temp; i++)
    {
        ChatHash chat;
        HashUtility::deserialize_hash_text(chats_ifs, chat.hash);
        chats_.add(chat);
    }

    chats_ifs.close();
}

std::ostream& operator<<(std::ostream& os, const User& user)
{
    os << user.getName() << " | type: ";
    if (user.role_ == UserRole::ADMIN)
        os << "Admin | code (" << user.code_ << ")";
    else if (user.role_ == UserRole::MEMBER)
        os << "Member";
    else
        throw std::runtime_error("Could not deserialize user");
    os << " | chats: " << user.chats_.getSize();

    return os;
}
