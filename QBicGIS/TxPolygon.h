#pragma once

#include "ITxGeometry.h"
#include "TxPoint.h"
#include "TxPolyline.h"

class TxMPolygon;
/**
*@brief 폴리곤
*@details Tx 폴리곤
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxPolygon : public ITxGeometry
{
public:
	TxPolygon();
	TxPolygon(const std::vector<TxPoint>& vecPoint);
	TxPolygon(const TxPolygon& right);
    TxPolygon(std::shared_ptr<TxPolygon> spPoly);
	TxPolygon(QBicBinary& oBinary, size_t nIndex=0);
	TxPolygon(BoostPolygon& oBoostPoly);
	virtual ~TxPolygon();
public:
	TxPolygon& operator =  (const TxPolygon& right);

	// 폴리곤 정의
	// 한개의 OutRing과 복수의 Hole
	// OutRing은 시계방향으로 구성
	// Hole은 반시계 방향으로 구성된다.
	// Hole은 OutRing 내부에 있어야 한다.
private:
	TxPolyline* m_pOutRing; 
public:
	std::vector<TxPoint> OutRing() const;
	void OutRing(const std::vector<TxPoint>& vecPoint, int nType=0);

private:
	std::vector<TxPolyline*> m_vecHoleRing;
public:
	/**
	*@brief Hole의 개수
	*@return Hole의 개수 반환
	*/
	size_t GetHoleCount() const {
		return m_vecHoleRing.size();
	}
	/**
	*@brief Hole정보 추출
	*@param size_t nIndex 번지수
	*@return Hole 정보 추출
	*/
	std::vector<TxPoint> GetHolePoints(size_t nIndex) const
	{
		return m_vecHoleRing[nIndex]->GetPoints();
	}
	/**
	*@brief Hole정보 추가
	*@param const std::vector<TxPoint>& vecPoint Point 자료t
	*@return 성공시 true, 실패시 false 반환
	*/
	bool AddHolePoint(const std::vector<TxPoint>& vecPoint);

private:
	TxEnvelope  m_oEnvelope;
public:
	/**
	*@brief 범위
	*@return 범위 확인
	*/
	virtual TxEnvelope Envelope() const;
public:
	/**
	*@brief 표출
	*@return Polygon / Hole 관련 정보 출력
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
	*@param const TxEnvelope& _oEnvelope 폴리곤 
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Intersect(const TxEnvelope& _oEnvelope) const;
	/**
	*@brief 교차점 
	*@param const TxPolyline& oPolyline 폴리라인
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Intersect(const TxPolyline& oPolyline)  const;
	/**
	*@brief 교차점 
	*@param std::shared_ptr<TxPolyline> spPolyline 폴리라인
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Intersect(std::shared_ptr<TxPolyline> spPolyline) const;
	/**
	*@brief 교차점 
	*@param const TxPolygon&  oPolygon 폴리곤
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Intersect(const TxPolygon&  oPolygon)   const;    
	/**
	*@brief 교차점 
	*@param std::shared_ptr<TxPolygon>  spPolygon 폴리곤
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Intersect(std::shared_ptr<TxPolygon>  spPolygon)  const;
	/**
	*@brief 교차점 
	*@param std::shared_ptr<TxMPolygon>  spMPolygon 멀티 폴리곤
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Intersect(std::shared_ptr<TxMPolygon> spMPolygon) const;
	/**
	*@brief 교차점 
	*@param const TxPoint& oPt 버텍스
	*@return Contains(oPt)를 반환
	*/
	virtual bool Intersect(const TxPoint& oPt) const {
		return Contains(oPt);
	}
	/**
	*@brief 교차점 
	*@param std::shared_ptr<TxPoint> spPoint 버텍스
	*@return Contains(spPoint)를 반환
	*/
	virtual bool Intersect(std::shared_ptr<TxPoint> spPoint) const {
		return Contains(spPoint);
	}
	/**
	*@brief 교차지점 확인
	*@param const TxPoint& oPt 버텍스
	*@return IsInPoint(oPt)를 반환
	*/
	virtual bool Contains(const TxPoint& oPt) const;
	/**
	*@brief 교차지점 확인
	*@param std::shared_ptr<TxPoint> spPoint 버텍스
	*@return IsInPoint(spPoint)를 반환
	*/
	virtual bool Contains(std::shared_ptr<TxPoint> spPoint) const;
	/**
	*@brief 교차지점 확인
	*@param const TxPolyline& oPolyline 폴리라인
	*@return Contains(spPolyline)를 반환
	*/
	virtual bool Contains(const TxPolyline& oPolyline) const;
	/**
	*@brief 교차지점 확인
	*@param std::shared_ptr<TxPolyline> spPolyline 폴리라인
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Contains(std::shared_ptr<TxPolyline> spPolyline) const;
	/**
	*@brief 교차지점 확인
	*@param std::shared_ptr<TxPolygon>  spPolygon 폴리곤
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Contains(std::shared_ptr<TxPolygon>  spPolygon)  const;
	/**
	*@brief 교차지점 확인
	*@param std::shared_ptr<TxMPolygon>  spMPolygon 멀티폴리곤
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Contains(std::shared_ptr<TxMPolygon> spMPolygon) const;
private:
public:
	/**
	*@brief 내부에 있는지 확인(버텍스)
	*@param const TxPoint& _oPt 가지고 있는 좌표
	*@return 정상적이면 ture, 맞지않으면 false 반환
	*/
	bool IsInPoint(const TxPoint& oPt) const;
    bool IsInPoint(TxPointPtr spPt) const;
