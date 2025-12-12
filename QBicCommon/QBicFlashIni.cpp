#include "stdafx.h"
#include "QBicFlashIni.h"
#include "QBicFileIni.h"
#include "ParseString.h"

QBicIni::QBicIni()
{
}

QBicIni::~QBicIni()
{
}


void QBicIni::Write( CString strKey, CString strValue )
{
    strKey.MakeLower();
    m_mapKeyValue[strKey] = strValue;
}

void QBicIni::Write( CString strKey, int nValue )
{
    CString strValue(_T("")); {
        strValue.Format(_T("%d"), nValue);
    }

    Write(strKey, strValue);
}

void QBicIni::Write( CString strKey, double dValue )
{
    CString strValue(_T("")); {
        strValue.Format(_T("%f"), dValue);
    }

    Write(strKey, strValue);
}

void QBicIni::Write( CString strKey, __int64 nxValue )
{
    CString strValue(_T("")); {
        strValue.Format(_T("%I64d"), nxValue);
    }

    Write(strKey, strValue);
}

void QBicIni::Write( CString strKey, COLORREF clrColor )
{
    CString strValue(_T("")); {
        strValue.Format(_T("%d,%d,%d"), GetRValue(clrColor), GetGValue(clrColor), GetBValue(clrColor));
    }

    Write(strKey, strValue);
}

void QBicIni::Write( CString strKey, std::vector<int>& vecValue )
{
    CString strValue(_T(""));    
    for (size_t i = 0; i < vecValue.size(); i++) {
        if (i == 0) {
            strValue.AppendFormat(_T("%d"), vecValue[i]);
        } else {
            strValue.AppendFormat(_T(",%d"), vecValue[i]);
        }
    }

    Write(strKey, strValue);
}

std::vector<int> QBicIni::ReadVecInt( CString strKey, std::vector<int>& vecValue )
{
    strKey.MakeLower(); {        
    } 

    auto iter = m_mapKeyValue.find(strKey);
    if (iter == m_mapKeyValue.end()) {
        Write(strKey, vecValue);
        return vecValue;
    } 

    CString strValue = iter->second;
    std::vector<CString> vecSplit = QBicSplit::Split(strValue, _T(','));

    std::vector<int> vecRInt; 
    for (size_t i=0; i<vecSplit.size(); i++) {
        vecRInt.push_back(_ttoi(vecSplit[i]));
    }

    return vecRInt;
}

CString QBicIni::Read( CString strKey, CString strDefault/*=_T("")*/ )
{
    strKey.MakeLower();
    auto iter = m_mapKeyValue.find(strKey);
    if (iter == m_mapKeyValue.end()) {
        m_mapKeyValue[strKey] = strDefault; 
        return strDefault;
    }
    else {
        return iter->second;
    }
}

int QBicIni::ReadInt( CString strKey, int nDefault )
{
    CString strDefault(_T("")); {
        strDefault.Format(_T("%d"), nDefault);
    }

    return _ttoi(Read(strKey, strDefault));
}

double QBicIni::ReadDbl( CString strKey, double dDefault )
{
    CString strDefault(_T("")); {
        strDefault.Format(_T("%f"), dDefault);
    }

    return _ttof(Read(strKey, strDefault));
}

__int64 QBicIni::ReadI64( CString strKey, __int64 nxDefault )
{
    CString strDefault(_T("")); {
        strDefault.Format(_T("%I64d"), nxDefault);
    }

    return _ttoi64(Read(strKey, strDefault));
}

COLORREF QBicIni::ReadRGB( CString strKey, COLORREF clrDefault )
{
    COLORREF clrOut = clrDefault;

    CString strValue = Read(strKey, _T(""));
    if (strValue.GetLength() == 0) {
        Write(strKey, clrDefault);
    }

    std::vector<CString> vecRGB = QBicSplit::Split(strValue, _T(','));
    if (vecRGB.size() == 3) {
        int nR = _ttoi(vecRGB[0]);
        int nG = _ttoi(vecRGB[1]);
        int nB = _ttoi(vecRGB[2]);                 
        clrOut = RGB(nR, nG, nB);
    }    

    return clrOut;
}

