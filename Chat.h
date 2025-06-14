#pragma once
#include "Message.h"
#include "SerializableList.hpp"

class UserBase;
typedef uint8_t UserHash[HASH_SIZE];

class Chat final : public Hashable, public ISerializable, public ISerializableDebug
{
    mutable String chat_filename_;
    void generate_chat_filename(bool debug) const;

    List<UserBase> participants_;
    SerializableList<Message> messages_;

    void generate_hash() override;

public:
    Chat();
    explicit Chat(const List<UserBase>& participants);

    bool isParticipantPresent(const UserBase& user) const;
    bool isParticipantPresent(const UserHash& user_hash) const;

    const List<UserBase>& getParticipants() const;
    const List<Message>& getMessages() const;

    void addParticipant(const UserBase& participant);
    void removeParticipant(const UserBase& participant);
    void removeParticipant(const UserHash& participant_hash);

    void sentMessage(const UserBase& sender, const String& message);
    void deleteMessage(const Message& message);

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const Chat& chat);
};
