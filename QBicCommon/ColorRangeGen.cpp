/**
 * @file ColorRangeGen.cpp
 * @brief KColorRangeGen 구현파일
 * @author 
 * @date 2011.02.21
 * @remark
 */

#include "StdAfx.h"
#include "ColorRangeGen.h"

KColorRangeGen::KColorRangeGen(void)
{
}

KColorRangeGen::~KColorRangeGen(void)
{
}


void KColorRangeGen::setStartColor(COLORREF clrStart)
{
	m_clrStart = clrStart;
}

void KColorRangeGen::setEndColor(COLORREF clrEnd)
{
	m_clrEnd = clrEnd;
}

void KColorRangeGen::setSize(int nSize)
{
	m_nSize = nSize;
	if(m_nSize <= 1)
	{
		m_nSize = 2;
	}
}

bool KColorRangeGen::createColorRange(void)
{
	m_vecColor.clear();
	m_nCurrentIndex = 0;

	try 
	{
		int nSr = GetRValue(m_clrStart);
		int nSg = GetGValue(m_clrStart);
		int nSb = GetBValue(m_clrStart);

		int nEr = GetRValue(m_clrEnd);
		int nEg = GetGValue(m_clrEnd);
		int nEb = GetBValue(m_clrEnd);

		unsigned char r, g, b;
		for (int i= 0; i< m_nSize; i++) {
			float n = (float)i / (float)(m_nSize-1);
			r = (unsigned char)((float)nSr * (1.0f-n) + (float)nEr * n);
			g = (unsigned char)((float)nSg * (1.0f-n) + (float)nEg * n);
			b = (unsigned char)((float)nSb * (1.0f-n) + (float)nEb * n);

			m_vecColor.push_back(COLORREF(RGB(r, g, b)));
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		return false;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		return false;
	} catch (...) {
		TxLogDebugException();
		return false;
	}

	return true;

//  return false;
// 	m_spColorRamp.CreateInstance(CLSID_AlgorithmicColorRamp);
// 	
// 	IColorPtr spStartColor, spEndColor;
// 	
// 	spStartColor.CreateInstance(CLSID_RgbColor);
// 	spStartColor->put_RGB(m_clrStart);
// 	m_spColorRamp->put_FromColor(spStartColor);
// 
// 	spEndColor.CreateInstance(CLSID_RgbColor);
// 	spEndColor->put_RGB(m_clrEnd);
// 	m_spColorRamp->put_ToColor(spEndColor);
// 
// 	m_spColorRamp->put_Size(m_nSize);
// 
// 	VARIANT_BOOL bResult = VARIANT_FALSE;
// 	m_spColorRamp->CreateRamp(&bResult);
// 	m_nCurrentIndex = 0;
// 
// 	return (VARIANT_TRUE == bResult);
}

COLORREF KColorRangeGen::getNextColor(void)
{
	if(m_nCurrentIndex == m_nSize)
	{
		m_nCurrentIndex = 0;
	}

	return m_vecColor[m_nCurrentIndex++];

//  return RGB(0,0,0);
// 	if(m_nCurrentIndex == m_nSize)
// 	{
// 		m_nCurrentIndex = 0;
// 	}
// 
// 	IColorPtr spColor;
// 
// 	m_spColorRamp->get_Color(m_nCurrentIndex++, &spColor);
// 
// 	OLE_COLOR ocColor;
// 	spColor->get_RGB(&ocColor);
// 
// 	COLORREF clrReturn;
// 	::OleTranslateColor(ocColor, NULL, &clrReturn);
// 	return clrReturn;
}
