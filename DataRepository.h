#pragma once
#include "HashTable.hpp"

class User;
class Chat;
class String;

class DataRepository
{
    HashTable<User>* users_;
    HashTable<Chat>* chats_;

public:
    DataRepository();
    DataRepository(const DataRepository& other) = delete;
    DataRepository& operator=(const DataRepository& other) = delete;
    ~DataRepository();

    DataRepository(DataRepository&& other) = delete;
    DataRepository& operator=(DataRepository&& other) = delete;

    void saveData() const;
    void loadData();

    void saveDataDebug() const;
    void loadDataDebug();

    void addUser(const User& user);
    void removeUser(const User& user);

    void addChat(const Chat& chat);
    void removeChat(const Chat& chat);

    bool hasUser(const String& name) const;
    bool hasUser(const uint8_t hash[HASH_SIZE]) const;
    bool hasChat(const uint8_t hash[HASH_SIZE]) const;

    const User& getUser(const uint8_t hash[HASH_SIZE]) const;
    User& getUser(const uint8_t hash[HASH_SIZE]);

    const User& getUser(const String& username) const;
    User& getUser(const String& username);

    const Chat& getChat(const uint8_t hash[HASH_SIZE]) const;
    Chat& getChat(const uint8_t hash[HASH_SIZE]);

    const Chat& getChat(const String& username) const;
    Chat& getChat(const String& username);
};
