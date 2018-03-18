#pragma once
#include <string>
#include <memory>
#include "MainWindow.h"
#include "DOMDocument.h"

class HTMLEngine
{
public:
    HTMLEngine();

    void LoadURL(const std::string &flename);

    void Run();

private:
    std::unique_ptr<MainWindow> m_mainWindow;
    std::unique_ptr<DOMDocument> m_document;
};
