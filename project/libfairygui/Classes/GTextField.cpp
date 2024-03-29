#include "GTextField.h"
#include "utils/ToolSet.h"

NS_FGUI_BEGIN
USING_NS_CC;

GTextField::GTextField()
    :_templateVars(nullptr),
    _ubbEnabled(false),
    _autoSize(TextAutoSize::BOTH)
{
}

GTextField::~GTextField()
{
    CC_SAFE_DELETE(_templateVars);
}

void GTextField::setText(const std::string & value)
{
    _text = value;
    setTextFieldText();
    updateGear(6);
    updateSize();
}

void GTextField::setUBBEnabled(bool value)
{
    if (_ubbEnabled != value)
    {
        _ubbEnabled = value;
        setTextFieldText();
        updateSize();
    }
}

void GTextField::setColor(const cocos2d::Color3B & value)
{
    getTextFormat()->color = value;
    applyTextFormat();
}

void GTextField::setFontSize(float value)
{
    getTextFormat()->fontSize = value;
    applyTextFormat();
}

void GTextField::setOutlineColor(const cocos2d::Color3B & value)
{
    getTextFormat()->outlineColor = value;
    applyTextFormat();
}

void GTextField::setTemplateVars(cocos2d::ValueMap* value)
{
    if (_templateVars == nullptr && value == nullptr)
        return;

    if (value == nullptr)
        CC_SAFE_DELETE(_templateVars);
    else
    {
        if (_templateVars == nullptr)
            _templateVars = new cocos2d::ValueMap();
        *_templateVars = *value;
    }

    flushVars();
}

GTextField* GTextField::setVar(const std::string& name, const cocos2d::Value& value)
{
    if (_templateVars == nullptr)
        _templateVars = new cocos2d::ValueMap();

    (*_templateVars)[name] = value;

    return this;
}

void GTextField::flushVars()
{
    setTextFieldText();
    updateSize();
}

void GTextField::updateSize()
{
}

void GTextField::setup_BeforeAdd(TXMLElement * xml)
{
    GObject::setup_BeforeAdd(xml);

    TextFormat* tf = getTextFormat();
    const char*p;
    p = xml->Attribute("font");
    if (p)
        tf->face = p;

    p = xml->Attribute("fontSize");
    if (p)
        tf->fontSize = atoi(p);

    p = xml->Attribute("color");
    if (p)
        tf->color = (Color3B)ToolSet::convertFromHtmlColor(p);

    p = xml->Attribute("align");
    if (p)
        tf->align = ToolSet::parseAlign(p);

    p = xml->Attribute("vAlign");
    if (p)
        tf->verticalAlign = ToolSet::parseVerticalAlign(p);

    p = xml->Attribute("leading");
    if (p)
        tf->lineSpacing = atoi(p);

    p = xml->Attribute("letterSpacing");
    if (p)
        tf->letterSpacing = atoi(p);

    p = xml->Attribute("ubb");
    if (p)
        _ubbEnabled = strcmp(p, "true") == 0;

    p = xml->Attribute("autoSize");
    if (p)
        setAutoSize(ToolSet::parseTextAutoSize(p));

    p = xml->Attribute("underline");
    if (p)
        tf->underline = strcmp(p, "true") == 0;

    p = xml->Attribute("italic");
    if (p)
        tf->italics = strcmp(p, "true") == 0;

    p = xml->Attribute("bold");
    if (p)
        tf->bold = strcmp(p, "true") == 0;

    p = xml->Attribute("singleLine");
    if (p)
        setSingleLine(strcmp(p, "true") == 0);

    p = xml->Attribute("strokeColor");
    if (p)
    {
        tf->outlineColor = (Color3B)ToolSet::convertFromHtmlColor(p);
        p = xml->Attribute("strokeSize");
        tf->outlineSize = p ? atoi(p) : 1;
        tf->enableEffect(TextFormat::OUTLINE);
    }

    p = xml->Attribute("shadowColor");
    if (p)
    {
        tf->shadowColor = (Color3B)ToolSet::convertFromHtmlColor(p);

        Vec2 offset;
        p = xml->Attribute("shadowOffset");
        if (p)
            ToolSet::splitString(p, ',', offset);
        offset.y = -offset.y;
        tf->shadowOffset = offset;
        tf->enableEffect(TextFormat::SHADOW);
    }

    if (xml->BoolAttribute("vars") && _templateVars == nullptr)
        _templateVars = new cocos2d::ValueMap();
}

