#pragma once

// Created By Nombara 2005. 05. 25
// Histroy : 
// ▶ Bursa-Wolf 변환식 적용
// ▶ 참고자료 : geotrans

#include <cmath>
#include "TxMath.h"

//#define	D2R(degree)	(degree	* PI / 180.0)
//#define	R2D(radian)	(radian	* 180.0	/ PI)
#define	LDouble	long double

enum EnumGeoSystem {
    KorTmKatec = 1, 
    KorUTMK    = 2, 
    KorUTMKMid = 5,
    KorTmWest  = 125, 
    KorTmMid   = 127, 
    KorTmEast  = 129, 
    KorTmJeju  = 128
};
/**
*@brief 공식 조직
*@details 공식 조직
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxCoordMath
{
public:
//     static LDouble D2R(LDouble degree)
//     {
//         LDouble rVal = 0.0;
//         /*rVal = degree * atan((LDouble)1.0) / (LDouble)45.0;*/
//         rVal = degree * (LDouble)TxMathVariable::PI / (LDouble)180.0;
//         return rVal;
//     };
//     static LDouble R2D(LDouble radian)
//     {
//         return radian * (LDouble)180.0 / (LDouble)TxMathVariable::PI;
//     };

    /*
    double TxMath::Degree2Radian( double dDegree )
    {
    double dRadian = dDegree * TxMathVariable::PI / 180.0;
    return dRadian;
    }

    double TxMath::Radian2Degree( double dRadian, bool bPlus )
    {
       double dDegree = dRadian * 180.0 / TxMathVariable::PI;
       dDegree = Mod(dDegree, 360.0);

    if (bPlus && dDegree < 0)
    {
    dDegree = 360 + dDegree;
    }
    return dDegree;
    }
    */
};
// CxEllipsoid, CxWGS84, CxBessel, CxBursaWolf -> 단순한 구조체로 정의한다.
// 로직은 CxCoordinateConv이 담당한다.
/**
*@brief 타원체
*@details 타원체
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxEllipsoid
{
public:
    LDouble		a;
    LDouble		b;
    LDouble		f;
    LDouble		e;
    LDouble		e2;

public:
    TxEllipsoid()
    {
        a	=	0.0;
        b	=	0.0;
        f	=	0.0;
        e2	=	0.0;
        e	=	0.0;
    };
    ~TxEllipsoid(){};
public:
	/**
	*@brief 편평도(Flatness of ellipsoid)를 체크한다
	*@param LDouble& f 편평도 반환
	*/
    void CheckF(LDouble& f)
    {
        if (f > 0.0)
        {
            f	= 1.0 / f;
        }		
    };
	/**
	*@brief 파라미터 설정
	*@param LDouble a	a값
	*@param LDouble f	f값
	*/
    void SetParameter(LDouble a, LDouble f)
    {
        this->a	=	a;		
        CheckF(f);
        this->f	=	f;
        this->b	=	a - a *	f;

        this->e2	=	( ::pow(a, (LDouble)2.0) - ::pow(b, (LDouble)2.0) ) / ::pow(a, (LDouble)2.0);
        this->e	=	sqrt(e2);		
    };	

	/**
	*@brief 횡곡률반경 계산
	*@param LDouble ldLan 위도
	*@return 계산값 반환
	*/
    LDouble N(LDouble ldLan)
    {
        LDouble	rVal	=	TxMath::Degree2Radian(ldLan);
        LDouble	sin2	=	pow((LDouble)sin(TxMath::Degree2Radian(ldLan)), (LDouble)2.0);

        LDouble	t	=	1 - e2*sin2;

        rVal	=	a / sqrt(t);

        return rVal;
    };
	/**
	*@brief 횡곡률반경 계산
	*@param LDouble ldLan 위도
	*@return 계산값 반환
	*/
    LDouble M(LDouble ldLan)
    {
        LDouble phi = TxMath::Degree2Radian(ldLan);
        LDouble e4  = pow(e, (LDouble)4.0);
        LDouble e6  = pow(e, (LDouble)6.0);

        LDouble M = (1.0 - e2/4.0 - 3.0 * e4 / 64.0 - 5.0 * e6 / 256.0) *phi;
        M = M - (3.0*e2/8.0 + 3.0 * e4 / 32.0 + 45.0 * e6 / 1024.0) * sin(2.0 * phi);
        M = M + (15.0 * e4 / 256.0 + 45.0 * e6 / 1024.0) * sin(4.0 * phi); 
        M = M - (35.0 * e6 / 3072.0) * sin(6.0 * phi);
        M = M * a;
        return M;
    }
};
/**
*@brief WGS84 좌표계
*@details WGS84 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxWGS84 : public TxEllipsoid
{
public:
    TxWGS84()
    {			
        SetParameter(6378137.0, 298.257223563);
    };
    ~TxWGS84(){};
};
/**
*@brief GRS80 좌표계
*@details GRS80
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxGRS80 : public TxEllipsoid
{
    // GRS80, WGS84나 고놈이 고놈이다. 동일하다라고 생각해도 된다.
public:
    TxGRS80()
    {			
        SetParameter(6378137.0, 298.257222101);
        // 장반경 : 6378137
        // 단반경 : 6356752.3 
    };
    ~TxGRS80(){};
};
/**
*@brief 베셀 좌표계
*@details 베셀
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxBessel : public TxEllipsoid
{
public:
    TxBessel()
    {
        SetParameter(6377397.155, 299.1528128);
    };
    ~TxBessel(){};
};
/**
*@brief 지구 시스템?
*@details 지구 시스템
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxGeoSystem
{
public:
    LDouble	sfactor;
    LDouble	lonCenter;
    LDouble	latCenter;
    LDouble	falseEast;
    LDouble	falseNorth;	

protected:
	/**
	*@brief 인수 설정
	*@param LDouble	factor 인수
	*/
    void SetFactor(LDouble	factor)
    {
        sfactor	=	factor;
    };
	/**
	*@brief 인수 설정
	*@param LDouble	east 인수
	*@param LDouble	north 인수
	*/
    void SetFalse(LDouble east, LDouble north)
    {
        falseEast	=	east;
        falseNorth	=	north;
    };
	/**
	*@brief 인수 설정
	*@param LDouble lat 위도
	*@param LDouble lon 경도
	*/
    void SetLanLonCenter(LDouble lat, LDouble lon)
    {
        latCenter	=	lat;
        lonCenter	=	lon;
    };
};
/**
*@brief TM좌표(Korea)
*@details TM좌표
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxTMKorea : public TxGeoSystem
{
public:
    TxTMKorea(EnumGeoSystem geoSystem)
    {
        SetGeoSystem(geoSystem);
    };	
    ~TxTMKorea(){};

public:
	/**
	*@brief 지구시스템 설정
	*@param EnumGeoSystem system 시스템
	*/
    void SetGeoSystem(EnumGeoSystem system)
    {
        if (system == KorTmKatec) {
            SetFactor(0.9999);
            SetFalse(400000.0, 600000.0);
        }

        if (system == KorTmJeju) {
            SetFactor(1.0);
            SetFalse(200000.0, 550000.0);
        }

        if (system == KorTmWest) {
            SetFactor(1.0);
            SetFalse(200000.0, 500000.0);
        }

        if (system == KorTmMid)  {
            SetFactor(1.0);
            SetFalse(200000.0, 500000.0);
        }

        if (system == KorTmEast) {
            SetFactor(1.0);
            SetFalse(200000.0, 500000.0);
        }

        // korUTMK
        if (system == KorUTMK) {
            SetFactor(0.9996);
            SetFalse(1000000.0, 2000000.0);
        }

        if (system == KorUTMKMid) {
            SetFactor(1.0);
            SetFalse(200000.0, 600000.0);
        }

        LDouble	lat	=	38.0;
        LDouble	lon	=	0.0;

        // 10.405초: 127 -> 127.0028902777778  0.002890277
        LDouble		minus	= 0.0028902777778;			
        if (system == KorTmKatec) {
            lon	=	128.0;
        }	

        if (system == KorTmWest) {
            lon	=	125.0	+	minus;
        }		

        if (system == KorTmMid)  {
            lon	=	127.0	+	minus;
        }		

        if (system == KorTmEast) {
            lon	=	129.0	+	minus;
        }

        if (system == KorUTMK) {
            lon	=	127.5;
        }

        if (system == KorUTMKMid) {
            lon	=	127.0;
        }

        SetLanLonCenter(lat, lon);
    };
};

