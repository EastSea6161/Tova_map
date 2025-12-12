#pragma once
#include "TxEnvelope.h"
#include "TxPoint.h"
/**
*@brief 지도 표출
*@details 지도 표출
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class ThunderMapEnvelopeStatck
{
public:
    ThunderMapEnvelopeStatck() {m_nIndex = 0;}
    virtual ~ThunderMapEnvelopeStatck() {}

private:
    size_t m_nIndex;
protected:
    std::vector<TxEnvelope> m_vecEnvelope;
private:
	/**
	*@brief 
	*@param const TxEnvelope& oEnvelope
	*/
    void PushEnvelope(const TxEnvelope& oEnvelope)
    {
        size_t nxCount = m_vecEnvelope.size();
        if (nxCount == 0)
        {
            m_vecEnvelope.push_back(oEnvelope);
            m_nIndex = m_vecEnvelope.size() - 1;
            return;
        }
        
        if (m_nIndex != nxCount-1)
        {// 이전/이후보기 상태에서 
            std::vector<TxEnvelope> vecEnvelope;
            for (size_t i=0; i<m_nIndex; i++)
                vecEnvelope.push_back(m_vecEnvelope[i]);

            m_vecEnvelope = vecEnvelope;
        }

        m_vecEnvelope.push_back(oEnvelope);
        m_nIndex = m_vecEnvelope.size() - 1;
    }
private:
	/**
	*@brief 원상복귀 가능 여부
	*@return 
	*/
    bool CanUndo() {
        size_t nxCount = m_vecEnvelope.size();
        if (nxCount <= 1 || m_nIndex < 1)
            return false;
        
        return true;
    }
	/**
	*@brief 새로고침 가능 여부
	*@return 
	*/
    bool CanRedo() {
        size_t nxCount = m_vecEnvelope.size();
        if (nxCount <= 1 || m_nIndex >= nxCount-1)
            return false;

        return true;
    }
private:
	/**
	*@brief 
	*@param TxEnvelope& oEnvelope
	*@return 
	*/
    bool Undo(TxEnvelope& oEnvelope) {
		if (CanUndo() == false) return false;

        m_nIndex--;
        oEnvelope = m_vecEnvelope[m_nIndex];
        return true;
    }
	/**
	*@brief 새로고침
	*@param TxEnvelope& oEnvelope
	*@return 
	*/
    bool Redo(TxEnvelope& oEnvelope) {
        if (CanRedo() == false) return false;

        m_nIndex++;
        oEnvelope = m_vecEnvelope[m_nIndex];  

        return true;
    }

    friend class ThunderMapDisp;
};

typedef std::shared_ptr<ThunderMapEnvelopeStatck> ThunderMapEnvelopeStatckPtr;
/**
*@brief 맵 표출
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class ThunderMapDisp
{
public:
    ThunderMapDisp(double dMonitorPixelDist);
    virtual ~ThunderMapDisp();
public:
	/**
	*@brief 
	*/
    void DrawEnvelopeAdjust();
    void SetMapRect   (const TxEnvelope& oMapEnvelope, bool bStack=true);
	/**
	*@brief 사각 화면 설정
	*@param const TxEnvelope& oScreenEnvelope
	*/
    void SetScreenRect(const TxEnvelope& oScreenEnvelope);

private:
    ThunderMapEnvelopeStatckPtr m_spThunderMapEnvelopeStatck;
public:
	/**
	*@brief 이전/이후 보기
	*@param const TxEnvelope& oMapEnvelope
	*/
    void PushEnvelopeStack(const TxEnvelope& oMapEnvelope) { // 이전,이후보기
        m_spThunderMapEnvelopeStatck->PushEnvelope(oMapEnvelope);
    }
public:
	/**
	*@brief 원상복귀 가능 여부
	*@return 
	*/
    bool CanUndo() {return m_spThunderMapEnvelopeStatck->CanUndo();}
	/**
	*@brief 새로고침 가능 여부
	*@return 
	*/
    bool CanRedo() {return m_spThunderMapEnvelopeStatck->CanRedo();}
public:
	/**
	*@brief 원상복귀
	*@return 
	*/
    bool Undo();
	/**
	*@brief 새로고침
	*@return 
	*/
    bool Redo();

private:
    TxEnvelope m_oMapEnvelope;
    TxEnvelope m_oScreenEnvelope;

    TxEnvelope m_oDrawMapEnvelope;
public:
	/**
	*@brief 지도 범위
	*@return 범위 반환
	*/
    TxEnvelope MapEnvelope();
	/**
	*@brief 화면 범위
	*@return 화면 범위 반환
	*/
    TxEnvelope ScreenEnvelope() {
        return m_oScreenEnvelope;
    }
private:
	double     m_dMonitorPixelDist; // 모니터의 픽셀의 실제 길이 값
    double     m_dMapPixelDist;
	double     m_dMapScale;
public:
	/**
	*@brief 범위
	*@return 규모 반환
	*/
    double GetScale();
    void   SetScale(double dScale);
public:
    TxEnvelope GetScaleEnvelope(double dScale);
public:
	/**
	*@brief 픽셀
	*@return 픽셀값 반환
	*/
    double GetPixelToDist();
	/**
	*@brief 버텍스부터의 거리
	*@return 값 반환
	*/
    double GetPointToDist();
	/**
	*@brief 1픽셀의 거리
	*@param double dDist
	*@return 
	*/
    double GetDistToPoint(double dDist);
	/**
	*@brief 1픽셀의 거리
	*@param double dDist
	*@return 
	*/
    double GetDistToPixel(double dDist);
public:
	/**
	*@brief 좌표 위치
	*@param double x
	*@param double y
	*@return 
	*/
    TxPoint ScreenToMap(double x, double y);
	/**
	*@brief 좌표 위치
	*@param long x
	*@param long y
	*@return 
	*/
    TxPoint ScreenToMap(long x, long y);
	/**
	*@brief 좌표 위치
	*@param int x
	*@param int y
	*@return 
	*/
    TxPoint ScreenToMap(int  x, int  y);
	/**
	*@brief 좌표 위치
	*@param const TxPoint& oPt
	*@return 
	*/
    TxPoint ScreenToMap(const TxPoint& oPt);
	/**
	*@brief 좌표 위치
	*@param const TxPoint& oPt
	*@return 
	*/
    TxPoint MapToScreen(const TxPoint& oPt);
	/**
	*@brief 좌표 위치
	*@param TxPoint* pPoint
	*@return 
	*/
    TxPoint MapToScreen(TxPoint*   pPoint);
	/**
	*@brief 좌표 위치
	*@param TxPointPtr spPoint
	*@return 
	*/
    TxPoint MapToScreen(TxPointPtr spPoint);
    TxPoint MapToScreen(double X, double Y);
    
	//D2D1_POINT_2F MapToScreenDx2F(const TxPoint& oPt);
    //D2D1_POINT_2F MapToScreenDx2F(TxPoint* pPoint);
public:
	/**
	*@brief 좌표 위치
	*@param const CPoint& oPt
	*@return 
	*/
    TxPoint ScreenToMap(const CPoint& oPt);
public:
    std::shared_ptr<ThunderMapDisp> Copy();
};

typedef std::shared_ptr<ThunderMapDisp> ThunderMapDispPtr;