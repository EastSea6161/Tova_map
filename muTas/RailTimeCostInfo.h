#pragma once

class KIOColumn;

class KRailTimeCostInfo
{
public:
	KRailTimeCostInfo(void);
	~KRailTimeCostInfo(void);


public:

	void Mode(int a_nModeName);
	int  Mode(void);

	void Type(int a_nType);
	int  Type(void);

	void UseFunction(bool a_bSelected);
	bool UseFunction(void);

	void UpLoadValue(double a_dUpLaodValue);
	double UpLoadValue(void);

	void DownLoadValue(double a_dDownLoadValue);
	double DownLoadValue(void);

	void RoadTripValue(double a_dRoadTripValue);
	double RoadTripValue(void);

	void RailTripValue(double a_dRailTripValue);
	double RailTripValue(void);

	void FacilityValue(double a_dFacilityValue);
	double FacilityValue(void);

	void OriginRoadColumn(KIOColumn* a_pKIOColumn);
	KIOColumn* OriginRoadColumn(void);

	void DestinationRoadColumn(KIOColumn* a_pKIOColumn);
	KIOColumn* DestinationRoadColumn(void);

	void RailColumn(KIOColumn* a_pKIOColumn);
	KIOColumn* RailColumn(void);

	void OutPutColumn(KIOColumn* a_pKIOColumn);
	KIOColumn* OutPutColumn(void);

	CString OriginRoadColumnName();
	CString DestinationRoadColumName();
	CString RailColumnName();
	CString OutPutColumnName();

	bool CheckUsingColumn(CString& a_strUnitName);

protected:

	bool m_bSelected;

	int  m_nMode;
	int  m_nType;

	double m_dUpLoadValue;
	double m_dDownLoadValue;
	double m_dRoadTripValue;
	double m_dRailTripValue;
	double m_dFacilityValue;

	CString m_strOriginRoadName;
	CString m_strDestinationRoadName;
	CString m_strRailName;
	CString m_strOutPutName;

	KIOColumn* m_pOriginRoadColumn;
	KIOColumn* m_pDestionationRoadColumn;
	KIOColumn* m_pRailColumn;
	KIOColumn* m_pOutputColumn;
};

