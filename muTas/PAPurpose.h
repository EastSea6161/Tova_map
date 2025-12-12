/**
 * @file PAPurpose.h
 * @brief KPAPurpose 선언 파일
 * @author 
 * @date 2011.06.09
 * @remark
 */


#pragma once
#include "model_common.h"

class KPurpose;

class KPAPurpose
{
public:
	KPAPurpose(KEMPurposeGroup emPurposeGroup, int nPurposeID, LPCTSTR strName, KEMPAType emPAType);
	KPAPurpose(const KPurpose& purpose, KEMPAType emPAType);
	KPAPurpose(const KPAPurpose& r);
	~KPAPurpose(void);

public :
	KPAPurpose& operator=(const KPAPurpose& r);
public:
	KEMPurposeGroup PurposeGroup(void);
	void PurposeGroup(KEMPurposeGroup emGroup);
public:
	int PurposeID(void);
	void PurposeID(int nObjectID);
public:
	LPCTSTR PurposeName(void);
	void PurposeName(LPCTSTR strPurposeName);
public:
	KEMPAType PAType(void);
	void PAType(KEMPAType emPAType);

protected :
	KEMPAType m_emPAType;
	KPurpose* m_pPurpose;
};
