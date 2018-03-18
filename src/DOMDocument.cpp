#include "DOMDocument.h"
#include "HTMLElement.h"

using namespace std;

DOMDocument::DOMDocument()
{
    m_rootElement = new HTMLElement("");
    // m_registeredElements["p"] 
}

HTMLElement *DOMDocument::GetRootElement()
{
    return m_rootElement;
}

HTMLElement *DOMDocument::CreateElement(const string &tagName)
{
    auto it = m_registeredElements.find(tagName);

    if (it != m_registeredElements.end())
    {
        return (it->second)(tagName);
    }

    return new HTMLElement(tagName);    
}
#include "dom.h"
void DOMDocument::LoadDocument(const std::string &path)
{
    std::vector<HTMLToken> tokens;
    size_t sz;
    char *buf = load_all(path.c_str(), &sz);

    buf_it it = { buf, buf + sz };
    parse(&it, tokens);

    delete m_rootElement;
    m_rootElement = create_dom(tokens);

    m_rootElement->DumpTree();
}