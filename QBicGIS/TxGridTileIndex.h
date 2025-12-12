#pragma once
/**
*@brief 그리드 ID
*@details 그리드 셋팅
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxGridID
{
public:
    __int64 Gx;
    __int64 Gy;
public:
    TxGridID(__int64 _LeftX =0, __int64 _BottomY = 0)
    {
        Gx   = _LeftX;
        Gy = _BottomY;
    }
    ~TxGridID(void){}
public:
    bool operator <  (const TxGridID& right) const
    {
        if ( Gx == right.Gx ) 
        {
            return Gy < right.Gy;
        }
        else 
        {
            return Gx < right.Gx;
        }
    }
    bool operator == (const TxGridID& right) const
    {
        return (Gx == right.Gx && Gy == right.Gy);
    }
};
/**
*@brief 그리드 인덱스 조회
*@details 0,0 기준 그리드 정보 조회
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
// 0, 0을 기준으로 그리드 인덱스 정보 조회
class AFX_EXT_CLASS TxGridTileIndex
{
public:
    TxGridTileIndex(double dGridSize);
    virtual ~TxGridTileIndex();

private:
    double m_dGridMinX;
    double m_dGridMinY;
private:
    double m_dGridSize;
public:
	/**
	*@brief 
	*@param double x 
	*@param double y
	*@param __int64& Gx
	*@param __int64& Gy
	*/
    void GetGeoGridID(double x, double y, __int64& Gx, __int64& Gy);
	/**
	*@brief 
	*@param double dMinX  
	*@param double dMinY
	*@param double dMaxX
	*@param double dMaxY
	*/
    std::set<TxGridID> GetGeoGridID(double dMinX, double dMinY, double dMaxX, double dMaxY);
};