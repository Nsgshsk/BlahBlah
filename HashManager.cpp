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

void HashManager::copyHash(unsigned char dest[16], const unsigned char src[16])
{
    for (int i = 0; i < HASH_SIZE; i++)
        dest[i] = src[i];
}
