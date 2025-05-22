#include "Admin.h"
#include <fstream>

#include "HashUtility.h"

static constexpr char CODE_BEGINNING = '#';
static constexpr uint8_t DATE_TIME_MAX_SIZE = 26;
static constexpr int CODE_MODULO = 1000000;

namespace 
{
    char digitToChar(int digit)
    {
        return digit % 10 + '0';
    }
}

void Admin::generate_code()
{
    char timebuff[DATE_TIME_MAX_SIZE]{'\0'};
    std::time_t now = time(nullptr);
    ctime_s(timebuff, DATE_TIME_MAX_SIZE, &now);
    
    String represent = getName();
    represent += timebuff;
    const uint8_t* temp = HashUtility::hash_user(represent.c_str());

    int temp_int = temp[0];
    temp_int = (temp_int << 8) + temp[1];
    temp_int = (temp_int << 8) + temp[2];

    temp_int %= CODE_MODULO;

    code_[0] = CODE_BEGINNING;
    for (int i = 1; i < CODE_SIZE - 1; i++)
        code_[i] = digitToChar(temp_int);
    code_[CODE_SIZE - 1] = '\0';
}

void Admin::generate_hash()
{
    String represent = getName();
    represent += code_;

    const uint8_t* temp = HashUtility::hash_message(represent.c_str());
    HashUtility::copy_hash(hash_, temp);
    delete[] temp;
}

Admin::Admin() = default;

Admin::Admin(const String& username, const String& password) : User(username, password)
{
    generate_code();
    generate_hash();
}

const char* Admin::getCode() const
{
    return code_;
}

void Admin::serialize(std::ofstream& ofs) const
{
    ofs.write((const char*)&code_, sizeof(int));
    User::serialize(ofs);
}

void Admin::deserialize(std::ifstream& ifs)
{
    ifs.read((char*)&code_, sizeof(int));
    User::deserialize(ifs);
}

void Admin::serialize_debug(std::ofstream& ofs) const
{
    ofs << code_ << '\n';
    User::serialize_debug(ofs);
}

void Admin::deserialize_debug(std::ifstream& ifs)
{
    ifs >> code_;
    User::deserialize_debug(ifs);
}
