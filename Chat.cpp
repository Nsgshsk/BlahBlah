#include <ctime>

#include "Chat.h"
#include "UserBase.h"
#include "HashUtility.h"

constexpr char CHAT_FILENAME_PREFIX[] = "chat_";
constexpr char FILE_EXTENSION[] = ".bin";
constexpr char DEBUG_FILE_EXTENSION[] = ".debug.txt";

void Chat::generate_chat_filename(bool debug = false) const
{
    chat_filename_ = CHAT_FILENAME_PREFIX;
    chat_filename_ += HashUtility::hash_to_str(getHash());
    chat_filename_ += debug ? DEBUG_FILE_EXTENSION : FILE_EXTENSION;
}

size_t Chat::getInvitationId(const UserBase& invitation) const
{
    for (size_t i = 0; i < pending_.getSize(); i++)
        if (pending_[i] == invitation)
            return i;

    throw std::invalid_argument("Invitation does not exist");
}

void Chat::generate_hash()
{
    char timebuff[DATE_TIME_MAX_SIZE]{'\0'};
    std::time_t now = time(nullptr);
    ctime_s(timebuff, DATE_TIME_MAX_SIZE, &now);

    String message_representation = timebuff;
    for (size_t i = 0; i < participants_.getSize(); i++)
    {
        const UserBase& temp = participants_[i];
        message_representation += temp.getName();
        message_representation += HashUtility::hash_to_str(temp.getHash());
    }

    const uint8_t* temp = HashUtility::hash_chat(message_representation.c_str());
    HashUtility::copy_hash(hash_, temp);
    delete[] temp;
}

Chat::Chat() = default;

Chat::Chat(const List<UserBase>& participants, ChatType type, const UserBase& owner, const String& name)
{
    participants_ = participants;
    participants_.insertAt(0, owner);
    type_ = type;
    HashUtility::copy_hash(owner_, owner.getHash());

    size_t temp = participants_.getSize();
    if (name.isEmpty())
    {
        for (size_t i = 0; i < temp - 1; i++)
            name_ += participants_[i].getName() + ", ";

        name_ += participants_[temp - 1].getName();
    }
    else
        name_ = name;

    Chat::generate_hash();
}

bool Chat::hasParticipant(const UserBase& user) const
{
    for (size_t i = 0; i < participants_.getSize(); i++)
        if (participants_[i] == user)
            return true;

    return false;
}

bool Chat::hasParticipant(const UserHash& user_hash) const
{
    for (size_t i = 0; i < participants_.getSize(); i++)
        if (participants_[i] == user_hash)
            return true;

    return false;
}

bool Chat::isOwner(const UserBase& user) const
{
    return user == owner_;
}

void Chat::setOwner(const UserBase& user)
{
    if (!hasParticipant(user))
        throw std::exception("User isn't part of the chat");

    HashUtility::copy_hash(owner_, user.getHash());
}

const String& Chat::getName() const
{
    return name_;
}

bool Chat::invitation_control_status() const
{
    return invitation_control_;
}

void Chat::switch_invitation_control()
{
    invitation_control_ = !invitation_control_;

    if (!invitation_control_)
        pending_.clear();
}

const List<UserBase>& Chat::get_pending_invitations() const
{
    return pending_;
}

void Chat::review_invitation(const UserBase& invitation, bool accepted)
{
    size_t id = getInvitationId(invitation);
    if (accepted)
        participants_.add(invitation);

    pending_.removeAt(id);
}

const List<UserBase>& Chat::getParticipants() const
{
    return participants_;
}

size_t Chat::getParticipantsCount() const
{
    return participants_.getSize();
}

const List<Message>& Chat::getMessages() const
{
    return messages_;
}

void Chat::addParticipant(const UserBase& participant)
{
    if (hasParticipant(participant))
        throw std::invalid_argument("Participant already exists");

    participants_.add(participant);
}

void Chat::inviteParticipant(const UserBase& participant)
{
    if (hasParticipant(participant))
        throw std::invalid_argument("Participant already exists");

    pending_.add(participant);
}

void Chat::removeParticipant(const UserBase& participant)
{
    for (size_t i = 0; i < participants_.getSize(); i++)
        if (participants_[i] == participant)
        {
            participants_.removeAt(i);
            return;
        }

    throw std::invalid_argument("Participant not found");
}

void Chat::removeParticipant(const UserHash& participant_hash)
{
    for (size_t i = 0; i < participants_.getSize(); i++)
        if (participants_[i] == participant_hash)
        {
            participants_.removeAt(i);
            return;
        }

    throw std::invalid_argument("Participant not found");
}

void Chat::sentMessage(const UserBase& sender, const String& message)
{
    if (!hasParticipant(sender))
        throw std::runtime_error("Participant isn't part of the chat");

    messages_.add(Message(sender.getName(), message));
}

void Chat::deleteMessage(const Message& message)
{
    for (size_t i = 0; i < messages_.getSize(); i++)
        if (messages_[i] == message)
        {
            messages_.removeAt(i);
            return;
        }

    throw std::invalid_argument("Message not found");
}

