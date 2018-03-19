#pragma once
#include "color.h"

class ADOMNode;

class Stylesheet
{
public:
    enum BackgroundType
    {
        BT_Transparent,
        BT_Image,
        BT_SolidColor
    };

    enum TextAlignment
    {
        Left,
        Center,
        Right,
        Justify // No supported yet
    };

    explicit Stylesheet(ADOMNode *element);

    void LoadUserAgentStyles();

    void SetBackgroundColor(color_t color) { m_backgroundColor = color; }
    color_t GetBackgroundColor() const { return m_backgroundColor; }

    BackgroundType GetBackgroundType() const { return m_backgroundType; }
    void SetBackgroundType(BackgroundType backgroundType) { m_backgroundType = backgroundType; }

    color_t GetTextColor() const;
    TextAlignment GetTextAlignment() const;
    float GetFontSize() const;

    ADOMNode *GetParent() const;

private:
    ADOMNode *m_parent;

    float m_fontSize;
    BackgroundType m_backgroundType;
    color_t m_backgroundColor;
};
