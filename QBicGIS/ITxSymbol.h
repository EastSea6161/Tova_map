#pragma once

#include "TxHatchInfo.h"

enum TxSymbolType
{
    enSymbolHide,    
    enSymbolPointType,
    enSymbolPointTypeCircle,
    enSymbolPointTypeRect,
    enSymbolPointTypeQuad,
    enSymbolPointTypeImage,
    enSymbolLineType,
    enSymbolLineDirectionType,
    enSymbolPolygonType,
    enSymbolPolygonHatchType    
};
/**
*@brief 심볼 
*@details 심볼 초기화
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ITxSymbol
{
public:
    ITxSymbol(){
        m_nDrawOrder = 1;
        m_bShow      = true;
    }
    virtual ~ITxSymbol () {}

public:
	/**
	*@brief 타입 설정 초기화
	*/
    virtual TxSymbolType GetType() = 0;
private:
    bool m_bShow;
public:
	/**
	*@brief 보이기 
	*@return 보이지 반환
	*/
    virtual bool Show() {
        return m_bShow;
    }
	/**
	*@brief 보이기
	*@param bool bShow 보이기 값
	*/
    virtual void Show(bool bShow) {
        m_bShow = bShow;
    }

protected:
    int m_nDrawOrder;
public:
	/**
	*@brief ..
	*@param int nOrder 옵션 
	*/

    // 동일 위치에 복수의 엔티티를 그릴 경우. 중복이 발생하여 보이지 않는 경우가 생김
    // 엔진에서는 DrawOrderOptionValue(m_nDrawOrder * 심볼크기)를 갖고 정렬하여 그린다.
    // 항상 최상단에 그리고 싶다면 DrawOrderOption의 값을 0으로 정의한다.
    // 항상 최하단에 그리고 싶다면 DrawOrderOption의 값을 매우 큰값으로 정의한다. 
    virtual void DrawOrderOption(int nOrder) {
        m_nDrawOrder = nOrder;
    }
	/**
	*@brief 그리기 옵션
	*@return 옵션 반환
	*/
    virtual int DrawOrderOption() {
        return m_nDrawOrder;
    }

    virtual double DrawOrderOptionValue() = 0; 
};

typedef std::shared_ptr<ITxSymbol> ITxSymbolPtr;
/**
*@brief 심볼 숨기기 설정
*@details 심볼 숨기기
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxSymbolHide : public ITxSymbol
{
public:
    TxSymbolHide() {}
    virtual ~TxSymbolHide() {}
public:
	/**
	*@brief 타입 설정 초기화
	*@return 숨기기 타입
	*/
    virtual TxSymbolType GetType() {
        return enSymbolHide;
    }
public:
	/**
	*@brief 보이기
	*@return false 반환
	*/
    virtual bool Show() {
        return false;
    }
public:
	/**
	*@brief 숨김심볼 생성
	*@return 심볼 숨김 반환
	*/
    static ITxSymbolPtr CreateHideSymbol() {
        return ITxSymbolPtr (new TxSymbolHide());
    }
public:
	/**
	*@brief 그리기 옵션
	*@return 0
	*/
    virtual double DrawOrderOptionValue() {
        return 0;
    }
};

typedef std::shared_ptr<TxSymbolHide> TxSymbolHidePtr;