QBicGrpIni::QBicGrpIni()
{
}


QBicGrpIni::~QBicGrpIni()
{
}

void QBicGrpIni::Write(int nType, CString strKey, CString strValue )
{
    strKey.MakeLower();
    std::map<CString, CString>& mapKeyVal = m_mapTypeKeyValue[nType]; {
        mapKeyVal[strKey] = strValue; 
    }       
}

void QBicGrpIni::Write( int nType, CString strKey, int nValue )
{
    CString strValue(_T("")); {
        strValue.Format(_T("%d"), nValue);
    }

    Write(nType, strKey, strValue);
}

void QBicGrpIni::Write( int nType, CString strKey, double dValue )
{
    CString strValue(_T("")); {
        strValue.Format(_T("%f"), dValue);
    }

    Write(nType, strKey, strValue);
}

void QBicGrpIni::Write( int nType, CString strKey, __int64 nxValue )
{
    CString strValue(_T("")); {
        strValue.Format(_T("%I64d"), nxValue);
    }

    Write(nType, strKey, strValue);
}

void QBicGrpIni::Write( int nType, CString strKey, COLORREF clrColor)
{
    CString strValue(_T("")); {
        strValue.Format(_T("%d,%d,%d"), GetRValue(clrColor), GetGValue(clrColor), GetBValue(clrColor));
    }

    Write(nType, strKey, strValue);
}

void QBicGrpIni::Write( int nType, CString strKey, std::shared_ptr<QBicIni> spIni)
{
    //~Key|Value~Key|Value
    std::map<CString, CString>& mapKeyValue = spIni->m_mapKeyValue;

    CString strValue(_T(""));    
    bool bFirst(true);
    for (auto iter = mapKeyValue.begin(); iter != mapKeyValue.end(); ++iter) {
        if (bFirst == true) {
            bFirst = false;
            strValue.AppendFormat(_T("%s|%s" ), iter->first, iter->second);
        } else {
            strValue.AppendFormat(_T("~%s|%s"), iter->first, iter->second);
        }
    }

    Write(nType, strKey, strValue);
}

void QBicGrpIni::Write( int nType, CString strKey, std::vector<int>& vecValue )
{
    CString strValue(_T(""));    
    for (size_t i = 0; i < vecValue.size(); i++) {
        if (i == 0) {
            strValue.AppendFormat(_T("%d"), vecValue[i]);
        } else {
            strValue.AppendFormat(_T(",%d"), vecValue[i]);
        }
    }

    Write(nType, strKey, strValue);
}

std::vector<int> QBicGrpIni::ReadVecInt( int nType, CString strKey, std::vector<int>& vecValue )
{
    strKey.MakeLower(); {        
    } 

    std::map<CString, CString>& mapKeyVal = m_mapTypeKeyValue[nType];

    auto iter = mapKeyVal.find(strKey);
    if (iter == mapKeyVal.end()) {
        Write(nType, strKey, vecValue);
        return vecValue;
    } 

    CString strValue = iter->second;
    std::vector<CString> vecSplit = QBicSplit::Split(strValue, _T(','));

    std::vector<int> vecRInt; 
    for (size_t i=0; i<vecSplit.size(); i++) {
        vecRInt.push_back(_ttoi(vecSplit[i]));
    }

    return vecRInt;
}

