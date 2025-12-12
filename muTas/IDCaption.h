#pragma once


struct TIDZoneCaption
{
	Integer TnxZoneID;
	CString TstrZoneName;
	CString TstrMainCode;
	CString TstrSubCode;
};


class KIDCaption
{
public:
    KIDCaption(CString a_strID=_T(""), CString a_strCaption=_T("")):TID(a_strID), TCaption(a_strCaption) 
    {
    }

    KIDCaption(Integer a_nxID, CString a_strCaption=_T("")): TID(_T("")), TCaption(a_strCaption) 
    {
        TID.Format(_T("%I64d"), a_nxID);
    }

    ~KIDCaption(){}

    CString TID;
    CString TCaption;
};

class KID2ColumnCaption
{
public:
	KID2ColumnCaption(CString a_strID=_T(""), CString a_strCaption=_T(""), CString a_strCaption2=_T(""))
		:TID(a_strID), TCaption(a_strCaption),TCaption2(a_strCaption2) 
	{
	}
	
	KID2ColumnCaption(Integer a_nxID, int a_nCapton2, CString a_strCaption= _T(""))
		: TID(_T("")), TCaption(a_strCaption) , TCaption2(_T(""))
	{
		TID.Format(_T("%I64d"), a_nxID);
		TCaption2.Format(_T("%d"), a_nCapton2);
	}

	~KID2ColumnCaption(){}

	CString TID;
	CString TCaption;
	CString TCaption2;
};