#include "stdafx.h"
#include "ImChampMapDefine.h"
#include "DBaseConnector.h"
//^#include "ParseString.h"

double   ImChampMapDefineLabel::DefaultLabelSize     = 9.0;
COLORREF ImChampMapDefineLabel::DefaultLabelColor    = RGB(0, 0, 0);
CString  ImChampMapDefineLabel::DefaultLabelFontName = _T("Tahoma");

double   ImChampMapDefineLabel::NodeLabelSize     = 9.0;
COLORREF ImChampMapDefineLabel::NodeLabelColor    = RGB(0, 0, 0);
CString  ImChampMapDefineLabel::NodeLabelFontName = _T("Tahoma");

double   ImChampMapDefineLabel::LinkLabelSize     = 9.0;
COLORREF ImChampMapDefineLabel::LinkLabelColor    = RGB(28, 28, 28);
CString  ImChampMapDefineLabel::LinkLabelFontName = _T("Tahoma");

TPreferenceSymbol ImChampMapDefineSymbol::DefaultSymbol;
TPreferenceSymbol ImChampMapDefineSymbol::NodeSymbol;
TPreferenceSymbol ImChampMapDefineSymbol::ZoneSymbol;

void ImChampMapDefineLabel::LoadPreferenceLabel()
{
    try
    {
        CString strSQL(_T(""));
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("   LabelType, LabelSize, LabelTextColor, LabelFontName "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T(" Preference_Label "));

        KDBaseConPtr spDBaseConnection = KDBase::GetApplicationDBConnection();
        if (spDBaseConnection == NULL)
        {		
            AfxMessageBox(_T("TOVA System File(tova) Not Found"));
            return;
        }

        KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while(spResultSet->Next())
        {
            int     nLabelType  = spResultSet->GetValueInt   (0);
            double  dLabelSize  = spResultSet->GetValueDouble(1);
            CString strRGB      = spResultSet->GetValueString(2);
            CString strFontName = spResultSet->GetValueString(3);

            std::vector<CString> vecRGB = KParseString::ParseString(strRGB,  _T(','));
            int nRValue(0);
            int nGValue(0);
            int nBValue(0);

            if (vecRGB.size() == 3)
            {
                nRValue  =  _ttoi(vecRGB[0]);
                nGValue  =  _ttoi(vecRGB[1]);
                nBValue  =  _ttoi(vecRGB[2]);
            }

            switch(nLabelType)
            {
            case 0:
                DefaultLabelSize     = dLabelSize;
                DefaultLabelColor    = RGB(nRValue, nGValue, nBValue);
                DefaultLabelFontName = strFontName;
                break;
            case 100:
                NodeLabelSize     = dLabelSize;
                NodeLabelColor    = RGB(nRValue, nGValue, nBValue);
                NodeLabelFontName = strFontName;
                break;
            case 200:
                LinkLabelSize     = dLabelSize;
                LinkLabelColor    = RGB(nRValue, nGValue, nBValue);
                LinkLabelFontName = strFontName;
                break;
            default:
                break;
            }
        }
    }
    catch (KExceptionPtr ex)
    {   
    	TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }    
}

void ImChampMapDefineLabel::SavePreferenceLabel()
{
    SavePreferenceLabel(100, NodeLabelSize, NodeLabelColor, NodeLabelFontName);
    SavePreferenceLabel(200, LinkLabelSize, LinkLabelColor, LinkLabelFontName);
}

void ImChampMapDefineLabel::SavePreferenceLabel( int a_nLabelType, double a_dLabelSize, COLORREF a_rgbLabelColor, CString a_strLabelFontName )
{
    try
    {
        KDBaseConPtr spDBaseConnection = KDBase::GetApplicationDBConnection();
        if (spDBaseConnection == NULL)
        {		
            AfxMessageBox(_T("TOVA System File(tova) Not Found"));
            return;
        }

        CString strRGB(_T(""));
        strRGB.Format(_T("%d,%d,%d"), GetRValue(a_rgbLabelColor), GetGValue(a_rgbLabelColor), GetBValue(a_rgbLabelColor));

        CString strUpdate(_T(""));
        strUpdate.AppendFormat(_T(" Update Preference_Label Set LabelSize ='%.f', LabelTextColor='%s', LabelFontName = '%s' " ), a_dLabelSize, strRGB, a_strLabelFontName);
        strUpdate.AppendFormat(_T(" Where LabelType = '%d' "), a_nLabelType);

        spDBaseConnection->ExecuteUpdate(strUpdate);
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }    
}


