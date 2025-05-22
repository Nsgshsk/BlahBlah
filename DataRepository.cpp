#include "DataRepository.h"

#include "Chat.h"
#include "User.h"

static constexpr char USERS_FILENAME[] = "users.data";
static constexpr char CHAT_FILENAME[] = "chats.data";
static constexpr char USERS_DEBUG_FILENAME[] = "users.debug.txt";
static constexpr char CHAT_DEBUG_FILENAME[] = "chats.debug.txt";

void DataRepository::saveData()
{
    std::ofstream usersFile(USERS_FILENAME, std::ios::binary);
    if (!usersFile.is_open())
        throw std::runtime_error("Could not open users.data");
    
    users_.serialize(usersFile);

    usersFile.close();

    throw std::exception("Not implemented");
}

void DataRepository::loadData()
{
    std::ifstream userFile(USERS_FILENAME, std::ios::binary);
    if (!userFile.is_open())
        throw std::runtime_error("Could not open users.data");

    throw std::exception("Not implemented");
}

void DataRepository::saveDataDebug()
{
    throw std::exception("Not implemented");
}

void DataRepository::loadDataDebug()
{
    throw std::exception("Not implemented");
}

void DataRepository::addUser(User& user)
{
    users_.add(user);
}

void DataRepository::removeUser(User& user)
{
    users_.remove(user.getHash());
}

void DataRepository::addChat(Chat& chat)
{
    chats_.add(chat);
}

void DataRepository::removeChat(Chat& chat)
{
    chats_.remove(chat.getHash());
}

User* DataRepository::getUser(const uint8_t hash[16]) const
{
    return users_.find(hash);
}

Chat* DataRepository::getChat(const uint8_t hash[16]) const
{
    return chats_.find(hash);
}
