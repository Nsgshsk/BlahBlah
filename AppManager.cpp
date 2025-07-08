#include "AppManager.h"

#include "Admin.h"
#include "AdminManager.h"
#include "Member.h"

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

void AppManager::help_command()
{
    std::cout << "Commands:\n";
    std::cout << "\t1) help\n";
    std::cout << "\t2) login <username> <password>\n";
    std::cout << "\t3) register <username> <password>\n";
    std::cout << "\t4) exit\n";
}

void AppManager::login_command(const String& username, const String& password)
{
    try
    {
        User& user = data_.getUser(username);
        if (!user.checkPassword(password))
            throw std::invalid_argument("Invalid username or password");

        if (user.getRole() == UserRole::ADMIN)
        {
            AdminManager manager(&user, &data_);
            manager.login();
        }
        else if (user.getRole() == UserRole::MEMBER)
        {
            MemberManager manager(&user, &data_);
            manager.login();
        }
        else
            throw std::runtime_error("Invalid user role!");
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}

void AppManager::register_command(const String& username, const String& password)
{
    try
    {
        if (data_.hasUser(username))
            throw std::invalid_argument("Username already taken");

        Member user(username, password);
        data_.addUser(user);
        std::cout << username << " registered successfully!\n";
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}

void AppManager::login_input()
{
    String username, password;
    std::cin >> username;
    std::cin >> password;

    login_command(username, password);
}

void AppManager::register_input()
{
    String username, password;
    std::cin >> username;
    std::cin >> password;

    register_command(username, password);
}

AppManager::AppManager(bool debug)
{
    debug_ = debug;
    try
    {
        if (!debug_)
            data_.loadData();
        else
            data_.loadDataDebug();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n' << '\n';
        std::cout << "Creating new database...\n";
        Admin admin("Admin", "0000");
        data_.addUser(admin);
        std::cout << "DEBUG: Admin code - " << admin.getCode() << '\n';
        // This is the name with which you can interact with the admin in the app (like login, chatting, etc.)
        std::cout << "DEBUG: Admin access name - " << admin.getName() << admin.getCode() << '\n';
        std::cout << "Database successfully created!\n";
    }
}

AppManager::~AppManager()
{
    try
    {
        if (!debug_)
            data_.saveData();
        else
            data_.saveDataDebug();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
        std::cout << "Database won't be saved!!!\n";
    }
}

void AppManager::run()
{
    try
    {
        std::cout << "Use help if you don't remember the commands.\n";
        String input;
        while (true)
        {
            std::cout << "> ";
            std::cin >> input;

            if (input == "exit")
                break;

            if (input == "help")
                help_command();
            else if (input == "login")
                login_input();
            else if (input == "register")
                register_input();
            else
                std::cout << "Invalid command!\n";
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Fatal error: " << e.what() << '\n';
    }

    std::cout << "Shutting down...\n";
}
