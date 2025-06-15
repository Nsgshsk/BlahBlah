#include "Admin.h"
#include <fstream>

#include "HashUtility.h"

static constexpr int power_of_10(int exponent)
{
    return !exponent ? 1 : 10 * power_of_10(exponent - 1);
}

static constexpr char CODE_BEGINNING = '#';
static constexpr uint8_t DATE_TIME_MAX_SIZE = 26;
static constexpr int CODE_MODULO = power_of_10(CODE_SIZE);

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

    code_ = CODE_BEGINNING;
    for (int i = 1; i < CODE_SIZE - 1; i++)
        code_ += digitToChar(temp_int);
}

Admin::Admin() = default;

Admin::Admin(const String& username, const String& password)
    : User(username, password, UserRole::ADMIN)
{
    generate_code();
}
