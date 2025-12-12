#pragma once
#include "ITxGeometry.h"
#include "TxPoint.h"
class TxPolygon;
/**
*@brief 폴리라인
*@details Tx 폴리라인
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxPolyline : public ITxGeometry
{
public:
    TxPolyline();
    TxPolyline(TxPoint oFrom, TxPoint oTo);
    TxPolyline(TxPointPtr spFrom, TxPointPtr spTo);

    TxPolyline(const std::vector<TxPoint>& vecPoint);
    TxPolyline(const std::vector<TxPointPtr>& vecPoint);
    TxPolyline(const TxPolyline& right);
    TxPolyline(std::shared_ptr<TxPolyline> spPLine);
    TxPolyline(QBicBinary& oBinary, size_t nIndex=0);
    virtual ~TxPolyline();
public:
    TxPolyline& operator =  (const TxPolyline& right);
private:
	/**
	*@brief 범위 초기화
	*/
    void ResetEnvelope();
public:
	/**
	*@brief 버텍스 삽입
	*@return 버텍스 벡터 반환
	*/
    std::vector<TxPoint> GetPoints() const;
	/**
	*@brief 버텍스 삽입
	*@param double dMinVertexDist
	*@return 버텍스 벡터 반환
	*/
    std::vector<TxPoint> GetPoints(double dMinVertexDist) const;
	/**
	*@brief 버텍스 교차
	*@param double dOffset
	*@return 
	*/
    std::vector<TxPoint> GetOffsetPoint(double dOffset) const;
	/**
	*@brief 버텍스 교차
	*@param double dOffset
	*@param double dMinDist
	*@return
	*/
    std::vector<TxPoint> GetOffsetPoint(double dOffset, double dMinDist) const;
private:
    double*  m_pPointsX;
    double*  m_pPointsY;
    size_t   m_nxPointCnt;

private:
    TxEnvelope m_oEnvelope;
public:
	/**
	*@brief 범위
	*@return 범위 확인
	*/
    virtual TxEnvelope Envelope() const;
public:
	/**
	*@brief 표출
	*@return PolyLIne 관련 정보 출력
	*/
    virtual CString ToString();
public:
	/**
	*@brief 정보 입력 함수
	*@param QBicBinary& oBinary 자료 입력 변수
	*/
    virtual void GetBinary(QBicBinary& oBinary);
public:
	/**
	*@brief 교차점 
	*@param const TxEnvelope& _oEnvelope 
	*@return 정상처리 ture, 비정상처리 false 
	*/
    virtual bool Intersect(const TxEnvelope& _oEnvelope) const;
	/**
	*@brief 교차점 
	*@param const TxPolyline& rOther 폴리라인
	*@return 정상처리 ture, 비정상처리 false 
	*/
    virtual bool Intersect(const TxPolyline& rOther) const;
public:
	/**
	*@brief 교차점 
	*@param TxEnvelopePtr spEnv 
	*@return 정상처리 ture, 비정상처리 false 
	*/
    virtual bool Intersect(TxEnvelopePtr spEnv) const;
	/**
	*@brief 교차점 
	*@param std::shared_ptr<TxPolyline> spLine 폴리라인
	*@return 정상처리 ture, 비정상처리 false 
	*/
    virtual bool Intersect(std::shared_ptr<TxPolyline> spLine) const;
public:
	/**
	*@brief 버텍스 설정
	*@param const std::vector<TxPoint>& vecPoint
	*/
    void SetPoints(const std::vector<TxPoint>& vecPoint);
public:
	/**
	*@brief 
	*/
    void Flip();
	/**
	*@brief 
	*@return 
	*/
    std::shared_ptr<TxPolyline> BackwardLine();
public:
	/**
	*@brief 최소거리
	*@param const TxPoint& oGivePt
	*@return 최소거리 반환
	*/
    virtual double MinDist(const TxPoint& oGivePt);
private:
	/**
	*@brief 
	*@param const TxPoint& oGivePt
	*@param TxPoint& outIP
	*@param double& dLineTraceLen
	*@param size_t& nxFindIdx
	*@param int nFlag
	*@return 
	*/
    virtual bool LineOnPoint(const TxPoint& oGivePt, TxPoint& outIP, double& dLineTraceLen, size_t& nxFindIdx, int nFlag);
public:
	/**
	*@brief 
	*@param const TxPoint& oGivePt
	*@param TxPoint& outIP
	*@return 
	*/
    virtual bool LineOnPoint(const TxPoint& oGivePt, TxPoint& outIP); 
	/**
	*@brief 
	*@param const TxPoint& oGivePt
	*@param TxPoint& outIP
	*@param double& dLineTraceLen
	*@return 
	*/
    virtual bool LineOnPoint(const TxPoint& oGivePt, TxPoint& outIP, double& dLineTraceLen);
	/**
	*@brief 
	*@param const TxPoint& oGivePt
	*@param TxPoint& outIP
	*@param double& dLineTraceLen
	*@param size_t& nxFindIdx
	*@return 
	*/
    virtual bool LineOnPoint(const TxPoint& oGivePt, TxPoint& outIP, double& dLineTraceLen, size_t& nxFindIdx/* 절점(0)-절점(1)-^-절점(2)-절점(3) 사이인 경우 1을 리턴*/);
    
