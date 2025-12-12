#include "stdafx.h"
#include "ImChampCommand.h"

ImChampCommand::ImChampCommand( CString a_strCommand, ImChampCommandReceiver* a_pResultReceiver )
{
    m_strCommand       = a_strCommand;
    m_pResultReceiver  = a_pResultReceiver;
    m_bMultiSelect     = false;
}

ImChampCommand::ImChampCommand( CString a_strCommand, std::vector<CString> a_setSelectaleInfo, ImChampCommandReceiver* a_pResultReceiver, bool a_bMultiSelect/*=false*/ )
{
    m_strCommand       = a_strCommand;
    m_vecSelectaleInfo = a_setSelectaleInfo;
    m_pResultReceiver  = a_pResultReceiver;
    m_bMultiSelect     = a_bMultiSelect;
}

ImChampCommand::~ImChampCommand( void )
{

}

CString ImChampCommand::GetCommand()
{
    return m_strCommand;
}

bool ImChampCommand::EnableMultiSelect()
{
    return m_bMultiSelect;
}

ImChampCommandReceiver* ImChampCommand::GetReceiver()
{
    return m_pResultReceiver;
}

std::vector<CString> ImChampCommand::GetSelectableInfo()
{
    return m_vecSelectaleInfo;
}

bool ImChampCommand::EnableCommand( CString a_strCommand )
{
    size_t nxCount = m_vecSelectaleInfo.size();
    if (nxCount == 0) return true;

    CString strTok;
    std::set<CString> setTokenCommand;

    int i=0;
    while (AfxExtractSubString(strTok, a_strCommand, i++, ','))
    {
        setTokenCommand.insert(strTok.MakeUpper());
    }
    
    if (! m_bMultiSelect)
    {
        if (setTokenCommand.size() > 1)
            return false;
    }

    int nExistCount = 0;
    std::set<CString>::iterator iter = setTokenCommand.begin();
    std::set<CString>::iterator end  = setTokenCommand.end  ();
    for(size_t i=0; i<nxCount; i++)
    {
        iter = setTokenCommand.find(m_vecSelectaleInfo[i].MakeUpper());
        if (iter != end) 
            nExistCount++;
    }

    if (nExistCount != setTokenCommand.size())
        return false;

    return true;
}