/*
1. Α α Alpha 
2. Β β Beta 
3. Γ γ Gamma 
4. Δ δ Delta 
5. Ε ε Epsilon
6. Ζ ζ Zeta 
7. Η η Eta 
8. Θ θ Theta 
9. Ι ι Iota 
10. Κ κ Kappa 
11. Λ λ Lambda 
12. Μ μ Mu 
13. Ν ν Nu 
14. Ξ ξ Xi 
15. Ο ο Omicron
16. Π π Pi 
17. Ρ ρ Rho 
18. Σ σ Sigma 
19. Τ τ Tau 
20. Υ υ Upsilon
21. Φ φ Phi 
22. Χ χ Chi 
23. Ψ ψ Psi 
24. Ω ω Omega 
*/

// 세계측지계에서 기존 측지계로의 변환을 할 경우 부호를 반대로 해야 한다.
// 국토지리정보원의 고시는 한국측지계->세계측지계로의 변환계수로 값이 세팅되어 있기 때문에
// 고시된 값에 -1을 곱하는 원칙으로 정함.
/**
*@brief BursaWolf
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxBursaWolf
{
public:
    LDouble	ds;	// Δs
    LDouble	dX;	// Δx
    LDouble	dY;	// Δy
    LDouble	dZ;	// Δz
    LDouble	kappa;	// κ
    LDouble	phi;	// φ
    LDouble	omega;	// ω
public:	
    TxBursaWolf()
    {
        ds		=	6.43;
        dX		=	-115.80;
        dY		=	474.99;
        dZ		=	674.11;

        kappa	=	1.63;
        phi		=	2.31;
        omega	=	-1.16;
    };
    ~TxBursaWolf(){};


public:
	/**
	*@brief 파라메타 설정
	*@param LDouble ds		S값
	*@param LDouble dx		X값
	*@param LDouble dy		Y값
	*@param LDouble dz		Z값
	*@param LDouble kappa	Kappa값
	*@param LDouble phi		phi값
	*@param LDouble omega	omega값
	*/
    void SetParameter(LDouble ds, LDouble dx, LDouble dy, LDouble dz, LDouble kappa, LDouble phi, LDouble omega)
    {
        ds	=	ds;
        dX	=	dx;
        dY	=	dy;
        dZ	=	dz;

        this->kappa	=	kappa;
        this->phi	=	phi;
        this->omega	=	omega;
    };
};
/**
*@brief Molodensky
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxMolodensky
{
public:
    LDouble	ds;	// Δs
    LDouble	dX;	// Δx
    LDouble	dY;	// Δy
    LDouble	dZ;	// Δz
    LDouble	kappa;	// κ
    LDouble	phi;   // φ
    LDouble	omega;	// ω
public:	
    TxMolodensky()
    {
        ds		=	6.342;
        dX		=	-145.907;
        dY		=	505.034;
        dZ		=	685.756;

        kappa	=	1.592;
        phi		=	2.347;
        omega	=	-1.162;		
    };
    ~TxMolodensky(){};
    
public:
	/**
	*@brief 파라메타 설정
	*@param LDouble ds		S값
	*@param LDouble dx		X값
	*@param LDouble dy		Y값
	*@param LDouble dz		Z값
	*@param LDouble kappa	Kappa값
	*@param LDouble phi		phi값
	*@param LDouble omega	omega값
	*/
    void SetParameter(LDouble ds, LDouble dx, LDouble dy, LDouble	dz, LDouble kappa, LDouble phi, LDouble omega)
    {
        ds	=	ds;
        dX	=	dx;
        dY	=	dy;
        dZ	=	dz;

        this->kappa	=	kappa;
        this->phi	=	phi;
        this->omega	=	omega;
    };
};
/**
*@brief 좌표계 컨버트
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxCoordinateConv  
{
public:
	/**
	*@brief 좌표계 전환
	*@details 타원체의 위 경도값을 지심좌표계로 변환
	*@param TxEllipsoid& oEllipse	타원체
	*@param LDouble	geo_Lan			위도
	*@param LDouble geo_Lon			경도
	*@param LDouble geo_h			시간값
	*@param LDouble& car_X			X좌표
	*@param LDouble& car_Y			Y좌표
	*@param LDouble& car_Z			Z좌표
	*/
    void GeoCoordinate2Cartesian(TxEllipsoid& oEllipse, LDouble	geo_Lan, LDouble geo_Lon, LDouble geo_h, LDouble& car_X, LDouble& car_Y, LDouble& car_Z);
	/**
	*@brief 좌표계 전환
	*@details 지심좌표계 에서 타원체의 경위도로 변환
	*@param TxEllipsoid& oEllipse	타원체
	*@param LDouble car_X			X좌표
	*@param LDouble car_Y			Y좌표
	*@param LDouble car_Z			Z좌표
	*@param LDouble& rGeoLan		위도
	*@param LDouble& rGeoLon		경도
	*@param LDouble& h				시간값
	*/
    void CartesianCoordinate2Geographic(TxEllipsoid& oEllipse, LDouble car_X, LDouble car_Y, LDouble car_Z, LDouble&	rGeoLan, LDouble& rGeoLon, LDouble& h);

	/**
	*@brief BursaWolf 방식에의한 3차원 직각 좌표계 변환
	*@details (국토지리정보원 고시에 따라 폐지 됨)
	*/
    void TransBursaWolfBessel2GRS80(TxBursaWolf& oBursaWolf, LDouble inCar_X, LDouble inCar_Y, LDouble inCar_Z, LDouble& outCar_X, LDouble& outCar_Y, LDouble& outCar_Z);
	/**
	*@brief BursaWolf 방식에의한 3차원 직각 좌표계 변환
	*@details (국토지리정보원 고시에 따라 폐지 됨)
	*/
    void TransBursaWolfGRS802Bessel(TxBursaWolf& oBursaWolf, LDouble inCar_X, LDouble inCar_Y, LDouble inCar_Z, LDouble& outCar_X, LDouble& outCar_Y, LDouble& outCar_Z);

    // Molodensky 방식에의한 3차원 직각 좌표계 변환(국토지리정보원 고시 제2003-497호)
	/**
	*@brief 베셀좌표계에서 GRS80 좌표계 변환
	*@details Molodensky 방식에의한 3차원 직각 좌표계 변환(국토지리정보원 고시 제2003-497호)
	*@param LDouble inCar_X		X좌표
	*@param LDouble inCar_Y		Y좌표
	*@param LDouble inCar_Z		Z좌표
	*@param LDouble& outCar_X	X좌표
	*@param LDouble& outCar_Y	Y좌표
	*@param LDouble& outCar_Z	Z좌표
	*/
    void TransMolodenskyBessel2GRS80(LDouble inCar_X, LDouble inCar_Y, LDouble inCar_Z, LDouble& outCar_X, LDouble& outCar_Y, LDouble& outCar_Z);
	/**
	*@brief GRS80좌표계에서 베셀 좌표계 변환
	*@details Molodensky 방식에의한 3차원 직각 좌표계 변환(국토지리정보원 고시 제2003-497호)
	*@param LDouble inCar_X		X좌표
	*@param LDouble inCar_Y		Y좌표
	*@param LDouble inCar_Z		Z좌표
	*@param LDouble& outCar_X	X좌표
	*@param LDouble& outCar_Y	Y좌표
	*@param LDouble& outCar_Z	Z좌표
	*/
    void TransMolodenskyGRS802Bessel(LDouble inCar_X, LDouble inCar_Y, LDouble inCar_Z, LDouble& outCar_X, LDouble& outCar_Y, LDouble& outCar_Z);


    //void xKtm2GeoCoordinate(CxEllipsoid& oEllipsoid, CxGeoSystem& oGeoSystem, LDouble X, LDouble Y, LDouble& geoLan, LDouble& geoLon);

	/**
	*@brief 타원체경위도를 평면 지각좌표계로 전환
	*@details 타원체상의 경위도 좌표를 평면 직각좌표로 변환(국토지리정보원-1/1000수치지형도좌표변환표준작업지침)
	*@param TxEllipsoid& oEllipsoid	타원체
	*@param TxGeoSystem& oGeoSystem	직각좌표계
	*@param LDouble geoLan			위도
	*@param LDouble geoLon			경도
	*@param LDouble& X				X좌표
	*@param LDouble& Y				Y좌표
	*/
    void TxCoordinateConv::GeoCoordinate2XYCoordinate(TxEllipsoid& oEllipsoid, TxGeoSystem& oGeoSystem, LDouble geoLan, LDouble geoLon, LDouble& X, LDouble& Y);
	/**
	*@brief 평면 지각좌표계를 타원체경위도로 전환
	*@details 평면직각좌표를 타원체면상의 경위도 좌표로 변환(국토지리정보원-1/1000수치지형도좌표변환표준작업지침)
	*@param TxEllipsoid& oEllipsoid	타원체
	*@param TxGeoSystem& oGeoSystem	직각좌표계
	*@param LDouble X				X좌표
	*@param LDouble Y				Y좌표
	*@param LDouble& geoLan			위도
	*@param LDouble& geoLon			경도
	*/
    void TxCoordinateConv::XYCoordinate2GeoCoordinate(TxEllipsoid& oEllipsoid, TxGeoSystem& oGeoSystem, LDouble X, LDouble Y, LDouble& geoLan, LDouble& geoLon);
