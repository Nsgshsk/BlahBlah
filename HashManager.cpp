#include "HashManager.h"

#include <cstdint>
#include <cstring>

#include "MD5.h"

// Helper functions and constants
namespace
{
    // Constants used by manager
    constexpr size_t SALT_LEN = 32;
    const char* PASSWORD_SALT = "SrHzaqyFkprv5fwsQsytVbjt9saQRFgh";
    const char* USER_SALT = "WEfgyK0vdeqyIHdeRfaWerFgiop2hfEr";
    const char* CHAT_SALT = "AQwscg7rewYuiopGFES4schyrgjRFds";

    // Function salts the provided string with the provided salt (aka appending them)
    const char* saltString(const char* str, const char* salt)
    {
        size_t size = strlen(str) + SALT_LEN + 1; // Length of chat string

        // Allocating memory for temporary prep string
        char* prep = new char[size];

        // Copies chat string to prep
        strcpy_s(prep, size, str);
        // Concatenates chat salt to the prep string
        strcat_s(prep, size, salt);

        return prep;
    }

    char hex_to_char(unsigned char c)
    {
        if (c <= 9)
            return (char)(c + '0');
        if (10 <= c && c <= 15)
            return (char)(c - 10 + 'a');
        
        return -1;
    }
}

// Function takes the password string that is then appended with salt.
// Function returns MD5 hash of the salted string
//
// !!!     Hash string is dynamically allocated    !!!
// !!! You should deallocate it when done using it !!!
const unsigned char* HashManager::hash_password(const char* password)
{
    // Getting salted string
    const char* prep = saltString(password, PASSWORD_SALT);

    // Getting the md5 hash from salted string
    const uint8_t* hash = calculate_md5(prep);
    delete[] prep; // Freeing resources taken for salted string

    return hash; // Returning md5 hash
}

// Function takes the user representation that is then appended with salt.
// Function returns MD5 hash of the salted string
//
// !!!     Hash string is dynamically allocated    !!!
// !!! You should deallocate it when done using it !!!
const unsigned char* HashManager::hash_user(const char* user)
{
    // Getting salted string
    const char* prep = saltString(user, USER_SALT);

    // Getting the md5 hash from salted string
    const uint8_t* hash = calculate_md5(prep);
    delete[] prep; // Freeing resources taken for salted string

    return hash; // Returning md5 hash
}

// Function takes the chat representation that is then appended with salt.
// Function returns MD5 hash of the salted string
//
// !!!     Hash string is dynamically allocated    !!!
// !!! You should deallocate it when done using it !!!
const unsigned char* HashManager::hash_chat(const char* chat)
{
    // Getting salted string
    const char* prep = saltString(chat, CHAT_SALT);

    // Getting the md5 hash from salted string
    const uint8_t* hash = calculate_md5(prep);
    delete[] prep; // Freeing resources taken for salted string

    return hash; // Returning md5 hash
}

// Copies hash from source to destination
void HashManager::copy_hash(unsigned char dest[HASH_SIZE], const unsigned char src[HASH_SIZE])
{
    for (int i = 0; i < HASH_SIZE; i++)
        dest[i] = src[i];
}

const char* HashManager::hash_to_str(const unsigned char hash[HASH_SIZE])
{
    char* prep = new char[HASH_SIZE * 2 + 1];
    for (uint8_t i = 0; i < HASH_SIZE; i++)
    {
        uint8_t curr = i * 2;
        char ch = hex_to_char(hash[i] / 16);
        prep[curr] = ch;
        ch = hex_to_char(hash[i] % 16);
        prep[curr + 1] = ch;
    }
    prep[(uint8_t)(HASH_SIZE * 2)] = '\0';
    return prep;
}
