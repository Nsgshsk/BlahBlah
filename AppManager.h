#pragma once
#include "DataRepository.h"

class AppManager
{
    DataRepository data_;

    static void help_command();
    void login_command(const String& username, const String& password);
    void register_command(const String& username, const String& password);

    void login_input();
    void register_input();

public:
    AppManager();
    ~AppManager();

    void run();
};
