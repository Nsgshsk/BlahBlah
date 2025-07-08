#include "DataRepository.h"

#include "Chat.h"
#include "HashUtility.h"
#include "User.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

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

void DataRepository::loadData() const
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

void DataRepository::loadDataDebug() const
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

void DataRepository::addUser(const User& user) const
{
    users_->add(user);
}

void DataRepository::removeUser(const User& user) const
{
    users_->remove(user.getHash());
}

void DataRepository::addChat(const Chat& chat) const
{
    chats_->add(chat);
}

void DataRepository::removeChat(const Chat& chat) const
{
    chats_->remove(chat.getHash());
}

bool DataRepository::hasUser(const String& name) const
{
    const uint8_t* hash = HashUtility::hash_user(name.c_str());
    bool exists = hasUser(hash);
    delete[] hash;

    return exists;
}

bool DataRepository::hasUser(const uint8_t hash[HASH_SIZE]) const
{
    const User* user = users_->find(hash);
    return user != nullptr;
}

bool DataRepository::hasChat(const uint8_t hash[HASH_SIZE]) const
{
    const Chat* chat = chats_->find(hash);
    return chat != nullptr;
}

const User& DataRepository::getUser(const uint8_t hash[16]) const
{
    const User* user = users_->find(hash);
    if (user == nullptr)
        throw std::invalid_argument("User not found");

    return *user;
}

User& DataRepository::getUser(const uint8_t hash[16])
{
    User* user = users_->find(hash);
    if (user == nullptr)
        throw std::invalid_argument("User not found");

    return *user;
}

const User& DataRepository::getUser(const String& username) const
{
    const uint8_t* hash = HashUtility::hash_user(username.c_str());
    const User& user = getUser(hash);
    delete[] hash;

    return user;
}

User& DataRepository::getUser(const String& username)
{
    const uint8_t* hash = HashUtility::hash_user(username.c_str());
    User& user = getUser(hash);
    delete[] hash;

    return user;
}


const Chat& DataRepository::getChat(const uint8_t hash[16]) const
{
    const Chat* chat = chats_->find(hash);
    if (chat == nullptr)
        throw std::invalid_argument("Chat not found");

    return *chat;
}

Chat& DataRepository::getChat(const uint8_t hash[16])
{
    Chat* chat = chats_->find(hash);
    if (chat == nullptr)
        throw std::invalid_argument("Chat not found");

    return *chat;
}

List<Chat> DataRepository::getAllChats() const
{
    return chats_->getAll();
}
