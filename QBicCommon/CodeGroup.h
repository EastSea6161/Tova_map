#pragma once
/**
*@brief 코드그룹
*@details 간단한 코드들을 모아놓은 클래스
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KCodeGroup
{
public:
	KCodeGroup(KDBaseConnectionPtr a_spDBaseConnection, int nKey, LPCTSTR strGroupName, LPCTSTR strSystemCodeYN);
	KCodeGroup(const KCodeGroup& r);
	~KCodeGroup(void);

private:
    KDBaseConnectionPtr m_spDBaseConnection;
public :
	static const CString TABLE_CODEDATA;
	KCodeGroup& operator=(const KCodeGroup& r);
public:
	/**
	*@brief 코드그룹 키
	*@param int nKey	키값
	*@details 입력된 수를 그룹키로 변경
	*/
	void CodeGroupKey(int nKey);
	/**
	*@brief 코드그룹 키
	*@return 그룹키를 반환
	*/
	int  CodeGroupKey(void);
public:
	/**
	*@brief 코드그룹 명칭
	*@param LPCTSTR strName	명칭
	*@details 입력된 글을 그룹명으로 변경
	*/
	void    CodeGroupName(LPCTSTR strName);
	/**
	*@brief 코드그룹 명칭
	*@return 그룹 명칭을 반환
	*/
	LPCTSTR CodeGroupName(void);
public:
	/**
	*@brief 시스템 코드
	*@param LPCTSTR strSysCodeYN	시스템 코드 Yes / No
	*@details 시스템 코드 작동 여부
	*/
	void    SystemCodeYN(LPCTSTR strSysCodeYN);
	/**
	*@brief 시스템 코드
	*@return 시스템 코드 작동 여부 값을 리턴
	*/
	LPCTSTR SystemCodeYN(void);
public:
	/**
	*@brief 코드의 개수
	*@return 시스템 코드 작동 여부 값을 리턴
	*/
	int CodeCount(void);
public:
	/**
	*@brief 코드데이터 읽어오기
	*@details sqlite를 통하여 kcode_key, kcode_display를 불러오기
	*@return 성공이면 ture, 실패면 false 반환
	*@throws 실패시 디버그
	*/
	bool ReadCodeData();
	/**
	*@brief 코드데이터 추가
	*@details 코드그룹키, 코드키, 코드디스플레이 추가
	*@param int nCode			코드
	*@param LPCTSTR strValue	값
	*@return 성공이면 ture, 실패면 false 반환
	*@throws 실패시 디버그
	*/
	bool AddCode(int nCode, LPCTSTR strValue);
	/**
	*@brief 코드데이터 수정
	*@details 코드그룹키, 코드키를 조건으로 코드디스플레이를 수정
	*@param int nCode			코드
	*@param LPCTSTR strValue	값
	*@return 성공이면 ture, 실패면 false 반환
	*@throws 실패시 디버그
	*/
	bool UpdateCode(int nCode, LPCTSTR strValue);
	/**
	*@brief 코드데이터 삭제
	*@details 코드그룹키, 코드키를 조건으로 삭제
	*@param int nCode		코드
	*@throws 실패시 디버그
	*/
	void RemoveCode(int nCode);
	/**
	*@brief 코드데이터그룹 전체 삭제
	*@details 코드그룹키를 조건으로 그룹 전체 삭제
	*@throws 실패시 디버그
	*/
	void RemoveAllCode(void);
	/**
	*@brief 코드데이터 전체 삭제
	*@details 코드데이터 전체 삭제 clear();
	*@throws 실패시 디버그
	*/
	void ResetCodeData(void);
	/**
	*@brief 코드/코드명 호출
	*@details 코드번호 : 코드명 호출
	*@param int nCode			코드
	*@param int nType			타입
	*@return 코드번호 : 코드명 반환 
	*/
	CString SingleCodeValue(int nCode, int nType=0);
	/**
	*@brief 코드값 호출
	*@details 여러개의 코드번호 : 코드명 호출
	*@param LPCTSTR a_strCodes	글자형식 코드
	*@param int nType			타입
	*@return 코드번호 : 코드명 , 코드번호 : 코드명 반환
	*/
    CString MultiCodeValue(LPCTSTR a_strCodes, int nType=0);
	/**
	*@brief 코드값 넣기
	*@details 벡터 형식 코드값 넣기
	*@param std::vector<int>& codes	코드(다수)
	*/
	void GetCodes(std::vector<int>& codes);
	/**
	*@brief 코드값 넣기
	*@param std::map<int, CString>& a_mapCode	키 / 값
	*@details map 형식 코드값 넣기
	*/
    void GetCodes(std::map<int, CString>& a_mapCode);    
protected:
	int     m_nGroupKey;
	CString m_strGroupName;
    CString m_strSystemCodeYN;
	std::map<int, CString> m_mapCode;	
};
