#pragma once
#include "Message.h"
#include "SerializableList.hpp"

class UserBase;

class Chat final : public Hashable, public ISerializable, public ISerializableDebug
{
    List<UserBase> participants_;
    SerializableList<Message> messages_;

    void generate_hash() override;

public:
    Chat();
    explicit Chat(const List<UserBase>& participants);

    bool isParticipantPresent(const UserBase& user) const;
    bool isParticipantPresent(const uint8_t user_hash[HASH_SIZE]) const;
    
    const uint8_t* getParticipantHash(size_t index) const;
    const Message& getMessage(size_t index) const;

    void addParticipant(const UserBase& participant);
    void removeParticipant(const UserBase& participant);

    void sentMessage(const UserBase& sender, const String& message);
    void deleteMessage(const Message& message);

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const Chat& chat);
};
