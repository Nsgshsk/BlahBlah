#pragma once
#include "Message.h"
#include "SharedPtr.hpp"
#include "SerializableList.hpp"

class User;

typedef SharedPtr<User> UserPtr;

class Chat final : public Hashable, public ISerializable, public ISerializableDebug
{
    List<UserPtr> participants_;
    SerializableList<Message> messages_;

    void generate_hash() override;

public:
    Chat();
    explicit Chat(const List<UserPtr>& participants);

    bool isParticipantPresent(const UserPtr& user) const;
    
    const User& getParticipant(size_t index) const;
    const Message& getMessage(size_t index) const;

    void addParticipant(const UserPtr& participant);
    void removeParticipant(const UserPtr& participant);

    void sentMessage(const UserPtr& sender, const String& message);
    void deleteMessage(const Message& message);

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const Chat& chat);
};
