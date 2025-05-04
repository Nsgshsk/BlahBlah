#include "MD5.h"

#include <cstdint>
#include <cstring>
#include <stdexcept>

// MD5 implementation based on pseudocode from https://en.wikipedia.org/wiki/MD5  
// Created on 03/05/2025 for a university project requiring a custom hash solution.  
//  
// Notes:  
// - Endianness: Message length is appended in little-endian format.  
// - Portability: Uses custom rotate-left to avoid compiler-specific intrinsics.  
//  
// This is a learning exercise; not intended for production use.  

// Constants used for the MD5 algorithm implementation 
namespace hash_constants
{
    constexpr uint8_t UINT32_BITS = 32;

    constexpr uint8_t CHAR_SIZE = 8;
    constexpr uint8_t DIGEST_PART_COUNT = 16;
    constexpr uint8_t DIGEST_SIZE = 64;
    constexpr uint8_t DIGEST_REMAINDER_SIZE = 56;
    constexpr uint16_t DIGEST_LENGTH = 512;

    // s specifies the per-round shift amounts
    constexpr uint32_t s[DIGEST_SIZE] =
    {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

    // K stores pre-calculated binary integer parts of the sines of integers (Radians)
    constexpr uint32_t K[DIGEST_SIZE] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };
}

// Helper functions
namespace
{
    // Left rotates the bits (example: 111000 << 2 -> 100011 )
    uint32_t rotate_left(uint32_t x, uint32_t n)
    {
        n %= hash_constants::UINT32_BITS;
        return x << n | x >> (hash_constants::UINT32_BITS - n);
    }

    // Frees memory used for data preparation
    void free_memory(uint8_t* data)
    {
        delete[] data;
    }

    // The function converts a string to MD5 compatible format into "output"
    uint64_t prepare_str_md5(const char* input, uint8_t*& output)
    {
        // Allocating memory for needed lengths/sizes
        // [0] input length; [1] empty bytes count; [2] output final size in bytes
        uint64_t length[] = {strlen(input), 0, 0};

        // Getting the count of empty bytes until the final output size in bytes is 56 (mod 64)
        while ((length[0] + 1 + length[1]) % hash_constants::DIGEST_SIZE != hash_constants::DIGEST_REMAINDER_SIZE)
            length[1]++;

        // Storing final output size in bytes
        length[2] = length[0] + 1 + length[1] + hash_constants::DIGEST_SIZE / hash_constants::CHAR_SIZE;
        output = new uint8_t[length[2]]; // Allocating memory for output

        // Length of input in bits
        uint64_t length_bits = length[0] * hash_constants::CHAR_SIZE;

        // Replacing with more robust solution because this is system dependant (little-endian systems only)
        // const char* temp = (const char*)&(length_bits);

        // Output creating process
        for (uint64_t i = 0; i < length[2]; i++)
        {
            if (i < length[0])
                output[i] = input[i]; // Copies the input string to the beginning of output
            else if (i == length[0])
                output[i] = 0x80; // Appends a byte with only one leading 1 to the input
            else if (i - length[0] <= length[1])
                output[i] = 0x00; // Appends empty bytes until the output length in bytes is 56 (mod 64)
            else
            {
                // Appends the input length in bits to the end of the output so that we have 0 (mod 64) bytes

                // Old version which is system dependant
                // output[i] = temp[i - length[2] + hash_constants::CHAR_SIZE_LENGTH];

                // New version which specifically converts the uint64_t to little-endian byte order
                output[i] = (uint8_t)length_bits;
                length_bits >>= 8; // Left shifts length_bits with 1 byte so that we read the next byte in needed order
            }
        }

        return length[2]; // Returns the byte size of the prepared MD5 string
    }

    // Separates 512-bit segment of data into 16 32-bit parts
    void prepare_parts(const uint8_t* data, const uint64_t current,
                       uint32_t digest[hash_constants::DIGEST_PART_COUNT])
    {
        uint64_t i = 0 + current * hash_constants::DIGEST_SIZE; // Setting the starting index
        for (uint8_t j = 0; j < hash_constants::DIGEST_PART_COUNT; j++) // Filling all 16 32-bit segments into digest
        {
            // Filling current 32-bit segment with bytes (little-endian order)
            digest[j] = 0x0;
            for (uint8_t k = 0; k < 4; k++)
            {
                digest[j] |= data[i] << (k * 8);
                i++;
            }
        }
    }

