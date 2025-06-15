#include "AppManager.h"

#include "Admin.h"
#include "Member.h"

void AppManager::help_command()
{
    std::cout << "Commands:\n";
    std::cout << "\t1) help\n";
    std::cout << "\t2) login\n";
    std::cout << "\t3) register\n";
    std::cout << "\t4) exit\n";
}

void AppManager::login_command(const String& username, const String& password)
{
    try
    {
        User& user = data_.getUser(username);
        if (!user.checkPassword(password))
            throw std::invalid_argument("Invalid username or password");
    }
    catch (std::invalid_argument& e)
    {
        std::cout << e.what() << "\n";
        std::cout << "Invalid username or password";
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
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    login_command(username, password);
}

void AppManager::register_input()
{
    String username, password;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    register_command(username, password);
}

AppManager::AppManager()
{
    try
    {
        data_.loadData();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n' << '\n';
        std::cout << "Creating new database...\n";
        Admin admin("Admin", "0000");
        data_.addUser(Admin("Admin", "0000"));
        std::cout << "DEBUG: Admin code - " << admin.getCode() << '\n';
        std::cout << "Database successfully created!\n";
    }
}

AppManager::~AppManager()
{
    try
    {
        data_.saveData();
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