void GTextField::setup_AfterAdd(TXMLElement * xml)
{
    GObject::setup_AfterAdd(xml);

    applyTextFormat();

    const char* p;
    p = xml->Attribute("text");
    if (p && strlen(p) > 0)
        setText(p);
}

std::string GTextField::parseTemplate(const char* text)
{
    const char* pString = text;

    ssize_t pos;
    ssize_t pos2;
    std::string tag, attr;
    std::string repl;
    std::string out;

    while (*pString != '\0')
    {
        const char* p = strchr(pString, '{');
        if (!p)
        {
            out.append(pString);
            break;
        }

        pos = p - pString;
        if (pos > 0 && *(p - 1) == '\\')
        {
            out.append(pString, pos - 1);
            out.append("{");
            pString += pos + 1;
            continue;
        }

        out.append(pString, pos);
        pString += pos;

        p = strchr(pString, '}');
        if (!p)
        {
            out.append(pString);
            break;
        }

        pos = p - pString;
        if (pos == 1)
        {
            out.append(pString, 0, 2);
            pString += 2;
            continue;
        }

        tag.assign(pString + 1, pos - 1);

        attr.clear();
        repl.clear();
        pos2 = tag.find('=');
        if (pos2 != -1)
        {
            auto it = _templateVars->find(tag.substr(0, pos2));
            if (it != _templateVars->end())
                out.append(it->second.asString());
            else
                out.append(tag.substr(pos2 + 1));
        }
        else
        {
            auto it = _templateVars->find(tag);
            if (it != _templateVars->end())
                out.append(it->second.asString());
        }
        pString += pos + 1;
    }
    return out;
}

//---------------------------

GBasicTextField::GBasicTextField() :
    _label(nullptr),
    _updatingSize(false)
{
    _touchDisabled = true;
}

GBasicTextField::~GBasicTextField()
{
}

void GBasicTextField::handleInit()
{
    _label = FUILabel::create();
    _label->retain();

    _displayObject = _label;
}

void GBasicTextField::applyTextFormat()
{
    _label->applyTextFormat();
    updateGear(4);
    if (!_underConstruct)
        updateSize();
}

void GBasicTextField::setAutoSize(TextAutoSize value)
{
    _autoSize = value;
    switch (value)
    {
    case TextAutoSize::NONE:
        _label->setOverflow(Label::Overflow::CLAMP);
        break;
    case TextAutoSize::BOTH:
        _label->setOverflow(Label::Overflow::NONE);
        break;
    case TextAutoSize::HEIGHT:
        _label->setOverflow(Label::Overflow::RESIZE_HEIGHT);
        break;
    case TextAutoSize::SHRINK:
        _label->setOverflow(Label::Overflow::SHRINK);
        break;
    }

    if (_autoSize == TextAutoSize::BOTH)
        _label->setDimensions(0, 0);
    else if (_autoSize == TextAutoSize::HEIGHT)
        _label->setDimensions(_size.width, 0);
    else
        _label->setDimensions(_size.width, _size.height);

    if (!_underConstruct)
        updateSize();
}

void GBasicTextField::setSingleLine(bool value)
{
    _label->enableWrap(!value);
    if (!_underConstruct)
        updateSize();
}

void GBasicTextField::setTextFieldText()
{
    if (_templateVars != nullptr)
        _label->setText(parseTemplate(_text.c_str()));
    else
        _label->setText(_text);
}

void GBasicTextField::updateSize()
{
    if (_updatingSize)
        return;

    _updatingSize = true;

    Size sz = _label->getContentSize();
    if (_autoSize == TextAutoSize::BOTH)
        setSize(sz.width, sz.height);
    else if (_autoSize == TextAutoSize::HEIGHT)
        setHeight(sz.height);

    _updatingSize = false;
}

void GBasicTextField::handleSizeChanged()
{
    if (_updatingSize)
        return;

    if (_autoSize != TextAutoSize::BOTH)
    {
        _label->setDimensions(_size.width, _size.height);

        if (_autoSize == TextAutoSize::HEIGHT)
        {
            if (!_text.empty())
                setSizeDirectly(_size.width, _label->getContentSize().height);
        }
    }
}

void GBasicTextField::handleGrayedChanged()
{
    GObject::handleGrayedChanged();

    _label->setGrayed(_finalGrayed);
}

NS_FGUI_END
