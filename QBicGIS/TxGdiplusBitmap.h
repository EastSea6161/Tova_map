#pragma once
/**
*@brief 비트맵?
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxGdiplusBitmap
{
private:
    Gdiplus::Bitmap* m_pBitmap;
public:
    TxGdiplusBitmap();
    TxGdiplusBitmap(int nWidth, int nHeight);
    TxGdiplusBitmap(LPCWSTR pFile);
    virtual ~TxGdiplusBitmap();
public:
	/**
	*@brief 비우기(초기화)
	*/
    void Empty();
	/**
	*@brief 불러오기
	*@param LPCWSTR pFile 파일 명
	*@return 파일 여는 것 반환
	*/
    bool Load(LPCWSTR pFile);
	/**
	*@brief 생성
	*@param int nWidth	가로길이
	*@param int nHeight 세로길이
	*/
    void Create(int nWidth, int nHeight);
public:
	/**
	*@brief 그림 그리기
	*@param Gdiplus::Bitmap* pBitmap 
	*@param float x						x 점		
	*@param float y						y 점
	*/
    void DrawImage(Gdiplus::Bitmap* pBitmap, float x, float y);
	/**
	*@brief 그림 그리기
	*@param Gdiplus::Bitmap* pBitmap	
	*@param Gdiplus::RectF rectDest		
	*@param float x						x 점		
	*@param float y						y 점
	*@param float fWidth				가로길이
	*@param float fHeight				세로길이
	*/
    void DrawImage(Gdiplus::Bitmap* pBitmap, Gdiplus::RectF rectDest, float x, float y, float fWidth, float fHeight);
public:
	/**
	*@brief 그림 작성
	*@param int nIndex
	*/
    void WriteImage(int nIndex=0);
	/**
	*@brief 그림 작성
	*@param Gdiplus::Bitmap* pBitmap
	*/
    void WriteImage(Gdiplus::Bitmap* pBitmap);
public:
    operator Gdiplus::Bitmap*() const { 
        return m_pBitmap; 
    }
public:
	/**
	*@brief 비트맵 추출
	*@return 비트맵 추출
	*/
    Gdiplus::Bitmap* GetBitmap() {
        return m_pBitmap;
    }
private:
	/**
	*@brief 
	*@param __in LPCWSTR pwszFormat
	*@param __out GUID *pGUID		
	*@return 
	*/
    HRESULT GetGdiplusEncoderClsid(__in LPCWSTR pwszFormat, __out GUID *pGUID);
};


typedef std::shared_ptr<TxGdiplusBitmap> TxGdiplusBitmapPtr;