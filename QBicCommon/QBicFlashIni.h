#pragma once
/**
*@brief Ini 
*@details Ini 관련 클래스(입출력)
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicIni 
{
public:
    QBicIni();
    virtual ~QBicIni();

protected:
    std::map<CString, CString> m_mapKeyValue;
public:
    std::set<CString> Keys() {
        std::set<CString> setKey;
        for (auto iter = m_mapKeyValue.begin(); iter != m_mapKeyValue.end(); ++iter) {
            setKey.insert(iter->first);
        }

        return setKey;
    }
public:
	/**
	*@brief key값과 value값 작성
	*@param CString strKey		키값
	*@param CString strValue	벨류값
	*/
    virtual void Write(CString strKey, CString  strValue);
	/**
	*@brief key값과 value값 작성
	*@param CString strKey		키값
	*@param int     nValue		벨류값
	*@details value값을 CString 형식으로 수정하여 Write함수를 다시 호출
	*/
    virtual void Write(CString strKey, int      nValue);
	/**
	*@brief key값과 value값 작성
	*@param CString strKey		키값
	*@param double  dValue		벨류값
	*@details value값을 CString 형식으로 수정하여 Write함수를 다시 호출
	*/
    virtual void Write(CString strKey, double   dValue);
	/**
	*@brief key값과 value값 작성
	*@param CString strKey		키값
	*@param __int64 nxValue		벨류값
	*@details value값을 CString 형식으로 수정하여 Write함수를 다시 호출
	*/
    virtual void Write(CString strKey, __int64  nxValue);
	/**
	*@brief key값과 value값 작성
	*@param CString strKey		키값
	*@param COLORREF clrColor	색
	*@details value값을 CString 형식으로 수정하여 Write함수를 다시 호출
	*/
    virtual void Write(CString strKey, COLORREF clrColor);
	/**
	*@brief key값과 value값 작성
	*@param CString strKey				키값
	*@param std::vector<int>& vecValue	벨류값(벡터)
	*@details value값을 CString 형식으로 수정하여 Write함수를 다시 호출
	*/
    virtual void Write(CString strKey, std::vector<int>& vecValue);
public:
	/**
	*@brief 벡터Int값 읽어오기
	*@details 넣으려는 값이 존재를 하게 된다면, 다시 원래의 vecValue를 반환한다.
	*@param CString strKey				키값
	*@param std::vector<int>& vecValue	벨류값(벡터)
	*@return 정상적으로 함수가 돌아간다면 vec의 번지수가 반환 된다.
	*/
    virtual std::vector<int> ReadVecInt(CString strKey, std::vector<int>& vecValue);
public:
	/**
	*@brief 읽어오기
	*@details 번지값 읽어오기
	*@param CString strKey		키값
	*@param CString strDefault	벨류값
	*@return 정상적으로 불러오면 번지값을, 실패하면 value값을 반환
	*/
    virtual CString Read   (CString strKey, CString strDefault =_T(""));
	/**
	*@brief 읽어오기
	*@details Int형식으로 번지값 읽어오기
	*@param CString strKey		키값
	*@param int     nDefault	벨류값
	*@return 번지값 읽어오기
	*/
    virtual int     ReadInt(CString strKey, int     nDefault   = 0);
	/**
	*@brief 읽어오기
	*@details Double형식으로 번지값 읽어오기
	*@param CString strKey		키값
	*@param double  dDefault	벨류값
	*@return 번지값 읽어오기
	*/
    virtual double  ReadDbl(CString strKey, double  dDefault   = 0);
	/**
	*@brief 읽어오기
	*@details Int64형식으로 번지값 읽어오기
	*@param CString strKey		키값
	*@param __int64 nxDefault	벨류값
	*@return 번지값 읽어오기
	*/
    virtual __int64 ReadI64(CString strKey, __int64 nxDefault  = 0);
public:
	/**
	*@brief 색상 불러오기
	*@details 색의 3원소를 기준으로 색 가져오기
	*@param CString strKey		키값
	*@param COLORREF clrDefault	색상
	*@return RGB(nR, nG, nB) 반환
	*/
    COLORREF ReadRGB(CString strKey, COLORREF clrDefault = RGB(0,0,0));

    friend class QBicGrpIni;
};

