﻿#pragma once
#include "Message.h"
#include "SerializableList.hpp"
#include "UserBase.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

typedef uint8_t UserHash[HASH_SIZE];

enum class ChatType
{
    DIRECT,
    GROUP,
};

class Chat : public Hashable, public ISerializable, public ISerializableDebug
{
    mutable String chat_filename_;
    void generate_chat_filename(bool debug) const;

    String name_;
    ChatType type_;
    UserHash owner_;
    List<UserBase> participants_;
    bool invitation_control_;
    List<UserBase> pending_;
    SerializableList<Message> messages_;

    size_t getInvitationId(const UserBase& invitation) const;
    void generate_hash() override;

public:
    Chat();
    Chat(const List<UserBase>& participants, ChatType type,
         const UserBase& owner, const String& name);

    bool hasParticipant(const UserBase& user) const;
    bool hasParticipant(const UserHash& user_hash) const;

    bool isOwner(const UserBase& user) const;
    const List<UserBase>& getParticipants() const;
    size_t getParticipantsCount() const;
    const List<Message>& getMessages() const;

    void addParticipant(const UserBase& participant);
    void inviteParticipant(const UserBase& participant);
    void removeParticipant(const UserBase& participant);
    void removeParticipant(const UserHash& participant_hash);

    void setOwner(const UserBase& user);
    ChatType getType() const;

    void setName(const String& name);
    const String& getName() const;

    bool invitation_control_status() const;
    void switch_invitation_control();
    const List<UserBase>& get_pending_invitations() const;
    void review_invitation(const UserBase& invitation, bool accepted);

    void sentMessage(const UserBase& sender, const String& message);
    void deleteMessage(const Message& message);

    void serialize(std::ofstream& ofs) const override;
    void deserialize(std::ifstream& ifs) override;

    void serialize_debug(std::ofstream& ofs) const override;
    void deserialize_debug(std::ifstream& ifs) override;

    friend std::ostream& operator<<(std::ostream& os, const Chat& chat);
};
