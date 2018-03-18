#pragma once
#include <cstdlib>
#include <string>

class CharacterBuffer
{
public:
    CharacterBuffer(char *buffer, size_t size);
    ~CharacterBuffer();

    void TrimLeft();
    bool IsEmpty() const;

    std::string GetRegex(const std::string &pattern);

    void operator++();
    char operator*() const;
    operator bool() const;

    static CharacterBuffer LoadFromFile(const std::string &filename);

    char *start;
    char *end;

private:
    char *m_buffer;
};
