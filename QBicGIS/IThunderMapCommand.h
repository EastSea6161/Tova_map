#pragma once
#include "IThunderMouseEvent.h"
#include "TxMapEnumType.h"
#include "IThunderMap.h"
/**
*@brief 지도 명령어
*@details 지도 명령어
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class IThunderMapCommand : public IThunderMouseEvent
{
protected:
    HWND         m_hwnd;
    IThunderMap* m_pThunderMap;
	Gdiplus::Color m_crBackground;
protected:
    IThunderMapCommand(IThunderMap* pThunderMap, EnumTxMapCommand enThunderMapCommand, Gdiplus::Color crBackground = Gdiplus::Color::White) 
		: m_pThunderMap(pThunderMap), m_enumThunderMapCommand(enThunderMapCommand), m_crBackground(crBackground)
    {
        m_hwnd = pThunderMap->GetHwnd();
    }

    virtual ~IThunderMapCommand(){};

protected:
    EnumTxMapCommand m_enumThunderMapCommand;
public:
	/**
	*@brief 지도 제어 명령어
	*@return 지도 제어 명령어
	*/
    EnumTxMapCommand ThunderMapCommand()
    {
        return m_enumThunderMapCommand;
    }
	/**
	*@brief 지도 제어 명령어
	*@param EnumTxMapCommand enThunderMapCommand
	*/
    void ThunderMapCommand(EnumTxMapCommand enThunderMapCommand)
    {
        m_enumThunderMapCommand = enThunderMapCommand;
    }
public:
    virtual bool IsCommandRun() = 0;

	/**
	*@brief 지도 배경색 설정, 2019.03.25 추가
	*@param Gdiplus::Color crBackground : 지도 배경색 
	*/
	void SetBackgroundColor(Gdiplus::Color crBackground)
	{
		m_crBackground = crBackground;
	}

protected:
	/**
	*@brief 라벨 디버그
	*@param Gdiplus::Graphics& graphics	그래프
	*@param Gdiplus::PointF pointF		포인터
	*@param CString strDebugLabel		
	*/
    void DebugLabel(Gdiplus::Graphics& graphics, Gdiplus::PointF pointF, CString strDebugLabel);
};


typedef std::shared_ptr<IThunderMapCommand> IThunderMapCommandPtr;
