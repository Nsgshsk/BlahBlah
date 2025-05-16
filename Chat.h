#pragma once
#include "Message.h"
#include "SerializableList.hpp"
#include "UserBase.h"

class Chat final : public ISerializable, public ISerializableDebug
{
    uint8_t hash_[HASH_LENGTH];
    SerializableList<UserBase> participants_;
    SerializableList<Message> messages_;

    void generateHash();

public:
    Chat();
    Chat(const SerializableList<UserBase>& participants);

    const uint8_t* getHash() const;

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const Chat& chat);
};
