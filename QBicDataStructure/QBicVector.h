#pragma once
/**
*@brief 벡터
*@details 큐빅웨어 벡터 설정
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class QBicVector
{
private:
    double i;
    double j;
    double k;
public:
    QBicVector(void);    
    QBicVector(double ii, double jj, double kk=0);
    QBicVector(const QBicVector& right);
    ~QBicVector(void);
public:
    QBicVector operator=( const QBicVector & right );
    QBicVector operator+( const QBicVector & right );
    QBicVector operator-( const QBicVector & right );
    bool operator== (const QBicVector& right);
public:
	/**
	*@brief I값 설정
	*@param double ii 입력받은 값
	*/
    void SetI( double ii );
	/**
	*@brief J값 설정
	*@param double jj 입력받은 값
	*/
    void SetJ( double jj );
	/**
	*@brief k값 설정
	*@param double kk 입력받은 값
	*/
    void SetK( double kk );
	/**
	*@brief 좌표 설정
	*@param double ii 입력받은 값
	*@param double jj 입력받은 값
	*@param double kk 입력받은 값
	*/
    void Set (double ii, double jj, double kk=0.0);
	/**
	*@brief i값 추출
	*@return i값 반환
	*/
    double GetI();
	/**
	*@brief j값 추출
	*@return j값 반환
	*/
    double GetJ();
	/**
	*@brief k값 추출
	*@return k값 반환
	*/
    double GetK();
public:
	/**
	*@brief 정규화작업
	*/
    void Normalize();
	/**
	*@brief 내적
	*@param const QBicVector& right
	*@return 
	*/
    double InnerProduct(const QBicVector& right);     //내적, Dot Product, Scalar Product
	/**
	*@brief 외적
	*@param const QBicVector& right
	*@return 
	*/
    QBicVector OuterProduct(const QBicVector& right); //외적, Cross Product
public:
	/**
	*@brief 길이
	*@return 계산값 반환
	*/
    double Length();
	/**
	*@brief 길이 설정
	*@param double dLength
	*/
    void SetLength(double dLength);
	/**
	*@brief 정수<->음수 변경
	*/
    void Negative();
	/**
	*@brief 시계방향으로 회전
	*@param double a_dAngle 각도
	*@param double dBegree 도
	*/
    void RotateClockwise  (double a_dAngle, bool bDegree=false);
    void RotateUnClockwise(double a_dAngle, bool bDegree=false);
	/**
	*@brief 회전
	*@param double a_dAngle 각도
	*@param double dBegree 도
	*/
    void Rotate(double a_dAngle, bool bDegree=false);
	/**
	*@brief 곱하기
	*@param double dScalar 값
	*/
    void Multiply(double dScalar);
	/**
	*@brief 탄젠트 값
	*@return 탄젠트i,j값
	*/
    double AzimuthAngle();
public:
	/**
	*@brief 사이의 각
	*@param const QBicVector& left
	*@param const QBicVector& right
	*@return 각도 반환
	*/
    static double     AngleBetween(const QBicVector& left, const QBicVector& right);
	/**
	*@brief 곱하기
	*@param double dScalar
	*@param const QBicVector& right
	*@return dScalar과 right의 변수들 곱 반환
	*/
    static QBicVector Multiply(double dScalar, const QBicVector& right);
public:
	/**
	*@brief left + right 값
	*@param const QBicVector& left
	*@param const QBicVector& right
	*@return 결과값 반환
	*/
    static QBicVector PlusOper (const QBicVector& left, const QBicVector& right);
	/**
	*@brief left - right 값
	*@param const QBicVector& left
	*@param const QBicVector& right
	*@return 결과값 반환
	*/
    static QBicVector MinusOper(const QBicVector& left, const QBicVector& right);
};

