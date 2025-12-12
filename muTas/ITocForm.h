#pragma once

class ITocParent
{
public:
    ITocParent() {};
    virtual ~ITocParent() {};
public:
    virtual void TemplateChanged(LPCTSTR a_strTemplateName) = 0;
};

class ITocChild
{
public:
    ITocChild() {};
    virtual ~ITocChild() {};
public:
    virtual void ReloadTemplate(LPCTSTR a_strTemplateName) = 0;
};