private:
	/**
	*@brief 
	*@param LDouble ap
	*@param LDouble bp
	*@param LDouble cp
	*@param LDouble dp
	*@param LDouble ep
	*@param LDouble sphi
	*@return 결과값 반환
	*/
    LDouble fnSPHTMD(LDouble ap, LDouble bp, LDouble cp, LDouble dp, LDouble ep, LDouble sphi);
	/**
	*@brief 모유곡선의 곡률반경
	*@param LDouble a
	*@param LDouble es
	*@param LDouble sphi
	*@return 결과값 반환
	*/
    LDouble fnSPHSN(LDouble a, LDouble es, LDouble sphi);
	/**
	*@brief 
	*@param LDouble a
	*@param LDouble es
	*@param LDouble sphi
	*@return 결과값 반환
	*/
    LDouble fnSPHSR(LDouble a, LDouble es, LDouble sphi);
	/**
	*@brief 
	*@param LDouble es
	*@param LDouble sphi
	*@return 결과값 반환
	*/
    LDouble fnDENOM(LDouble es, LDouble sphi);
public:
    TxCoordinateConv();
    virtual ~TxCoordinateConv();

private:
    TxMolodensky m_oMolodensky;
};
/**
*@brief 좌표계 컨버트
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxCoordinateConvert
{
public:
    TxCoordinateConvert() {};
    ~TxCoordinateConvert(){};
public:
	/**
	*@brief ???에서 도로 변환
	*@param double dValue 입력할 NMEA 단위
	*@return 도 반환
	*/
    static double NMEA2Degree(double dValue);
