#pragma once
#include <functional>
#include <map>

class HTMLElement;

class DOMDocument
{
public:
    DOMDocument();

    HTMLElement *GetRootElement();

    HTMLElement *CreateElement(const std::string &tagName);

    void LoadDocument(const std::string &path);

private:
    HTMLElement *m_rootElement;
    std::map<std::string, std::function<HTMLElement *(std::string)>> m_registeredElements;
};