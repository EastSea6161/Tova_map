#pragma once
#include "ITxLayer.h"
#include "ITxImageLayer.h"
#include "TxCoordinateConvert.h"
#include "TxTmsServer.h"
#include "TxTmsNaver.h"
/**
*@brief 이미지 레이어(TMS)
*@details 이미지레이어
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/

class AFX_EXT_CLASS TxImageTmsLayer : public ITxImageLayer
{
public:
    TxImageTmsLayer (TxLayerInfoPtr spLayerInfo, ITxTmsServerPtr spTmsServer = nullptr, int nCoodType = 0, bool bGray=false, int nType=0); 
    virtual ~TxImageTmsLayer() {};

protected:
    virtual void Clear() { 
        m_spTmsServer = nullptr; 
    }
private:
    void WriteImage(Gdiplus::Bitmap* pBitmap);
private:
    HRESULT GetGdiplusEncoderClsid(__in LPCWSTR pwszFormat, __out GUID *pGUID);
private:
    TxCoordinateConvert m_oCoorConvert;
    ITxTmsServerPtr     m_spTmsServer;

public:
    virtual TxGdiplusBitmapPtr GetImage(ThunderMapDispPtr spThunderMapDisp);
private:
    TxGdiplusBitmapPtr GetImageLeftTop(ThunderMapDispPtr spThunderMapDisp);
    TxGdiplusBitmapPtr GetImageLeftBottom(ThunderMapDispPtr spThunderMapDisp);
    virtual void SetTmsServer(ITxTmsServerPtr spTmsServer) { 
        m_spTmsServer = spTmsServer; 
    }

private:
    int m_nIndex;
	/**
	*@brief 쓰레드 프로세스 다운로드
	*@param void *pParam
	*@return 1 반환 
	*/
    static unsigned __stdcall DownloadThreadProcess(void *pParam);

    class TParam
    {
    public:
        TParam() {
            TID   = -1;
            ::ZeroMemory(TServerPath, sizeof(TServerPath));
            ::ZeroMemory(TLocalPath,  sizeof(TLocalPath));
        }
        ~TParam(){}
        int   TID;
        TCHAR TServerPath[512];
        TCHAR TLocalPath [512];
    };

private:
    int m_nDebug;
};