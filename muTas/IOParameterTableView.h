#pragma once

#include "IOTableView.h"

class KIOParameterTableView : public KIOTableView
{
	DECLARE_DYNCREATE(KIOParameterTableView)

protected:
	KIOParameterTableView(void);
	~KIOParameterTableView(void);

protected :
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTableviewAddField();
	afx_msg void OnTableViewCalculatedPathLength();
	afx_msg void OnTableViewCalculateTimeCost();
};

