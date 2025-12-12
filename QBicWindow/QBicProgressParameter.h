#pragma once
#include "QBicSimpleProgress.h"
#include "QBicListSimpleProgress.h"
/**
*@brief 심플 프로그레스 파라미터
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicSimpleProgressParameter
{
private:
    QBicSimpleProgressParameter(QBicSimpleProgress* a_pSimpleProgress, void* a_pParameter)
    {
        m_pProgress  = a_pSimpleProgress;
        m_pParameter = a_pParameter;
    }

    ~QBicSimpleProgressParameter()
    {
    }

private:
    QBicSimpleProgress* m_pProgress;
    void*               m_pParameter;

public:
	/**
	*@brief 프로그레스 윈도우
	*@return 프로그레스 반환
	*/
    QBicSimpleProgress* GetProgressWindow()
    {
        return m_pProgress;
    }
	/**
	*@brief 파라미터 추출
	*/
    void* GetParameter()
    {
        return m_pParameter;
    }

    friend class QBicSimpleProgress;
};
/**
*@brief 심플 프로그레스 파라미터 리스트
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicListSimpleProgressParameter
{
private:
    QBicListSimpleProgressParameter(QBicListSimpleProgress* a_pSimpleProgress, void* a_pParameter)
    {
        m_pProgress  = a_pSimpleProgress;
        m_pParameter = a_pParameter;
    }

    ~QBicListSimpleProgressParameter()
    {
    }

private:
    QBicListSimpleProgress* m_pProgress;
    void*                   m_pParameter;

public:
	/**
	*@brief 프로그레스 윈도우
	*@return 프로그레스 반환
	*/
    QBicListSimpleProgress* GetProgressWindow()
    {
        return m_pProgress;
    }
	/**
	*@brief 파라미터 추출
	*/
    void* GetParameter()
    {
        return m_pParameter;
    }

    friend class QBicListSimpleProgress;
};