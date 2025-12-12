#pragma once

class ImChampCommandReceiver
{
public:
    ImChampCommandReceiver()         {}
    virtual ~ImChampCommandReceiver(){}
public:
    virtual void CommandFeedback(CString a_strResult) = 0;
    virtual void Canceled       (                   ) = 0;
};


class ImChampCommand
{
private:
    CString                 m_strCommand;
    std::vector<CString>    m_vecSelectaleInfo;
    ImChampCommandReceiver*   m_pResultReceiver;
    bool                    m_bMultiSelect;
public:
    ImChampCommand(CString a_strCommand, ImChampCommandReceiver* a_pResultReceiver);
    ImChampCommand(CString a_strCommand, std::vector<CString>  a_setSelectaleInfo, ImChampCommandReceiver* a_pResultReceiver, bool a_bMultiSelect=false);
    virtual ~ImChampCommand(void);

public:
    CString GetCommand       ();    
    bool    EnableMultiSelect();
    ImChampCommandReceiver* GetReceiver      ();    
    std::vector<CString>  GetSelectableInfo();

    bool EnableCommand(CString a_strCommand);    
    
};

typedef std::shared_ptr<ImChampCommand> ImChampModalCommandPtr;