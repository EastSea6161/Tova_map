#pragma once
#include "TxMapEnumType.h"
#include "ITxSymbol.h"
/**
*@brief 방향
*@details 방향
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxSymbolDirectionLine : public ITxSymbol
{
private:
    ITxSymbolPtr m_spForward;
    ITxSymbolPtr m_spBackward;

public:
    TxSymbolDirectionLine();
    TxSymbolDirectionLine(EnumTxDirectionFlag emFlag, ITxSymbolPtr spSymbol);
    TxSymbolDirectionLine(ITxSymbolPtr spForward,   ITxSymbolPtr spBackward);
    virtual ~TxSymbolDirectionLine();
public:
    virtual TxSymbolType GetType() {return enSymbolLineDirectionType;}
public:
	/**
	*@brief 앞 방향
	*@param ITxSymbolPtr spSymbol 
	*/
    void ForwardSymbol (ITxSymbolPtr spSymbol);
	/**
	*@brief 뒷 방향
	*@param ITxSymbolPtr spSymbol 
	*/
    void BackwardSymbol(ITxSymbolPtr spSymbol);
public:
	/**
	*@brief 앞방향
	*@return 
	*/
    ITxSymbolPtr ForwardSymbol();
	/**
	*@brief 뒷방향
	*@return 
	*/
    ITxSymbolPtr BackwardSymbol();
	/**
	*@brief 그리기 옵션
	*@return 
	*/
    virtual double DrawOrderOptionValue();
};

typedef std::shared_ptr<TxSymbolDirectionLine> TxSymbolDirectionLinePtr;