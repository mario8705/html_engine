#pragma once

class CharacterBuffer;

class HTMLParser
{
public:
    HTMLParser();

    void Parse(CharacterBuffer &buf);
};
