#include "BaseManager.h"

#include <stdexcept>

#include "String.h"

BaseManager::BaseManager(User* user, DataRepository* data)
{
    if (!user || !data)
        throw std::invalid_argument("Invalid user or data");

    user_ = user;
    data_ = data;
}

void BaseManager::login()
{
    try
    {
        String input;
        while (true)
        {
        }
    }
    catch (...)
    {
    }
}