std::shared_ptr<QBicIni> QBicGrpIni::ReadIni( int nType, CString strKey, std::shared_ptr<QBicIni> spDefault )
{
    strKey.MakeLower(); {        
    }    

    std::map<CString, CString>& mapKeyVal = m_mapTypeKeyValue[nType];

    auto iter = mapKeyVal.find(strKey);
    if (iter == mapKeyVal.end()) {
        Write(nType, strKey, spDefault);
        return spDefault;
    } 

    QBicIniPtr spIni = QBicIniPtr(new QBicIni());

    CString strValue = iter->second;
    std::vector<CString> vecIni = QBicSplit::Split(strValue, _T('~'));
    for (size_t i=0; i<vecIni.size(); i++) 
    {
        CString strKeyValue = vecIni[i];
        std::vector<CString> vecKeyVal = QBicSplit::Split(strKeyValue, _T('|'));

        if (vecKeyVal.size() < 2) { // 1개 이하는 통과, 3개 이상이 나올수도 있다
            continue;
        }

        spIni->Write(vecKeyVal[0], vecKeyVal[1]);
    }

    bool bAdded(false); 
    {// Default 항목을 추가
        std::map<CString, CString>& mapDefKeyVal = spDefault->m_mapKeyValue;
        for (auto iter = mapDefKeyVal.begin(); iter != mapDefKeyVal.end(); ++iter) 
        {
            CString strKey = iter->first;
            std::map<CString, CString>& mapKeyVal = spIni->m_mapKeyValue;

            auto iFind = mapKeyVal.find(strKey);
            if (iFind == mapKeyVal.end()) {
                spIni->Write(strKey, iter->second);
                bAdded = true;
            }
        }
    }

    if (bAdded == true) {
        Write(nType, strKey, spIni);
    }

    return spIni;
}

CString QBicGrpIni::Read(int nType, CString strKey, CString strDefault )
{
    strKey.MakeLower();
    std::map<CString, CString>& mapKeyVal = m_mapTypeKeyValue[nType]; {
        auto iter = mapKeyVal.find(strKey);
        if (iter == mapKeyVal.end()) {
            mapKeyVal[strKey] = strDefault; 
            return strDefault;
        }
        else {
            return iter->second;
        }
    }

    return _T("");
}

int QBicGrpIni::ReadInt( int nType, CString strKey, int nDefault )
{
    CString strDefault(_T("")); {
        strDefault.Format(_T("%d"), nDefault);
    }

    return _ttoi(Read(nType, strKey, strDefault));
}

double QBicGrpIni::ReadDbl( int nType, CString strKey, double dDefault )
{
    CString strDefault(_T("")); {
        strDefault.Format(_T("%f"), dDefault);
    }

    return _ttof(Read(nType, strKey, strDefault));
}

__int64 QBicGrpIni::ReadI64( int nType, CString strKey, __int64 nxDefault )
{
    CString strDefault(_T("")); {
        strDefault.Format(_T("%I64d"), nxDefault);
    }

    return _ttoi64(Read(nType, strKey, strDefault));
}

COLORREF QBicGrpIni::ReadRGB( int nType, CString strKey, COLORREF clrDefault )
{
    COLORREF clrOut = clrDefault;

    CString strValue = Read(nType, strKey, _T(""));
    if (strValue.GetLength() == 0) {        
        Write(nType, strKey, clrDefault);
    }

    std::vector<CString> vecRGB = QBicSplit::Split(strValue, _T(','));
    if (vecRGB.size() == 3) {
        int nR = _ttoi(vecRGB[0]);
        int nG = _ttoi(vecRGB[1]);
        int nB = _ttoi(vecRGB[2]);                 
        clrOut = RGB(nR, nG, nB);
    }    

    return clrOut;
}

void QBicGrpIni::SetData( std::shared_ptr<QBicGrpIni> spFlashIni )
{
    m_mapTypeKeyValue = spFlashIni->m_mapTypeKeyValue;
}

void QBicGrpIni::SetData( std::shared_ptr<QBicGrpIni> spFlashIni, int nType )
{
    m_mapTypeKeyValue[nType] = spFlashIni->m_mapTypeKeyValue[nType];
}
