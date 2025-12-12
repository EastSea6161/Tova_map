#pragma once

#include "DBaseAssignment.h"
#include "ARunningOptionInfo.h"

class KTarget;
class KAInputInfo;
class KOccupancyPCEMode;

class KMultiAssignInfo
{
public:
	KMultiAssignInfo(KTarget* a_pTarget, bool a_bIsBase = false);
	virtual ~KMultiAssignInfo(void);

public:
	std::list<KAInputInfo*> GetInputInfo(void);
	
	inline KTarget* Target(void)
	{
		return m_pTarget;
	}

	inline void InputList(std::list<KAInputInfo*> a_InputList)
	{
		m_InputList = a_InputList;
	}

protected:
	void InitInputInfoList(void);
	void ClearInputInfoList(void);

public:
	bool               m_bUseOverWrite;
	TAssignResultInfo  m_TOverWirteAResultInfo;
	
protected:
	KTarget* m_pTarget;

protected:
	bool m_bIsBase;	//Assignment에서 선택한 Target이면 True 나머지는 False, Assignmnet에서 선택한 경우 "std::list<KAInputInfo*> m_InputList"를 Clear 하지 않는다(AssignmentDlg에서 삭제하므로)
	
	// 2019.04.15 선택 가능여부(오류 여부)
	bool m_bSelected;
	bool m_bIsError;		// 해당 Target에 오류 여부(데이터 누락, 네트워크 오류 등)
	CString m_strStatus;	// 오류에 대한 설명

public:
	bool IsBase();
	void SetSelected(bool bSelected);
	bool GetSelected();
	void SetError(bool bError);
	bool GetError();
	void SetStatusString(CString& str);
	CString GetStatusString();

protected:
	std::list<KAInputInfo*> m_InputList;
	std::map<int, std::vector<KOccupancyPCEMode>> m_mapDefaultOccupancyPCEMode;
	std::map<int, std::vector<CString>>           m_mapModeChocieColumns;
	std::map<int, std::vector<KOccupancyPCEMode>> m_mapGetDefaultModeInfo;

};



/**
* @brief 도로통행배정 모형 중 SOBA의 여러 시나리오 반복 실행에 필요한 정보 클래스
*        KMultiAssignInfo 클래스를 상속하여 추가적으로  KARunningOptionInfo 정보를 포함함
* @remark
* @warning
* @version 1.0
* @author
* @date 2019.04.22
*/
class KMultiAssignInfoSOBA : public KMultiAssignInfo
{
public:
	KMultiAssignInfoSOBA(KTarget* a_pTarget, bool a_bIsBase = false);
	~KMultiAssignInfoSOBA(void);

public:
	KARunningOptionInfo& GetRunningOption();
	void SetRunningOption(KARunningOptionInfo& ro);

protected:
	KARunningOptionInfo m_runnginOption;

};