void Chat::serialize(std::ofstream& ofs) const
{
    ofs.write((const char*)hash_, HASH_SIZE);

    if (chat_filename_.isEmpty())
        generate_chat_filename();

    std::ofstream chat_ofs(chat_filename_.c_str(), std::ios::binary);
    if (!chat_ofs.is_open())
        throw std::runtime_error("Could not open chat file");

    size_t temp = name_.length();
    chat_ofs.write((const char*)&temp, sizeof(size_t));
    chat_ofs.write(name_.c_str(), temp + 1);

    chat_ofs.write((const char*)&type_, sizeof(ChatType));

    temp = participants_.getSize();
    chat_ofs.write((const char*)&temp, sizeof(size_t));
    for (size_t i = 0; i < temp; i++)
        participants_[i].serialize_base(chat_ofs);

    chat_ofs.write((const char*)&invitation_control_, sizeof(bool));
    if (invitation_control_)
    {
        temp = pending_.getSize();
        chat_ofs.write((const char*)&temp, sizeof(size_t));
        for (size_t i = 0; i < temp; i++)
            pending_[i].serialize_base(chat_ofs);
    }

    this->messages_.serialize(chat_ofs);

    chat_ofs.close();
}

void Chat::deserialize(std::ifstream& ifs)
{
    ifs.read((char*)hash_, HASH_SIZE);

    if (chat_filename_.isEmpty())
        generate_chat_filename();

    std::ifstream chat_ifs(chat_filename_.c_str(), std::ios::binary);
    if (!chat_ifs.is_open())
        throw std::runtime_error("Could not open chat file");

    size_t temp;
    chat_ifs.read((char*)&temp, sizeof(size_t));
    char* str = new char[temp + 1];
    chat_ifs.read(str, temp + 1);
    name_ = str;
    delete[] str;

    chat_ifs.read((char*)&type_, sizeof(ChatType));

    chat_ifs.read((char*)&temp, sizeof(size_t));
    for (size_t i = 0; i < temp; i++)
    {
        UserBase user;
        user.deserialize_base(chat_ifs);
        participants_.add(user);
    }

    chat_ifs.read((char*)&invitation_control_, sizeof(bool));
    if (invitation_control_)
    {
        chat_ifs.read((char*)&temp, sizeof(size_t));
        for (size_t i = 0; i < temp; i++)
            pending_[i].deserialize_base(chat_ifs);
    }

    this->messages_.deserialize(chat_ifs);

    chat_ifs.close();
}

void Chat::serialize_debug(std::ofstream& ofs) const
{
    HashUtility::serialize_hash_text(ofs, hash_);

    if (chat_filename_.isEmpty())
        generate_chat_filename(true);

    std::ofstream chat_ofs(chat_filename_.c_str(), std::ios::out | std::ios::beg);
    if (!chat_ofs.is_open())
        throw std::runtime_error("Could not open chat file");

    chat_ofs << name_ << '\n';
    chat_ofs << (int)type_ << '\n';

    chat_ofs << participants_.getSize() << '\n';
    for (size_t i = 0; i < participants_.getSize(); i++)
        participants_[i].serialize_base_debug(chat_ofs);

    chat_ofs << invitation_control_ << '\n';
    if (invitation_control_)
    {
        chat_ofs << pending_.getSize() << '\n';
        for (size_t i = 0; i < pending_.getSize(); i++)
            pending_[i].serialize_base_debug(chat_ofs);
    }

    this->messages_.serialize_debug(chat_ofs);

    chat_ofs.close();
}

void Chat::deserialize_debug(std::ifstream& ifs)
{
    HashUtility::deserialize_hash_text(ifs, hash_);

    if (chat_filename_.isEmpty())
        generate_chat_filename(true);

    std::ifstream chat_ifs(chat_filename_.c_str(), std::ios::in | std::ios::beg);
    if (!chat_ifs.is_open())
        throw std::runtime_error("Could not open chat file");

    chat_ifs >> name_;
    int type;
    chat_ifs >> type;
    if ((ChatType)type == ChatType::DIRECT)
        type_ = ChatType::DIRECT;
    else if ((ChatType)type == ChatType::GROUP)
        type_ = ChatType::GROUP;
    else
        throw std::runtime_error("Could not deserialize chat");

    size_t temp;
    chat_ifs >> temp;
    for (size_t i = 0; i < temp; i++)
    {
        UserBase user;
        user.deserialize_base_debug(chat_ifs);
        participants_.add(user);
    }

    chat_ifs >> invitation_control_;
    if (invitation_control_)
    {
        chat_ifs >> temp;
        for (size_t i = 0; i < temp; i++)
            pending_[i].deserialize_base_debug(chat_ifs);
    }

    this->messages_.deserialize_debug(chat_ifs);

    chat_ifs.close();
}

std::ostream& operator<<(std::ostream& os, const Chat& chat)
{
    os << chat.name_ << " | ";
    if (chat.type_ == ChatType::DIRECT)
        os << "Direct";
    else if (chat.type_ == ChatType::GROUP)
        os << "Group" << " (" << chat.getParticipantsCount() << " members)";
    else
        throw std::runtime_error("Could not deserialize chat");
    os << " | " << "messages: " << chat.messages_.getSize();


    return os;
}
