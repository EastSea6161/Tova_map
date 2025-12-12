#pragma once

#include "ITxGeometry.h"
#include "TxFeature.h"
#include "TxM2Key.h"

class ThunderMapdrawProcess;

#ifndef IDShiftPos
    typedef TxM2Key<__int64, __int64> IDShiftPos;
#endif
	/**
	*@brief 임시 맵 서버
	*@details 
	*@author 큐빅웨어
	*@date 2016/05/17
	*@version 1.0.0
	*/
class AFX_EXT_CLASS TxTempMapServer
{
public:
    TxTempMapServer();
    virtual ~TxTempMapServer();

private:
    CString m_strName;
private:
    boost::iostreams::mapped_file m_mmapShp;
    size_t m_nxPosition;
private:    
    typedef std::pair<BoostEnvelope, IDShiftPos> TreeValue;
    BoostGeometry::index::rtree< TreeValue, BoostGeometry::index::quadratic<16> > m_oRTree;
private:
    size_t m_nxDataHeaderSize;
private:
	/**
	*@brief 파일 삭제
	*@details __int64 nxID
	*/
    void SoftFileDelete(__int64 nxID);
	/**
	*@brief 파일 초기화
	*/
    void SoftFileDelete();
public:
	/**
	*@brief 초기화
	*/
    void Clear();
private:
	/**
	*@brief 삽입
	*@param std::map<__int64, ITxGeometryPtr>& mapGeometry
	*@param size_t nxPos
	*/
    void Insert (std::map<__int64, ITxGeometryPtr>& mapGeometry, size_t nxPos);

private:
	/**
	*@brief 교차점
	*@param const TxEnvelope& oEnvelope
	*@return 
	*/
    std::vector<IDShiftPos> Intersect( const TxEnvelope& oEnvelope );
	/**
	*@brief Bin파일 읽기
	*@param size_t nxShiftPos 
	*@return 
	*/
    ITxGeometryPtr ReadBin2Geo(size_t nxShiftPos);
public:
	/**
	*@brief Geometry 가져오기
	*@param __int64 nxID  ID값
	*@return 
	*/
    ITxGeometryPtr LoadGeometry(__int64 nxID);
	/**
	*@brief Geometry 가져오기
	*@param TxEnvelope oEnvelope
	*@param std::map<__int64, ITxGeometryPtr>&
	*/
    void LoadGeometry(TxEnvelope oEnvelope, std::map<__int64, ITxGeometryPtr>& );
		/**
	*@brief Geometry 가져오기
	*@param TxEnvelope oEnvelope
	*@param std::map<__int64, ITxGeometryPtr>&
	*@param ThunderMapdrawProcess* 
	*/
    void LoadGeometry(TxEnvelope oEnvelope, std::map<__int64, ITxGeometryPtr>&, ThunderMapdrawProcess* );
public:
	/**
	*@brief 삽입
	*@param std::map<__int64, ITxGeometryPtr>& mapGeometry
	*/
    void Insert (std::map<__int64, ITxGeometryPtr>& mapGeometry);
public:
	/**
	*@brief 초기화
	*/
    virtual void Delete();
	/**
	*@brief ID값 초기화
	*@param __int64 nxID
	*/
    virtual void Delete(__int64 nxID);
	/**
	*@brief 업데이트
	*@param TxFeaturePtr spFeature
	*/
    virtual void Update(TxFeaturePtr spFeature);
	/**
	*@brief 삽입
	*@param TxFeaturePtr spFeature
	*/
    virtual void Insert(TxFeaturePtr spFeature);

public:
	/**
	*@brief 
	*@return 
	*/
    virtual TxEnvelope Extent();
	/**
	*@brief 엔티티 개수
	*@return 개수 반환
	*/
    virtual __int64    EntityCount();

public:
	/**
	*@brief 최대 ID
	*@return 
	*/
    virtual __int64 MaxID();
};

typedef std::shared_ptr<TxTempMapServer> TxTempMapServerPtr;