    // Main logic of MD5 hashing algorithm
    void main_loop(uint32_t digest[16], uint32_t& A, uint32_t& B, uint32_t& C, uint32_t& D)
    {
        for (uint8_t i = 0; i < hash_constants::DIGEST_SIZE; i++)
        {
            uint32_t F, g;

            if (i < 16)
            {
                F = (B & C) | ((~B) & D);
                g = i;
            }
            else if (i < 32)
            {
                F = (D & B) | ((~D) & C);
                g = (5 * i + 1) % hash_constants::DIGEST_PART_COUNT;
            }
            else if (i < 48)
            {
                F = B ^ C ^ D;
                g = (3 * i + 5) % hash_constants::DIGEST_PART_COUNT;
            }
            else
            {
                F = C ^ (B | (~D));
                g = 7 * i % hash_constants::DIGEST_PART_COUNT;
            }

            F += A + hash_constants::K[i] + digest[g];
            A = D;
            D = C;
            C = B;
            B += rotate_left(F, hash_constants::s[i]);
        }
    }

    // Fills a byte from var to output at specified index and removes that byte from var
    void fill_uint32_into_uint8(uint8_t* output, uint32_t& var, uint32_t index)
    {
        output[index] = (uint8_t)var;
        var >>= hash_constants::CHAR_SIZE;
    }

    // Appends the values into the final MD5 hash
    void convert_vars_into_md5(uint32_t a0, uint32_t b0, uint32_t c0, uint32_t d0, uint8_t* output)
    {
        for (uint8_t i = 0; i < hash_constants::DIGEST_PART_COUNT; i++)
        {
            if (i < 4)
                fill_uint32_into_uint8(output, a0, i); // Appending a0
            else if (i < 8)
                fill_uint32_into_uint8(output, b0, i); // Appending b0
            else if (i < 12)
                fill_uint32_into_uint8(output, c0, i); // Appending c0
            else
                fill_uint32_into_uint8(output, d0, i); // Appending d0
        }
    }

    // MD5 hash function
    const uint8_t* calculate_md5(const uint8_t* data, const uint64_t length)
    {
        // Buffer of MD5 algorithm
        uint32_t a0 = 0x67452301; // A
        uint32_t b0 = 0xefcdab89; // B
        uint32_t c0 = 0x98badcfe; // C
        uint32_t d0 = 0x10325476; // D

        // Number of 512-bit segments in data
        const uint64_t parts = length * hash_constants::CHAR_SIZE / hash_constants::DIGEST_LENGTH;

        // Loops through segment to prepare the buffer
        uint64_t current = 0;
        while (current < parts)
        {
            uint32_t digest[hash_constants::DIGEST_PART_COUNT]{0};
            prepare_parts(data, current, digest);

            uint32_t A = a0;
            uint32_t B = b0;
            uint32_t C = c0;
            uint32_t D = d0;

            main_loop(digest, A, B, C, D);

            a0 += A;
            b0 += B;
            c0 += C;
            d0 += D;

            current++;
        }

        // Converts the buffer into the resulting MD5 hash
        uint8_t* result = new uint8_t[hash_constants::DIGEST_PART_COUNT];
        convert_vars_into_md5(a0, b0, c0, d0, result);

        return result;
    }
}

//         Calculates the MD5 hash of a string.
//
// !!!       Result is dynamically allocated       !!!
// !!! You should clear the hash when done with it !!!
const uint8_t* calculate_md5(const char* string)
{
    if (string == nullptr)
        return nullptr;

    uint8_t* data = nullptr;
    const uint64_t length = prepare_str_md5(string, data);
    const uint8_t* result = calculate_md5(data, length);

    free_memory(data); // Freeing the memory allocated for data
    return result; // Returns pointer to MD5 hash that is 16 bytes (128 bits) size
}
