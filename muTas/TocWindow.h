#pragma once

#include "officeTreeCtrl.h"

class KProject;

class KTocWindow : public CWnd
{
	DECLARE_DYNAMIC(KTocWindow)

public:
	KTocWindow(void);
	~KTocWindow(void);

public:

private:

protected:
	COfficeTreeCtrl m_wndTreeCtrl;
	CImageList      m_imgList;


protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
};

