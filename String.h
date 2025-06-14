#pragma once
#include <iosfwd>

class String
{
    char* data_;
    size_t capacity_;

    void copyFrom(const char* str);
    void copyFrom(const String& other);
    void moveFrom(String&& other);
    void free();
    void resize(size_t minSize);

public:
    String();
    String(char ch);
    String(const char* str);
    String(const String& other);
    String& operator=(const String& other);
    String& operator=(const char* str);
    ~String();

    String(String&& other) noexcept;
    String& operator=(String&& other) noexcept;

    const char& operator[](size_t index) const;
    char& operator[](size_t index);

    void clear();

    size_t length() const;
    bool isEmpty() const;

    void reverse();
    const char* c_str() const;

    String& operator+=(const String& other);

    friend std::ostream& operator<<(std::ostream& os, const String& str);
    friend std::istream& operator>>(std::istream& is, String& str);

    static void getline(std::istream& is, String& str);
};

String operator+(const String& left, const String& right);
