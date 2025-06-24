#pragma once
#include <iosfwd>

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

namespace HashUtility
{
    constexpr unsigned char HASH_SIZE = 16;

    const unsigned char* hash_password(const char* password);
    const unsigned char* hash_user(const char* user);
    const unsigned char* hash_chat(const char* chat);
    const unsigned char* hash_message(const char* message);
    void copy_hash(unsigned char dest[HASH_SIZE], const unsigned char src[HASH_SIZE]);
    const char* hash_to_str(const unsigned char hash[HASH_SIZE]);
    bool compare_hash(const unsigned char left[HASH_SIZE], const unsigned char right[HASH_SIZE]);

    void serialize_hash_text(std::ofstream& ofs, const unsigned char hash[HASH_SIZE]);
    void deserialize_hash_text(std::ifstream& ifs, unsigned char hash[HASH_SIZE]);
}