public:
	/**
	*@brief 카텍 좌표 -> WGS84 좌표
	*@param double x			카텍 X
	*@param double y			카텍 Y
	*@param double& rLatitude	위도 반환
	*@param double& rLongitude	경도 반환
	*/
    static void Katec2WGS84(double x,   double y,   double& rLatitude, double& rLongitude);
	/**
	*@brief WGS84 좌표 -> 카텍 좌표
	*@param double lat	WGS84 위도
	*@param double lon	WGS74 경도
	*@param double& rx	카텍 X
	*@param double& ry	카텍 Y
	*@return 
	*/
    static void WGS842Katec(double lat, double lon, double& rx, double& ry);
	/**
	*@brief WGS84 좌표 -> UTMK 좌표
	*@param double lat	WGS84 위도
	*@param double lon	WGS74 경도
	*@param double& rx	UTMK X
	*@param double& ry	UTMK Y
	*@return 
	*/
    static void WGS842UTMK (double lat, double lon, double& rx, double& ry);
	/**
	*@brief UTMK 좌표 -> WGS84 좌표
	*@param double x			UTMK X
	*@param double y			UTMK Y
	*@param double& rLatitude	위도 반환
	*@param double& rLongitude	경도 반환
	*/
    static void UTMK2WGS84 (double x,   double y,   double& rLatitude, double& rLongitude);
    
    static void WGS842UTMKMid (double lat, double lon, double& rx, double& ry);
    static void UTMKMid2WGS84 (double x,   double y,   double& rLatitude, double& rLongitude);
};