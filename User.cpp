#include "User.h"
#include "Chat.h"
#include "HashUtility.h"

constexpr char USER_CHATS_FILENAME_PREFIX[] = "user_";
constexpr char USER_CHATS_FILENAME_POSTFIX[] = "_chats";
constexpr char FILE_EXTENSION[] = ".bin";
constexpr char DEBUG_FILE_EXTENSION[] = ".debug.txt";

void User::generate_chats_filename(bool debug = false) const
{
    chats_filename_ = USER_CHATS_FILENAME_PREFIX;
    chats_filename_ += HashUtility::hash_to_str(getHash());
    chats_filename_ += USER_CHATS_FILENAME_POSTFIX;
    chats_filename_ += debug ? DEBUG_FILE_EXTENSION : FILE_EXTENSION;
}

void User::generate_hash()
{
    const String represent = getName();

    const uint8_t* temp = HashUtility::hash_user(represent.c_str());
    HashUtility::copy_hash(hash_, temp);
    delete[] temp;
}

User::User() = default;

User::User(const String& username, const String& password) : UserBase(username)
{
    const uint8_t* temp = HashUtility::hash_password(password.c_str());
    HashUtility::copy_hash(password_hash_, temp);
    User::generate_hash();
}

const char* User::getCode() const
{
    return nullptr;
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
    size_t temp = name_.length();
    ofs.write((const char*)&temp, sizeof(size_t));
    ofs.write(name_.c_str(), temp + 1);

    ofs.write((const char*)password_hash_, HASH_SIZE);

    if (chats_filename_.isEmpty())
        generate_chats_filename();

    std::ofstream chats_ofs(chats_filename_.c_str(), std::ios::binary);
    if (!chats_ofs.is_open())
        throw std::runtime_error("Could not open user_chats file");

    temp = chats_.getSize();
    chats_ofs.write((const char*)&temp, sizeof(size_t));
    for (size_t i = 0; i < temp; i++)
        chats_ofs.write((const char*)&chats_[i], sizeof(ChatHash));

    chats_ofs.close();
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

    if (chats_filename_.isEmpty())
        generate_chats_filename();

    std::ifstream chats_ifs(chats_filename_.c_str(), std::ios::binary);
    if (!chats_ifs.is_open())
        throw std::runtime_error("Could not open user_chats file");

    chats_ifs.read((char*)&temp, sizeof(size_t));
    for (size_t i = 0; i < temp; i++)
    {
        ChatHash chat;
        chats_ifs.read((char*)&chat, sizeof(ChatHash));
        chats_.add(chat);
    }

    chats_ifs.close();

    generate_hash();
}

void User::serialize_debug(std::ofstream& ofs) const
{
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
    ifs >> name_;
    HashUtility::deserialize_hash_text(ifs, password_hash_);

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

    generate_hash();
}

std::ostream& operator<<(std::ostream& os, const User& user)
{
    os << user.getName() << '\n';
    return os;
}