typedef std::shared_ptr<QBicIni> QBicIniPtr;
/**
*@brief grp
*@details grp 관련 클래스(입출력)
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicGrpIni
{
public:
    QBicGrpIni();
    virtual ~QBicGrpIni();
protected:
    std::map<int, std::map<CString, CString>> m_mapTypeKeyValue;
public:
    std::set<CString> Keys(int nType) {
        std::set<CString> setKey;
        auto iter = m_mapTypeKeyValue.find(nType);
        if (iter == m_mapTypeKeyValue.end())
            return setKey;

        std::map<CString, CString>& mapKeyValue = iter->second;
        
        for (auto iter = mapKeyValue.begin(); iter != mapKeyValue.end(); ++iter) {
            setKey.insert(iter->first);
        }

        return setKey;
    }
public:
	/**
	*@brief grp 작성
	*@details grp 타입, 키값, 벨류값
	*@param int nType			타입
	*@param CString strKey		키값
	*@param CString  strValue	벨류값
	*/
    virtual void Write(int nType, CString strKey, CString  strValue);
	/**
	*@brief grp 작성
	*@details grp 타입, 키값, 벨류값을 int형식으로 변경
	*@param int nType			타입
	*@param CString strKey		키값
	*@param int      nValue	벨류값
	*/
    virtual void Write(int nType, CString strKey, int      nValue);
	/**
	*@brief grp 작성
	*@details grp 타입, 키값, 벨류값을 double형식으로 변경
	*@param int nType			타입
	*@param CString strKey		키값
	*@param double   dValue	벨류값
	*/
    virtual void Write(int nType, CString strKey, double   dValue);
	/**
	*@brief grp 작성
	*@details grp 타입, 키값, 벨류값을 __int64형식으로 변경
	*@param int nType			타입
	*@param CString strKey		키값
	*@param __int64  nxValue	벨류값
	*/
    virtual void Write(int nType, CString strKey, __int64  nxValue);
	/**
	*@brief grp 작성
	*@details grp 타입, 키값, 벨류값을 색상
	*@param int nType			타입
	*@param CString strKey		키값
	*@param COLORREF clrColor	색상
	*/
    virtual void Write(int nType, CString strKey, COLORREF clrColor);
	/**
	*@brief grp 작성
	*@details grp 타입, 키값, 벨류값을 벡터 int 형식으로 변경
	*@param int nType					타입
	*@param CString strKey				키값
	*@param std::vector<int>& vecValue	벨류값
	*/
    virtual void Write(int nType, CString strKey, std::vector<int>& vecValue);
	/**
	*@brief grp 작성
	*@details grp 타입, 키값, 벨류값
	*@param int nType							타입
	*@param CString strKey						키값
	*@param std::shared_ptr<QBicIni> spIni	벨류값
	*/
	virtual void Write(int nType, CString strKey, std::shared_ptr<QBicIni> spIni);
public:
	/**
	*@brief 벡터Int값 읽어오기
	*@details 넣으려는 값이 존재를 하게 된다면, 다시 원래의 vecValue를 반환한다.
	*@param int nType							타입
	*@param CString strKey						키값
	*@param std::vector<int>& vecValue			벨류값
	*@return 정상적으로 함수가 돌아간다면 vec의 번지수가 반환 된다.
	*/
    virtual std::vector<int> ReadVecInt(int nType, CString strKey, std::vector<int>& vecValue);
public:

    virtual std::shared_ptr<QBicIni> ReadIni(int nType, CString strKey, std::shared_ptr<QBicIni> spDefault);
public:
	/**
	*@brief 읽어오기
	*@details 번지값 읽어오기
	*@param int nType			타입
	*@param CString strKey		키값
	*@param CString strDefault	기본값
	*@return 정상적으로 불러오면 번지값을, 실패하면 value값을 반환
	*/
    virtual CString Read   (int nType, CString strKey, CString strDefault=_T(""));
	/**
	*@brief 읽어오기
	*@details Int형식으로 번지값 읽어오기
	*@param int nType			타입
	*@param CString strKey		키값
	*@param int     nDefault	기본값
	*@return 번지값 읽어오기
	*/
    virtual int     ReadInt(int nType, CString strKey, int     nDefault);
	/**
	*@brief 읽어오기
	*@details Double형식으로 번지값 읽어오기
	*@param int nType			타입
	*@param CString strKey		키값
	*@param double  dDefault	기본값
	*@return 번지값 읽어오기
	*/
    virtual double  ReadDbl(int nType, CString strKey, double  dDefault);
	/**
	*@brief 읽어오기
	*@details Int64형식으로 번지값 읽어오기
	*@param int nType			타입
	*@param CString strKey		키값
	*@param __int64 nxDefault	기본값
	*@return 번지값 읽어오기
	*/
    virtual __int64 ReadI64(int nType, CString strKey, __int64 nxDefault);

public:
	/**
	*@brief 색상 불러오기
	*@details 색의 3원소를 기준으로 색 가져오기
	*@param int nType			타입
	*@param CString strKey		키값
	*@param COLORREF clrDefault	기본색상
	*@return RGB(nR, nG, nB) 반환
	*/
    COLORREF ReadRGB(int nType, CString strKey, COLORREF clrDefault);
public:
	/**
	*@brief 멤버변수 데이터의 고정작업
	*@param std::shared_ptr<QBicGrpIni> spFlashIni
	*/
    virtual void SetData(std::shared_ptr<QBicGrpIni> spFlashIni);    
public:
	/**
	*@brief 멤버변수 데이터(번지값)의 고정 작업
	*@param std::shared_ptr<QBicGrpIni> spFlashIni
	*@param int nType									타입
	*/
    virtual void SetData(std::shared_ptr<QBicGrpIni> spFlashIni, int nType);  
};

typedef std::shared_ptr<QBicGrpIni> QBicGrpIniPtr;