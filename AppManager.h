#pragma once
#include "DataRepository.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

class AppManager
{
    DataRepository data_;
    bool debug_;

    static void help_command();
    void login_command(const String& username, const String& password);
    void register_command(const String& username, const String& password);

    void login_input();
    void register_input();

public:
    explicit AppManager(bool debug = false);
    AppManager(const AppManager&) = delete;
    AppManager& operator=(const AppManager&) = delete;
    ~AppManager();

    AppManager(AppManager&&) = delete;
    AppManager& operator=(AppManager&&) = delete;

    void run();
};
