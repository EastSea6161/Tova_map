#pragma once
#include "TxMapEnumType.h"
/**
*@brief 라벨
*@details 라벨
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ITxLabel
{
public:
    ITxLabel() {
        m_bShow = true;
    }
    virtual ~ITxLabel() {}

private:
    bool m_bShow;
public:
	/**
	*@brief 라벨 표시
	*@param bool bShow 라벨 표시 유무 확인
	*/
    virtual void Show(bool bShow) {
        m_bShow = bShow;
    }
	/**
	*@brief 라벨 표시
	*@return 라벨 표시 값 
	*/
    virtual bool Show() {
        return m_bShow;
    }

public:
    virtual CString Label2Str(EnumTxDirectionFlag enFlag=EnumForward) = 0;
    virtual void Str2Label(CString _strLabel, EnumTxDirectionFlag enFlag=EnumForward) = 0;
    virtual EnumTxLabelType LabelType() = 0;
};
/**
*@brief 라벨
*@details 라벨
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxLabel : public ITxLabel
{
public:
    TxLabel(CString _strLabel) : m_strLabel(_strLabel) {}
    virtual ~TxLabel() {}
private:
    CString m_strLabel;

public:
	/**
	*@brief 
	*@param EnumTxDirectionFlag enFlag 
	*@return 
	*/
    virtual CString Label2Str(EnumTxDirectionFlag enFlag=EnumForward) {
        return m_strLabel;
    }
	/**
	*@brief 
	*@param CString _strLabel			문장
	*@param EnumTxDirectionFlag enFlag 
	*/
    virtual void Str2Label(CString _strLabel, EnumTxDirectionFlag enFlag=EnumForward){
        m_strLabel = _strLabel;
    }
	/**
	*@brief 라벨 타입
	*@return 라벨 타입
	*/
    virtual EnumTxLabelType LabelType() {
        return TxLabelType;
    }

public:
    static std::shared_ptr<TxLabel> CreateLabel(CString strLabel) {
        return std::shared_ptr<TxLabel>(new TxLabel(strLabel));
    }
};
/**
*@brief 방향 라벨
*@details 방향 라벨
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxDirectionLabel : public ITxLabel
{
public:
    TxDirectionLabel(CString _strForward, CString _strBackward) : ForwardLabel(_strForward), BackwardLabel(_strBackward) {}
    virtual ~TxDirectionLabel() {}

public:
    CString ForwardLabel;
    CString BackwardLabel;
	/**
	*@brief 
	*@param EnumTxDirectionFlag enFlag 
	*@return 
	*/
    virtual CString Label2Str(EnumTxDirectionFlag enFlag=EnumForward) {
        if (EnumForward == enFlag )
            return ForwardLabel;

        return BackwardLabel;
    }
	/**
	*@brief 
	*@param CString _strLabel			문장
	*@param EnumTxDirectionFlag enFlag 
	*/
    virtual void Str2Label(CString _strLabel, EnumTxDirectionFlag enFlag=EnumForward){
        if (EnumForward == enFlag )
        {
            ForwardLabel = _strLabel;
        }
        else
        {
            BackwardLabel = _strLabel;
        }
    }
	/**
	*@brief 라벨 타입
	*@return 라벨 타입
	*/
    virtual EnumTxLabelType LabelType() {
        return TxDirectionLabelType;
    }
};

typedef std::shared_ptr<ITxLabel> ITxLabelPtr;
typedef std::shared_ptr<TxLabel>   TxLabelPtr;

typedef std::shared_ptr<TxDirectionLabel> TxDirectionLabelPtr;