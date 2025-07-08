#pragma once
#include <iosfwd>

// Lubomir Vasilev Topalski
// 8MI0600446 SE 1 Year Group 2

template <class T>
class List;

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

    String toLower() const;
    String toUpper() const;

    size_t length() const;
    bool isEmpty() const;

    String substr(size_t start, size_t end) const;
    List<String> split(const String& separator) const;

    void reverse() const;
    const char* c_str() const;

    String& operator+=(const String& other);

    void serialize(std::ofstream& ofs) const;
    void deserialize(std::ifstream& ifs);

    friend std::ostream& operator<<(std::ostream& os, const String& str);
    friend std::istream& operator>>(std::istream& is, String& str);

    static void getline(std::istream& is, String& str);
};

String operator+(const String& left, const String& right);
bool operator==(const String& left, const String& right);
