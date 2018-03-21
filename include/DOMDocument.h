#pragma once
#include <functional>
#include <map>
#include <string>

class HTMLElement;

class DOMDocument
{
public:
    DOMDocument();

    HTMLElement *GetRootElement();

    HTMLElement *CreateElement(const std::string &tagName);

    void LoadDocument(const std::string &path);

    HTMLElement *FindLastOf(const std::string &tagName) const;

    std::string GetTitle() const;

private:
    HTMLElement *m_rootElement;
    std::string m_title;
    std::map<std::string, std::function<HTMLElement *(std::string)>> m_registeredElements;
};