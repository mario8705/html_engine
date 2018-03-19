#pragma once
#include "DOMNode.h"
#include <string>
#include <unordered_map>
#include <vector>

class HTMLElement : public ADOMNode
{
public:
    HTMLElement(const std::string &tagName, HTMLElement *parent = nullptr);
    ~HTMLElement();

    std::string GetTagName() const;

    bool HasAttribute(std::string name) const;
    std::string GetAttribute(std::string name) const;
    std::string GetAttribute(std::string name, std::string defaultValue) const;
    void SetAttribute(std::string name, std::string value);

    void AppendChildren(ADOMNode *child);
    std::vector<ADOMNode *> GetChildrens() const;

    inline float GetClientWidth() const { return m_clientWidth; }
    inline float GetClientHeight() const { return m_clientHeight; }
    inline void SetClientWidth(float clientWidth) { m_clientWidth = clientWidth; }
    inline void SetClientHeight(float clientHeight) { m_clientHeight = clientHeight; }

    Stylesheet *GetStylesheet() const { return m_stylesheet; }

    void DumpTree(int ident = 1) const;

private:
    float m_clientWidth, m_clientHeight;

    std::string m_tagName;
    std::unordered_map<std::string, std::string> m_attributes;
    std::vector<ADOMNode *> m_childrens;
};