#pragma once

/*forward declare*/
class KPurpose;
class KIOColumn;

class KInterInputInfo
{
public:
	KInterInputInfo(void);
	KInterInputInfo(const KInterInputInfo& r);
	~KInterInputInfo(void);

public:
	KInterInputInfo& operator=(const KInterInputInfo& r);
		
	bool 	  GetSelected(void);
	void 	  SetSelected(bool bSelected);

	KPurpose* GetPurpose(void);
	void 	  SetPurpose(KPurpose* a_pPurpose);
	void 	  CopyPurpose(const KPurpose* a_pPurpose);

	KIOColumn* InputColumn(void);
	void       InputColumn(KIOColumn* a_pInputColumn);

private:
	bool       m_bSelected;
	KPurpose*  m_pPurpose;
	KIOColumn* m_pInputColumn;
};

