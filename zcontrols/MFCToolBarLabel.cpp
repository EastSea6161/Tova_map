// MFCToolBarLabel.cpp : implementation file
//

#include "stdafx.h"
#include "MFCToolBarLabel.h"


IMPLEMENT_SERIAL(ZMFCToolBarLabel, CMFCToolBarButton, 1)

	
///////////////////////////////////////////////////////////////////////////////
// ZMFCToolBarLabel
///////////////////////////////////////////////////////////////////////////////
/**************************************
* Constructors and Destructor         *
**************************************/
ZMFCToolBarLabel::ZMFCToolBarLabel( UINT nID, LPCTSTR lpszText )
{
	if( NULL != lpszText )
	{
		m_strText = lpszText;
	}
	m_bText = TRUE;
	m_nID	= nID;
	m_iImage	= -1;
}


ZMFCToolBarLabel::~ZMFCToolBarLabel()
{
}


/**************************************
* Overrides                           *
**************************************/
void ZMFCToolBarLabel::OnDraw( CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages,
								BOOL bHorz, BOOL /*bCustomizeMode*/, BOOL /*bHighlight*/, 
								BOOL /*bDrawBorder*/,	BOOL /*bGrayDisabledButtons*/)
{
	UINT nStyle = m_nStyle;
	m_nStyle &= ~TBBS_DISABLED;

	CMFCToolBarButton::OnDraw( pDC, rect, pImages, bHorz, FALSE, FALSE, FALSE, FALSE);

	m_nStyle = nStyle;
}
