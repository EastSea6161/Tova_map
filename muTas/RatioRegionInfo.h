#include <list>

#pragma once



struct TRatioRegionManagement
{
	int     nZoneID;
	int     nZoneCategoryCode;
	double  dItemValue; 
	double  dResultValue;
};

class KRatioRegionInfo
{
public:
	KRatioRegionInfo(void);
	~KRatioRegionInfo(void);

public:
	std::map<int, double>     m_mapCategorySum;
	std::list<TRatioRegionManagement> m_lstRatioReionManage;

protected:
	int m_nCommdityCode;
	int m_nCommdityCodeGroup;
	CString m_strCommdityName;
	CString m_nSEDCode;
	CString m_strSEDCaption;
	CString m_strRatioReionCode;

public:

	void SetCommdityCode(int a_nCommdityCode);
	void SetCommdityGroupCode(int a_nCommdityCodeGroup);
	void SetSEDCode(CString a_nSEDCode);
	void SetCommdityName(CString a_strCommdityName);
	void SetSEDCaption(CString a_strSEDCaption);
	void SetRatioReionCode(CString a_strRatioReionCode);

	int GetCommdityCode();
	int GetCommdityCodeGroup();
	CString GetSEDCode();
	CString GetCommdityName();
	CString GetSEDCaption();
	CString GetRatioReionCode();


};

