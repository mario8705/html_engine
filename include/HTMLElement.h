#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class Stylesheet;

class HTMLElement
{
public:
    HTMLElement(const std::string &tagName, HTMLElement *parent = nullptr);
    ~HTMLElement();

    std::string GetTagName() const;

    bool HasAttribute(std::string name) const;
    std::string GetAttribute(std::string name) const;
    std::string GetAttribute(std::string name, std::string defaultValue) const;
    void SetAttribute(std::string name, std::string value);

    HTMLElement *GetParent() const;

    void AppendChildren(HTMLElement *child);
    std::vector<HTMLElement *> GetChildrens() const;

    // Deprecated stuff
    inline std::string GetData() const { return m_data; }
    inline void SetData(std::string data) { m_data = data; }

    inline float GetClientWidth() const { return m_clientWidth; }
    inline float GetClientHeight() const { return m_clientHeight; }
    inline void SetClientWidth(float clientWidth) { m_clientWidth = clientWidth; }
    inline void SetClientHeight(float clientHeight) { m_clientHeight = clientHeight; }

    Stylesheet *GetStylesheet() const { return m_stylesheet; }

    void DumpTree(int ident = 1) const;

private:
    std::string m_data;
    float m_clientWidth, m_clientHeight;

    std::string m_tagName;
    Stylesheet *m_stylesheet;
    std::unordered_map<std::string, std::string> m_attributes;
    HTMLElement *m_parent;
    std::vector<HTMLElement *> m_childrens;
};