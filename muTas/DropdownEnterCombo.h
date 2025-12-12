#pragma once


// KDropdownEnterCombo

class KDropdownEnterCombo : public CComboBox
{
	DECLARE_DYNAMIC(KDropdownEnterCombo)

public:
	KDropdownEnterCombo();
	virtual ~KDropdownEnterCombo();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


