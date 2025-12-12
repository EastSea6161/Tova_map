#pragma once

#include "IOTableView.h"

class KIOTableRecordSelection;

class KIOTerminalScheduleTableView : public KIOTableView
{
	DECLARE_DYNCREATE(KIOTerminalScheduleTableView)

public:
	KIOTerminalScheduleTableView(void);
	~KIOTerminalScheduleTableView(void);

public :
	virtual void OnDraw(CDC* pDC);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	virtual void RegisterMapView(KMapView* a_pMapView);
	virtual void UpdateRecord(void);

protected:
	KIOTableRecordSelection* m_pTerminalScheduleSelection;

protected:
	DECLARE_MESSAGE_MAP();

private:
    afx_msg void OnClose();
	afx_msg void OnReportRowRButtonClick (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReportCheckItem(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnSelectAllTerminalSchedule(void);
	afx_msg void OnDeSelectAllTerminalSchedule(void);


	afx_msg void OnOriginTerminalZoomTo(void);
	afx_msg void OnDestTerminalZoomTo(void);
	afx_msg void OnTerminalExtentTo(void);

	void DrawThemes(void);
};

