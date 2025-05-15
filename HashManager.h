#pragma once

constexpr unsigned char HASH_SIZE = 16;

class HashManager
{
public:
    static const unsigned char* hash_password(const char* password);
    static const unsigned char* hash_user(const char* user);
    static const unsigned char* hash_chat(const char* chat);
    // const unsigned char* hash_message(const char* message);
    static void copyHash(unsigned char dest[HASH_SIZE], const unsigned char src[HASH_SIZE]);
};
