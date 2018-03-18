#include "CharacterBuffer.h"
#include <iostream>
#include <fstream>
#include <regex>

#define MAX_FILE_SIZE (1 * 1024 * 1024)

CharacterBuffer::CharacterBuffer(char *buffer, size_t size)
    : m_buffer(buffer), start(buffer), end(buffer + size)
{
}

CharacterBuffer::~CharacterBuffer()
{
    delete[] m_buffer;
}

void CharacterBuffer::TrimLeft()
{
    while (start != end && isspace(*start))
    {
        ++start;
    }
}

bool CharacterBuffer::IsEmpty() const
{
    return start == end;
}

std::string CharacterBuffer::GetRegex(const std::string &pattern)
{
    std::regex regex(pattern);
    std::cmatch match;

    if (std::regex_search((const char *)start, (const char *)end, match, regex))
    {
        start += match.length();
        return match.str();
    }

    return "";
}

void CharacterBuffer::operator++()
{
    ++start;
}

char CharacterBuffer::operator*() const
{
    return *start;
}

CharacterBuffer::operator bool() const
{
    return start != end;
}

CharacterBuffer CharacterBuffer::LoadFromFile(const std::string &filename)
{
    std::ifstream in(filename, std::ios::ate);
    size_t size = in.tellg();
    in.seekg(0);

    if (size >= MAX_FILE_SIZE)
    {
        printf("Max file size exceeded\n");
        throw std::runtime_error("Max file size exceeded");
    }

    char *buf = new char[size];
    in.read(buf, size);

    return CharacterBuffer(buf, size);
}
