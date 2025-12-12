/**
 * @file MFCToolBarLabel.h
 * @brief ZMFCToolBarLabel 선언
 * @author 
 * @date 2010.07.03
 * @remark
 */


#pragma once

/**
 * @brief MFCToolBar에서 Static control처럼 text만 표출한다.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.07.03
 */
class AFX_EXT_CLASS ZMFCToolBarLabel : public CMFCToolBarButton
{
	DECLARE_SERIAL( ZMFCToolBarLabel )

	/**************************************
	* Constructors and Destructor         *
	**************************************/
public:
	ZMFCToolBarLabel( UINT nID = 0, LPCTSTR lpszText = NULL );
	virtual ~ZMFCToolBarLabel();

	/**************************************
	* Overrides                           *
	**************************************/
	virtual void OnDraw( CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages,
	BOOL bHorz=TRUE, BOOL bCustomizeMode=FALSE, BOOL bHighlight=FALSE, 
	BOOL bDrawBorder=TRUE, BOOL bGrayDisabledButtons=TRUE );
};