public:
	/**
	*@brief 
	*@param double dLineTranceLen 
	*@param TxPoint& outOnPt
	*@param TxPoint& outOffsetPt
	*@param double& outAngle
	*@param double dOffset
	*@return 
	*/
    virtual void LineOnPoint(double dLineTraceLen, TxPoint& outOnPt, TxPoint& outOffsetPt, double& outAngle, double dOffset=0.0 );
    virtual void LineOnPoint(double dLineTraceLen, TxPoint& outOnPt);

	/**
	*@brief 
	*@param TxPoint& oGivePt : 기준되는 Point
	*@param TxPoint& outIP   : 라인상의 Point
	*@param double& outAngle : 방위각
	*@param double& outDist : 라인상의 Point와 기존 Point 와의 거리
	*@return 
	*/
	virtual bool LineOnPointAngleDist(const TxPoint& oGivePt, TxPoint& outIP, double& outAngle, double& outDist);

	virtual bool LineOnPointAngleLeftRightOutDist(const TxPoint& oGivePt, double& outAngle, double& doutDist, double& dLeftDist, double& dRightDist);

    virtual bool LineOnPointAngleLeftRightOutDist(const TxPoint& oGivePt, TxPoint& outIP, double& outAngle, double& doutDist, double& dLeftDist, double& dRightDist);

	virtual bool  LineOnPointOutDistAndPoint(const TxPoint& oGivePt, TxPoint& outIP, double& doutDist, int &nOnType);

public:
	/**
	*@brief 
	*@param double dRate
	*@param TxPoint& outPt
	*@param double dOffset
	*@return 
	*/
    virtual void LinePercentPt(double dRate, TxPoint& outPt, double  dOffset = 0);
		/**
	*@brief 
	*@param double dRate
	*@param TxPoint& outPt
	*@param double& outAngle
	*@param double dOffset
	*@return 
	*/
    virtual void LinePercentPt(double dRate, TxPoint& outPt, double& outAngle, double dOffset);
public:
	/**
	*@brief 거리
	*@return 계산값 반환
	*/
    virtual double Length();   
    
public:
	/**
	*@brief 기하학 복사
	*@return 복사값 반환
	*/
    virtual ITxGeometryPtr CopyGeometry();

public:
	/**
	*@brief 거리 추출
	*@param double _dDist 거리
	*@param std::vector<TxPoint>& vecLeft 왼쪽 벡터
	*@param std::vector<TxPoint>& vecRight 오른쪽 벡터
	*/
    virtual void ExtractLine(double _dDist, std::vector<TxPoint>& vecLeft, std::vector<TxPoint>& vecRight);
	/**
	*@brief 거리 추출
	*@param double dMinDist 최소 거리
	*@param double dMaxDist 최대 거리
	*@return 
	*/
    virtual std::vector<TxPoint> ExtractLine(double dMinDist, double dMaxDist);

public:
	/**
	*@brief 버텍스 개수
	*@return 개수 반환
	*/
    size_t VertexCount() {
        return m_nxPointCnt;
    }
public:
	/**
	*@brief 마지막 추가
	*@param const TxPoint& oPt
	*/
    void AddVertex   (const TxPoint& oPt);
	/**
	*@brief 버텍스 제거
	*@param int nIdx 버텍스
	*/
    void RemoveVertex(int nIdx);
	/**
	*@brief 버텍스 좌표 이동
	*@param in nIdx 버텍스 
	*@param const TxPoint& oPt 좌표
	*/
    void MoveVertex  (int nIdx, const TxPoint& oPt);
    /**
	*@brief 버텍스 좌표 이동
	*@param in nIdx 버텍스 
	*@param const TxPoint& oPt 좌표
	*/
    void InsertVertex(int nIdx, const TxPoint& oPt);
	/**
	*@brief 시점 변경
	*@param const TxPoint& oPt
	*/
    void StartPtMove (const TxPoint& oPt);
	/**
	*@brief 종점 변경
	*@param const TxpOint& oPt
	*/
    void EndPtMove   (const TxPoint& oPt);

    TxPoint StartPoint();
    TxPoint EndPoint();

public:
    int IsLineRight(const TxPoint& oPt);
public:
    std::vector<TxPoint> ExtractVertex(double dMinDist);
friend class TxPolygon;
};

typedef std::shared_ptr<TxPolyline> TxPolylinePtr;
/**
*@brief 폴리라인
*@details TxM 폴리라인
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxMPolyline : public ITxGeometry
{
public:
    TxMPolyline();
    TxMPolyline(const TxMPolyline& right);
    TxMPolyline(QBicBinary& oBinary, size_t nIndex=0);
    virtual ~TxMPolyline();
public:
    TxMPolyline& operator =  (const TxMPolyline& right);
private:
    std::vector<TxPolyline> m_vecPolyline;
public:
	/**
	*@brief 추가
	*@param const TxPolyline& oPline
	*/
    void Add(const TxPolyline& oPline);
public:
	/**
	*@brief 개수
	*@return 폴리라인 개수 반환
	*/
    size_t PartCount() {return m_vecPolyline.size();}
private:
    TxEnvelope m_oEnvelope;
public:
	/**
	*@brief 범위
	*@return 범위 확인
	*/
    virtual TxEnvelope Envelope() const;
public:
	/**
	*@brief 정보 입력 함수
	*@param QBicBinary& oBinary 자료 입력 변수
	*/
    virtual void GetBinary(QBicBinary& oBinary);

public:
	/**
	*@brief 교차점 
	*@param const TxEnvelope& _oEnvelope
	*@return 정상처리 ture, 비정상처리 false 
	*/
    virtual bool Intersect(const TxEnvelope& _oEnvelope) const;
public:
	/**
	*@brief 최소 거리
	*@param const TxPoint& oPt 
	*@return 최소거리 반환
	*/
    virtual double MinDist(const TxPoint& oPt);
public:
	/**
	*@brief 기하학 복사
	*@return 복사값 반환
	*/
    virtual ITxGeometryPtr CopyGeometry();
};

typedef std::shared_ptr<TxMPolyline> TxMPolylinePtr;