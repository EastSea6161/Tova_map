#pragma once

class KIOColumn;

class KRoadTimeCostInfo
{
public:
	KRoadTimeCostInfo(void);
	~KRoadTimeCostInfo(void);

public:
	void Mode(int a_nModeName);
	int  Mode(void);
	void Type(int a_nType);
	int  Type(void);
	void       UseFunction(bool a_bSelected);
	bool       UseFunction(void);
	void       UpLoadValue(double a_dUpLaodValue);
	double     UpLoadValue(void);
	void       DownLoadValue(double a_dDownlaodValue);
	double     DownLoadValue(void);
	void       TripValue(double a_dTripTime);
	double     TripValue(void);
	void       OriginRoadColumn(KIOColumn* a_pKIoColumn);
	KIOColumn* OriginRoadColumn(void);
	void       DestinationRoadColumn(KIOColumn* a_pKIOColumn);
	KIOColumn* DestinationRoadColumn(void);
	void       OutPutColumn(KIOColumn* a_pKIOColumn);
	KIOColumn* OutPutColumn(void);
	CString    OriginRoadColumnName();
	CString    DestinationRoadColumnName();
	CString    OutPutColumnName();

	bool CheckUsingColumn(CString& a_strUnitName);

protected:
	int	   m_nMode;
	int	   m_nType;
	bool   m_bSelected;
	double m_dUpLoadValue;
	double m_dDownLoadValue;
	double m_dTripValue;
	CString m_strOriginRoadColumnName;
	CString m_strDestinationRoadColumnName;
	CString m_strOutPutColumnName;
	KIOColumn* m_pOriginRoadColumn;
	KIOColumn* m_pDestiantionRoadColumn;
	KIOColumn* m_pOutPutColumn;

};

