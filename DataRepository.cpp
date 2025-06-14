#include "DataRepository.h"

#include "Chat.h"
#include "HashUtility.h"
#include "User.h"

static constexpr char USERS_FILENAME[] = "users.data";
static constexpr char CHAT_FILENAME[] = "chats.data";
static constexpr char USERS_DEBUG_FILENAME[] = "users.debug.txt";
static constexpr char CHAT_DEBUG_FILENAME[] = "chats.debug.txt";

DataRepository::DataRepository()
{
    users_ = new HashTable<User>();
    chats_ = new HashTable<Chat>();
}

DataRepository::~DataRepository()
{
    delete users_;
    delete chats_;
}

void DataRepository::saveData() const
{
    std::ofstream usersFile(USERS_FILENAME, std::ios::binary | std::ios::out | std::ios::beg);
    if (!usersFile.is_open())
        throw std::runtime_error("Could not open users.data");

    users_->serialize(usersFile);

    usersFile.close();

    std::ofstream chatsFile(CHAT_FILENAME, std::ios::binary | std::ios::out | std::ios::beg);
    if (!chatsFile.is_open())
        throw std::runtime_error("Could not open chats.data");

    chats_->serialize(chatsFile);

    chatsFile.close();
}

void DataRepository::loadData()
{
    std::ifstream usersFile(USERS_FILENAME, std::ios::binary | std::ios::in | std::ios::beg);
    if (!usersFile.is_open())
        throw std::runtime_error("Could not open users.data");

    users_->deserialize(usersFile);

    usersFile.close();

    std::ifstream chatsFile(CHAT_FILENAME, std::ios::binary | std::ios::in | std::ios::beg);
    if (!chatsFile.is_open())
        throw std::runtime_error("Could not open chats.data");

    chats_->deserialize(chatsFile);

    chatsFile.close();
}

void DataRepository::saveDataDebug() const
{
    std::ofstream usersFile(USERS_DEBUG_FILENAME, std::ios::out | std::ios::beg);
    if (!usersFile.is_open())
        throw std::runtime_error("Could not open users.debug.txt");

    users_->serialize_debug(usersFile);

    usersFile.close();

    std::ofstream chatsFile(CHAT_DEBUG_FILENAME, std::ios::out | std::ios::beg);
    if (!chatsFile.is_open())
        throw std::runtime_error("Could not open chats.debug.txt");

    chats_->serialize_debug(chatsFile);

    chatsFile.close();
}

void DataRepository::loadDataDebug()
{
    std::ifstream usersFile(USERS_DEBUG_FILENAME, std::ios::in | std::ios::beg);
    if (!usersFile.is_open())
        throw std::runtime_error("Could not open users.debug.txt");

    users_->deserialize_debug(usersFile);

    usersFile.close();

    std::ifstream chatsFile(CHAT_DEBUG_FILENAME, std::ios::in | std::ios::beg);
    if (!chatsFile.is_open())
        throw std::runtime_error("Could not open chats.debug.txt");

    chats_->deserialize_debug(chatsFile);

    chatsFile.close();
}

void DataRepository::addUser(const User& user)
{
    users_->add(user);
}

void DataRepository::removeUser(const User& user)
{
    users_->remove(user.getHash());
}

void DataRepository::addChat(const Chat& chat)
{
    chats_->add(chat);
}

void DataRepository::removeChat(const Chat& chat)
{
    chats_->remove(chat.getHash());
}

const User* DataRepository::getUser(const uint8_t hash[16]) const
{
    return users_->find(hash);
}

User* DataRepository::getUser(const uint8_t hash[16])
{
    return users_->find(hash);
}

const User* DataRepository::getUser(const String& username) const
{
    const uint8_t* hash = HashUtility::hash_user(username.c_str());
    const User* user = getUser(hash);
    delete hash;

    return user;
}

User* DataRepository::getUser(const String& username)
{
    const uint8_t* hash = HashUtility::hash_user(username.c_str());
    User* user = getUser(hash);
    delete hash;

    return user;
}


const Chat* DataRepository::getChat(const uint8_t hash[16]) const
{
    return chats_->find(hash);
}

Chat* DataRepository::getChat(const uint8_t hash[16])
{
    return chats_->find(hash);
}

const Chat* DataRepository::getChat(const String& username) const
{
    const uint8_t* hash = HashUtility::hash_user(username.c_str());
    const Chat* chat = getChat(hash);
    delete hash;

    return chat;
}

Chat* DataRepository::getChat(const String& username)
{
    const uint8_t* hash = HashUtility::hash_user(username.c_str());
    Chat* chat = getChat(hash);
    delete hash;

    return chat;
}
