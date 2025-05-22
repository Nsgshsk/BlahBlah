#pragma once
#include "User.h"

constexpr uint8_t CODE_SIZE = 8;

class Admin final : public User
{
    char code_[CODE_SIZE];

    void generate_code();
    void generate_hash() override;
public:
    Admin();
    Admin(const String& username, const String& password);

    const char* getCode() const override;

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;
};