void ImChampMapDefineSymbol::LoadPreferenceSymbol()
{
	try
	{
		DefaultSymbol.nCharIndex = 74;
		DefaultSymbol.dSize      = 15;
		DefaultSymbol.clrSymbol  = RGB(0,204,255);
		
		NodeSymbol = DefaultSymbol;// Load 실패 대비 초기화
		ZoneSymbol = DefaultSymbol;

		CString strSQL(_T(""));
		strSQL.Append(_T(" SELECT "));
		strSQL.Append(_T("   SymbolType, CharIndex, Size, ColorRGB "));
		strSQL.Append(_T(" FROM "));
		strSQL.Append(_T(" Preference_Symbol "));

		KDBaseConPtr spDBaseConnection = KDBase::GetApplicationDBConnection();
		if (spDBaseConnection == NULL)
		{		
			AfxMessageBox(_T("TOVA System File(tova) Not Found"));
			return;
		}

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultSet->Next())
		{
			int     nSymbolType  = spResultSet->GetValueInt   (0);
			int     nCharIndex   = spResultSet->GetValueInt   (1);
			double  dSymbolSize  = spResultSet->GetValueDouble(2);
			CString strRGB       = spResultSet->GetValueString (3);

			std::vector<CString> vecRGB = KParseString::ParseString(strRGB,  _T(','));
			int nRValue(0);
			int nGValue(0);
			int nBValue(0);

			if (vecRGB.size() == 3)
			{
				nRValue  =  _ttoi(vecRGB[0]);
				nGValue  =  _ttoi(vecRGB[1]);
				nBValue  =  _ttoi(vecRGB[2]);
			}

			TPreferenceSymbol oTempSymbol;
			oTempSymbol.nCharIndex = nCharIndex;
			oTempSymbol.dSize      = dSymbolSize;
			oTempSymbol.clrSymbol  = RGB(nRValue,  nGValue, nBValue);

			switch(nSymbolType)
			{
			case 0:
				DefaultSymbol      = oTempSymbol;
				break;
			case 100:
				NodeSymbol         = oTempSymbol;
				break;
			case 200:
				ZoneSymbol         = oTempSymbol;
				break;
			default:
				break;
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}    
}


void ImChampMapDefineSymbol::SavePreferenceSymbol()
{
	SavePreferenceSymbol(100, NodeSymbol);
	SavePreferenceSymbol(200, ZoneSymbol);
}


void ImChampMapDefineSymbol::SavePreferenceSymbol(int a_nSymbolType, TPreferenceSymbol &a_oTPreferenceSymbol)
{
	try
	{
		KDBaseConPtr spDBaseConnection = KDBase::GetApplicationDBConnection();
		if (spDBaseConnection == NULL)
		{		
			AfxMessageBox(_T("TOVA System File(tova) Not Found"));
			return;
		}

		CString   strRGB(_T(""));
		COLORREF &clrSymbol = a_oTPreferenceSymbol.clrSymbol;
		strRGB.Format(_T("%d,%d,%d"), GetRValue(clrSymbol), GetGValue(clrSymbol), GetBValue(clrSymbol));

		CString strUpdate(_T(""));
		strUpdate.AppendFormat(_T(" Update Preference_Symbol Set CharIndex = %d, Size ='%.f', ColorRGB='%s' " ), a_oTPreferenceSymbol.nCharIndex, a_oTPreferenceSymbol.dSize, strRGB);
		strUpdate.AppendFormat(_T(" Where SymbolType = '%d' "), a_nSymbolType);

		spDBaseConnection->ExecuteUpdate(strUpdate);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}    
}
