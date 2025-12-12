#pragma once
/**
*@brief 벡터
*@details 벡터
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxVector
{
private:
    double i;
    double j;
    double k;
public:
    TxVector(void);    
    TxVector(double ii, double jj, double kk=0);
    TxVector(const TxVector& right);
    ~TxVector(void);
public:
    TxVector operator=( const TxVector & right );
    TxVector operator+( const TxVector & right );
    TxVector operator-( const TxVector & right );
    bool operator== (const TxVector& right);
public:
	/**
	*@brief I 설정
	*/
    void SetI( double ii );
	/**
	*@brief J 설정
	*/
    void SetJ( double jj );
	/**
	*@brief K 설정
	*/
    void SetK( double kk );
	/**
	*@brief 좌표 설정
	*@param double ii i값
	*@param double jj j값
	*@param double kk k값
	*/
    void Set (double ii, double jj, double kk=0.0);
	/**
	*@brief I값 추출
	*@return I값
	*/
    double GetI();
	/**
	*@brief J값 추출
	*@return J값
	*/
    double GetJ();
	/**
	*@brief K값 추출
	*@return K값
	*/
    double GetK();
public:
	/**
	*@brief 유효성 검사
	*/
    void Normalize();
	/**
	*@brief 내적 생성
	*@param const TxVector& right
	*@return 값 반환
	*/
    double InnerProduct(const TxVector& right);   //내적, Dot Product, Scalar Product
	/**
	*@brief 외적 생성
	*@param const TxVector& right
	*@return 값 반환
	*/
    TxVector OuterProduct(const TxVector& right); //외적, Cross Product
public:
	/**
	*@brief 거리
	*@return 거리
	*/
    double Length();
	/**
	*@brief 거리 설정
	*@param double dLength 거리
	*/
    void   SetLength(double dLength);
	/**
	*@brief 음수로 전환
	*/
    void Negative();
	/**
	*@brief 시계바퀴?
	*@param double a_dAngle	각도
	*@param bool bDegree	사용 유무
	*/
    void RotateClockwise  (double a_dAngle, bool bDegree=false);
    void RotateUnClockwise(double a_dAngle, bool bDegree=false);
	/**
	*@brief 바퀴?
	*@param double a_dAngle	각도
	*@param bool bDegree	사용 유무
	*/
    void Rotate(double a_dAngle, bool bDegree=false);
	/**
	*@brief 곱하기
	*@param double dScalar	스칼라
	*/
    void Multiply(double dScalar);
	/**
	*@brief 방위각
	*@return  방위각 반환
	*/
    double Azimuth();
public:
	/**
	*@brief 사이각
	*@param const TxVector& left
	*@param const TxVector& right
	*@return 사이각 반환
	*/
    static double   AngleBetween(const TxVector& left, const TxVector& right);
	/**
	*@brief 곱하기
	*@param double dScalar			스칼라
	*@param const TxVector& right	
	*@return 결과물 반환
	*/
    static TxVector Multiply(double dScalar, const TxVector& right);

public:
	/**
	*@brief 더하기
	*@param const TxVector& left	왼쪽 벡터
	*@param const TxVector& right	오른쪽 벡터
	*@return 더한 값 반환
	*/
    static TxVector PlusOper (const TxVector& left, const TxVector& right);
	/**
	*@brief 빼기
	*@param const TxVector& left	왼쪽 벡터
	*@param const TxVector& right	오른쪽 벡터
	*@return 뺀 값 반환
	*/
    static TxVector MinusOper(const TxVector& left, const TxVector& right);
};

