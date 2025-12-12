#pragma once

class KTableSubject;
/**
*@brief 테이블 관측
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KTableObserver 
{
public:
	KTableObserver(void) {};
	virtual ~KTableObserver(void) {};
public :
	/**
	*@brief 프로세스 담당
	*@details LPCTSTR a_strSubjectName	제목 명
	*@details __int64 a_nxObjectID		오브젝트 ID
	*/
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), __int64 a_nxObjectID=0) = 0;	
};