private:
	/**
	*@brief 시계방향으로 재는 넓이
	*/
	void ConvertClockwiseOutRing();
public:
	/**
	*@brief 넓이
	*@return 넓이 반환
	*/
	double Area();
private:
	/**
	*@brief 넓이
	*@param TxPolyline* pPolyline 라인
	*@param int nType 
	*@return 성공시 넓이, 실패시 0.0 반환
	*/
	double Area(TxPolyline* pPolyline, int nType=1);

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
public:
	/**
	*@brief 교차 길이 
	*@param std::shared_ptr<TxPolyline> spPolyline
	*@return 교차 길이 반환
	*/
	virtual double OverLineLength(std::shared_ptr<TxPolyline> spPolyline) const;

public:
	/**
	*@brief 중심
	*@return 중심 좌표 반환
	*/
	TxPoint Centroid();
public:
	/**
	*@brief 
	*@return
	*/
	BoostPolygon TxPolygon::ToBoost();
	friend class TxMPolygon;
};

typedef std::shared_ptr<TxPolygon> TxPolygonPtr;
/**
*@brief 폴리곤
*@details TxM 폴리곤
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxMPolygon : public ITxGeometry
{
public:
	TxMPolygon();
	TxMPolygon(const TxMPolygon& right);
	TxMPolygon(QBicBinary& oBinary, size_t nIndex=0);
	virtual ~TxMPolygon();
public:
	TxMPolygon& operator =  (const TxMPolygon& right);

private:
	std::vector<TxPolygon*> m_vecSimplePolygon;
public:
	/**
	*@brief 초기화
	*/
	void Clear();
public:
	/**
	*@brief 정보 입력 함수
	*@param QBicBinary& oBinary 자료 입력 변수
	*/
	virtual void GetBinary(QBicBinary& oBinary);
	/**
	*@brief 표출
	*@return Polygon 관련 정보 출력
	*/
	virtual CString ToString();
private:
	TxEnvelope  m_oEnvelope;
public:
	/**
	*@brief 범위
	*@return 범위 확인
	*/
	virtual TxEnvelope Envelope() const
	{
		return m_oEnvelope;
	}

public:
	/**
	*@brief 교차점 
	*@param const TxEnvelope& _oEnvelope 폴리곤 
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Intersect(const TxEnvelope& _oEnvelope) const;
	/**
	*@brief 교차점 
	*@param const TxPolyline& oPolyline 폴리라인
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Intersect(const TxPolyline& oPolyline)  const;
	/**
	*@brief 교차점 
	*@param std::shared_ptr<TxPolyline> spPolyline 폴리라인
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Intersect(std::shared_ptr<TxPolyline> spPolyline) const;
	/**
	*@brief 교차점 
	*@param const TxPolygon&  oPolygon 폴리곤
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Intersect(const TxPolygon& oPolygon)    const;
	/**
	*@brief 교차점 
	*@param std::shared_ptr<TxPolygon>  spPolygon 폴리곤
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Intersect(std::shared_ptr<TxPolygon> spPolygon)   const;
	/**
	*@brief 교차점 
	*@param const TxPoint& oPt 버텍스
	*@return Contains(oPt)를 반환
	*/
	virtual bool Contains(const TxPoint& oPt) const;
	/**
	*@brief 교차점 
	*@param std::shared_ptr<TxPoint> spPoint 버텍스
	*@return Contains(spPoint)를 반환
	*/
	virtual bool Contains(std::shared_ptr<TxPoint> spPoint) const;
	/**
	*@brief 교차지점 확인
	*@param const TxPolyline& oPolyline 폴리라인
	*@return Contains(spPolyline)를 반환
	*/
	virtual bool Contains(const TxPolyline& oPolyline) const;
	/**
	*@brief 교차지점 확인
	*@param std::shared_ptr<TxPolyline> spPolyline 폴리라인
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Contains(std::shared_ptr<TxPolyline> spPolyline) const; 
	/**
	*@brief 교차지점 확인
	*@param std::shared_ptr<TxPolygon>  spPolygon 폴리곤
	*@return 정상처리 ture, 비정상처리 false 
	*/
	virtual bool Contains(std::shared_ptr<TxPolygon>  spPolygon) const;
public:
	void AddSubPolygon(const TxPolygon& oSimplePoly);
    void AddSubPolygon(std::shared_ptr<TxPolygon> spPoly);
public:
	size_t SubPolygonCount() {
		return m_vecSimplePolygon.size();
	}

	TxPolygon* SubPolygon(size_t nxIndex) {
		return m_vecSimplePolygon[nxIndex];
	}

public:
	/**
	*@brief 넓이
	*@return 넓이 반환
	*/
	double Area();
public:
	/**
	*@brief 내부에 있는지 확인(버텍스)
	*@param const TxPoint& _oPt 가지고 있는 좌표
	*@return 정상적이면 ture, 맞지않으면 false 반환
	*/
	bool IsInPoint(TxPoint& oPt);
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

public:
	/**
	*@brief 교차 길이 
	*@param std::shared_ptr<TxPolyline> spPolyline
	*@return 교차 길이 반환
	*/
	virtual double OverLineLength(std::shared_ptr<TxPolyline> spPolyline) const;
};

typedef std::shared_ptr<TxMPolygon> TxMPolygonPtr;