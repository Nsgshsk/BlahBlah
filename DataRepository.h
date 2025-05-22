#pragma once
#include "HashTable.hpp"

class User;
class Chat;

class DataRepository
{
    HashTable<User> users_;
    HashTable<Chat> chats_;
public:
    void saveData();
    void loadData();
    
    void saveDataDebug();
    void loadDataDebug();

    void addUser(User& user);
    void removeUser(User& user);

    void addChat(Chat& chat);
    void removeChat(Chat& chat);

    const User* getUser(const uint8_t hash[HASH_SIZE]) const;
    User* getUser(const uint8_t hash[HASH_SIZE]);
    
    const Chat* getChat(const uint8_t hash[HASH_SIZE]) const;
    Chat* getChat(const uint8_t hash[HASH_SIZE]);
};
