#include "stdafx.h"
#include "TxCoordinateConvert.h"

TxCoordinateConv::TxCoordinateConv()
{

}

TxCoordinateConv::~TxCoordinateConv()
{

}

//타원체의 위.경도 값을 지심좌표계로 변환.
void TxCoordinateConv::GeoCoordinate2Cartesian(TxEllipsoid& oEllipse, LDouble geo_Lan, LDouble geo_Lon, LDouble geo_h, LDouble& car_X, LDouble& car_Y, LDouble& car_Z)
{
	LDouble	rLan	=	TxMath::Degree2Radian(geo_Lan);
	LDouble	rLon	=	TxMath::Degree2Radian(geo_Lon);
	
	LDouble N = fnSPHSN(oEllipse.a, oEllipse.e2, TxMath::Degree2Radian(geo_Lan));
	
	car_X	=	(N + geo_h) * cos(rLan)*cos(rLon);
	car_Y	=	(N + geo_h) * cos(rLan)*sin(rLon);
	//car_Z	=	((pow(oEllipse.b, (LDouble)2.0) / pow(oEllipse.a, (LDouble)2.0)) * N + geo_h) * sin(rLan);
	car_Z   = (N * ( 1.0 - oEllipse.e2) + geo_h) * sin(rLan);
}


// 2002년 국토지리정보원 고시를 기준으로 작성
// 2003년 국토지리정보원 폐지
// 기존측지계로 부터 -> 세계측지계로의 변환.
void TxCoordinateConv::TransBursaWolfBessel2GRS80(TxBursaWolf& oBursaWolf, LDouble inCar_X, LDouble inCar_Y, LDouble inCar_Z, LDouble& outCar_X, LDouble& outCar_Y, LDouble& outCar_Z)
{
	LDouble	ds	=	oBursaWolf.ds	*	0.000001;
	LDouble	dx	=	oBursaWolf.dX;
	LDouble	dy	=	oBursaWolf.dY;
	LDouble	dz	=	oBursaWolf.dZ;

	LDouble	omega	=	TxMath::Degree2Radian(oBursaWolf.omega / 3600.0);
	LDouble	phi		=	TxMath::Degree2Radian(oBursaWolf.phi   / 3600.0);
	LDouble	kappa	=	TxMath::Degree2Radian(oBursaWolf.kappa / 3600.0);

	LDouble	x	=		inCar_X;
	LDouble	y	=		inCar_Y;
	LDouble	z	=		inCar_Z;

	outCar_X	=	(1 + ds ) * (	x	 +	kappa  *    y     -	phi    *  z)	+	dx;	
	outCar_Y	=	(1 + ds ) * (-kappa  *    x	   +	y	  +	omega  *  z)	+	dy;
	outCar_Z	=	(1 + ds ) * (	phi  *    x	   -	omega *     y  +  z)	+	dz;
}

//세계측지계 -> 기존측지계
void TxCoordinateConv::TransBursaWolfGRS802Bessel(TxBursaWolf& oBursaWolf, LDouble inCar_X, LDouble inCar_Y, LDouble inCar_Z, LDouble& outCar_X, LDouble& outCar_Y, LDouble& outCar_Z)
{
	LDouble	ds	=	1.0 * oBursaWolf.ds	*	0.000001;
	LDouble	dx	=	1.0 * oBursaWolf.dX;
	LDouble	dy	=	1.0 * oBursaWolf.dY;
	LDouble	dz	=	1.0 * oBursaWolf.dZ;

	LDouble	omega	=	1.0 * TxMath::Degree2Radian(oBursaWolf.omega / 3600.0);
	LDouble	phi		=	1.0 * TxMath::Degree2Radian(oBursaWolf.phi   / 3600.0);
	LDouble	kappa	=	1.0 * TxMath::Degree2Radian(oBursaWolf.kappa / 3600.0);

	LDouble	x	=		inCar_X - dx;
	LDouble	y	=		inCar_Y - dy;
	LDouble	z	=		inCar_Z - dz;
	outCar_X  = 1.0 / (1.0 + ds) * (	x	           -    kappa  *    y   +	phi    *  z);
	outCar_Y  = 1.0 / (1.0 + ds) * (  kappa  *    x	   +	  y	            -	omega  *  z);
	outCar_Z  = 1.0 / (1.0 + ds) * (  -phi   *    x	   +	omega *     y   +       z      ); 
}


// 2003년 국토지리정보원 고시 제2003-497호에 신규 고시됨
// 기존 BursaWolf -> Molodensky-Badekas 모델로 변환 및 국가좌표변환계수 변경.
// 기존측지계로 부터 -> 세계측지계로의 변환.
/*
   구분       기준좌표(Bessel->GRS80) 
   ------------------------------------
   X0         -3,159,521.31
   Y0         4,068,151.32
   Z0         3,748,113.85
   
   구분       기준좌표(GRS80->Bessel)
   ------------------------------------
   X0         -3,159,666.86
   Y0         4,068,655.70
   Z0         3,748,799.65
   ------------------------------------
*/
void TxCoordinateConv::TransMolodenskyBessel2GRS80(LDouble inCar_X, LDouble inCar_Y, LDouble inCar_Z, LDouble& outCar_X, LDouble& outCar_Y, LDouble& outCar_Z)
{	
	LDouble X0	=	-3159521.31;
	LDouble	Y0  =	4068151.32;
	LDouble	Z0  =   3748113.85;
	
	LDouble	ds	=	m_oMolodensky.ds	*	0.000001;
	LDouble	dx	=	m_oMolodensky.dX;
	LDouble	dy	=	m_oMolodensky.dY;
	LDouble	dz	=	m_oMolodensky.dZ;

	LDouble	omega	=	TxMath::Degree2Radian(m_oMolodensky.omega / 3600.0);
	LDouble	phi		=	TxMath::Degree2Radian(m_oMolodensky.phi   / 3600.0);
	LDouble	kappa	=	TxMath::Degree2Radian(m_oMolodensky.kappa / 3600.0);

	LDouble	x	=		inCar_X - X0;
	LDouble	y	=		inCar_Y - Y0;
	LDouble	z	=		inCar_Z - Z0;

	outCar_X	=	X0 + dx + (1.0 + ds ) * (	x	        +	 kappa * y  -	phi    *  z) ;
	outCar_Y	=	Y0 + dy + (1.0 + ds ) * (-kappa * x     +	   y	    +	omega  *  z) ;
	outCar_Z	=	Z0 + dz + (1.0 + ds ) * (	phi * x     -	 omega * y  +   z          ) ;	
}

//세계측지계 -> 기존측지계
void TxCoordinateConv::TransMolodenskyGRS802Bessel(LDouble inCar_X, LDouble inCar_Y, LDouble inCar_Z, LDouble& outCar_X, LDouble& outCar_Y, LDouble& outCar_Z)
{	
	LDouble X0	=	-3159666.86;
	LDouble	Y0  =	4068655.70;
	LDouble	Z0  =   3748799.65;
	
	LDouble	ds	=	1.0  * m_oMolodensky.ds	*	0.000001;
	LDouble	dx	=	-1.0 * m_oMolodensky.dX;
	LDouble	dy	=	-1.0 * m_oMolodensky.dY;
	LDouble	dz	=	-1.0 * m_oMolodensky.dZ;

	LDouble	omega	=	1.0 * TxMath::Degree2Radian(m_oMolodensky.omega / 3600.0);
	LDouble	phi		=	1.0 * TxMath::Degree2Radian(m_oMolodensky.phi   / 3600.0);
	LDouble	kappa	=	1.0 * TxMath::Degree2Radian(m_oMolodensky.kappa / 3600.0);

	LDouble	x	=		inCar_X - X0;
	LDouble	y	=		inCar_Y - Y0;
	LDouble	z	=		inCar_Z - Z0;
	
	outCar_X  = X0 + dx + 1.0 / (1.0 + ds) * (	x	       -	 kappa * y  +	phi    *  z) ;
	outCar_Y  = Y0 + dy + 1.0 / (1.0 + ds) * ( kappa * x   +	   y	    -	omega  *  z) ;
	outCar_Z  = Z0 + dz + 1.0 / (1.0 + ds) * ( -phi   * x  +	 omega * y  +   z          ) ;			
}


LDouble TxCoordinateConv::fnSPHTMD(LDouble ap, LDouble bp, LDouble cp, LDouble dp, LDouble ep,LDouble sphi)
{
//     *** TRUE MERIDIONAL DISTANCE FROM LATITUDE ***
//

	LDouble result;
	result = ap * sphi - bp * sin(2.0 * sphi) + 
			 cp * sin(4.0 * sphi) - dp * sin(6.0 * sphi) + ep * sin(8.0 * sphi);

	return result;

}

// 모유곡선의 곡률반경
LDouble TxCoordinateConv::fnSPHSN(LDouble a, LDouble es, LDouble sphi)
{
//
//     *** RADIUS OF CURVATURE IN THE PRIME VERTICAL FROM LATITUDE ***
//
	LDouble result;
	result = a / sqrt((LDouble)1.0 - es * pow(sin(sphi), (LDouble)2.0));

	return result;
}


LDouble TxCoordinateConv::fnSPHSR(LDouble a, LDouble es, LDouble sphi)
{
//     *** RADIUS OF CURVATURE IN THE MERIDIAN FROM LATITUDE ***
//
	LDouble	result;

	result = a * (1.0 - es) / pow(fnDENOM(es, sphi), 3);
	
	return result;
}

LDouble TxCoordinateConv::fnDENOM(LDouble es, LDouble sphi)
{
//     *** RADIUS OF CURVATURE IN THE MERIDIAN FROM LATITUDE ***
//
	LDouble	result;

	result = sqrt(1.0 - es * pow(sin(sphi), 2));

	return result;
}


void TxCoordinateConv::XYCoordinate2GeoCoordinate(TxEllipsoid& oEllipsoid, TxGeoSystem& oGeoSystem, LDouble X, LDouble Y, LDouble& geoLan, LDouble& geoLon)
{
	LDouble Xn = Y;
	LDouble Ye = X;
	// 투영원점 위도, 경도
	LDouble  phi0    =  TxMath::Degree2Radian(oGeoSystem.latCenter);
	LDouble  Lambda0 =  TxMath::Degree2Radian(oGeoSystem.lonCenter);

	// 타원체 장반경
	LDouble  a = oEllipsoid.a;
	// 타원체 단반경
	LDouble  b = oEllipsoid.b;
	// 타원체 편평률
	LDouble  f = oEllipsoid.f;

	// 원점축척계수
	LDouble  k0 = oGeoSystem.sfactor;

	// Y축(East) 원점 가산값
	LDouble  deltaY = oGeoSystem.falseEast;
	// X축(North) 원점 가산값
	LDouble  deltaX = oGeoSystem.falseNorth;

	// 제1이심률
	LDouble  e   = oEllipsoid.e;
	LDouble  e2  = oEllipsoid.e2;
	LDouble  e4  = pow(e, (LDouble)4.0);
	LDouble  e6  = pow(e, (LDouble)6.0);

	LDouble  M0 = oEllipsoid.M(oGeoSystem.latCenter);
	LDouble  M  = M0 + (Xn - deltaX) / k0;
	LDouble mu1 = M / (a * (1.0 - e2/4.0-3.0*e4/64.0-5.0*e6/256.0));

	LDouble  e1   = (1.0 - sqrt((LDouble)(1-e2))) / (1.0 + sqrt((LDouble)(1-e2)));
	LDouble  e12  = pow(e1, (LDouble)2.0);
	LDouble  e13  = pow(e1, (LDouble)3.0);
	LDouble  e14  = pow(e1, (LDouble)4.0);

	LDouble  phi1 = mu1 + 
						(3.0 * e1 / 2.0     - 27.0 * e13 / 32.0) * sin(2.0 * mu1) +
						(21.0 * e12 / 16.0  - 55.0 * e14 / 32.0) * sin(4.0 * mu1) + 
						(151.0 * e13 / 96.0)                     * sin(6.0 * mu1) + 
						(1097.0 * e14 / 512.0)                   * sin(8.0 * mu1);


	LDouble  R1   = (a * (1.0 - e2)) /
		            pow((1.0 - e2*pow(sin(phi1), (LDouble)2.0)), (LDouble)1.5);

	//제2이심률
	LDouble  ed2  = (pow(a, (LDouble)2.0) - pow(b, (LDouble)2.0)) / pow(b, (LDouble)2.0);

	LDouble  C1   = ed2 * pow(cos(phi1), (LDouble)2.0);
	LDouble  C12  = pow(C1, (LDouble)2.0);

	LDouble  T1   = pow(tan(phi1), (LDouble)2.0);
	LDouble  T12  = pow(T1, (LDouble)2.0);

	LDouble  N1   = a / sqrt(1.0 - e2 * pow(sin(phi1), (LDouble)2.0));

	LDouble  D    = (Ye - deltaY) / (N1 * k0);
	LDouble  D2   = pow(D, (LDouble)2.0);
	LDouble  D3   = pow(D, (LDouble)3.0);
	LDouble  D4   = pow(D, (LDouble)4.0);
	LDouble  D5   = pow(D, (LDouble)5.0);
	LDouble  D6   = pow(D, (LDouble)6.0);

	geoLan  = phi1 - (N1 * tan(phi1) / R1) * ( D2/2.0 - 
											   D4/24.0 *  (5.0  + 3.0*T1  + 10.0*C1  - 4.0*C12  - 9.0*ed2) +
											   D6/720.0 * (61.0 + 90.0*T1 + 298.0*C1 + 45.0*T12 - 252.0*ed2 - 3.0*C12)
											 );
	geoLon  = Lambda0 + 1.0 / cos(phi1)    * ( D - D3/6.0   * (1.0 + 2.0*T1 + C1) +
											       D5/120.0 * (5.0 - 2.0*C1 + 28.0*T1 - 3.0*C12 + 8.0*ed2 + 24.0*T12)		                                       
		                                     );
	geoLan  = TxMath::Radian2Degree(geoLan, false);
	geoLon  = TxMath::Radian2Degree(geoLon, false);
}
// 지심좌표계 -> 타원체의 경위도 변환
void TxCoordinateConv::CartesianCoordinate2Geographic(TxEllipsoid& oEllipse, LDouble car_X, LDouble car_Y, LDouble car_Z, LDouble&	rGeoLan, LDouble& rGeoLon, LDouble& h)
{
	LDouble  Xg = car_X;
	LDouble  Yg = car_Y;
	LDouble  Zg = car_Z;

	// 타원체 장반경
	LDouble  a = oEllipse.a;
	// 타원체 단반경
	LDouble  b = oEllipse.b;
	// 타원체 편평률
	LDouble  f = oEllipse.f;

	LDouble  Xg2 = pow(Xg, (LDouble)2.0);
	LDouble  Yg2 = pow(Yg, (LDouble)2.0);
	
	LDouble  p = sqrt(Xg2 + Yg2);
	LDouble  theta = atan((Zg*a) / (p*b));

	// 제1이심률
	LDouble  e   = oEllipse.e;
	LDouble  e2  = oEllipse.e2;

	//제2이심률
	LDouble  ed2  = (pow(a, (LDouble)2.0) - pow(b, (LDouble)2.0)) / pow(b, (LDouble)2.0);

	LDouble  Phig   = atan((Zg + ed2*b* pow(sin(theta), (LDouble)3.0)) / (p-a*e2*pow(cos(theta), (LDouble)3.0)));
	LDouble  Lamdag = atan(Yg / Xg);
	LDouble  hg     = 0.0; //p*cos(theta) + Zg*sin(Phig)-pow(a, (LDouble)2.0) / N;

	rGeoLan  = TxMath::Radian2Degree(Phig, false);
	rGeoLon  = TxMath::Radian2Degree(Lamdag, false);
	h = hg;

	if (car_X < 0)
		rGeoLon	+= 180.0;

	if (rGeoLon < 0)
		rGeoLon	+=	360.0;
}


void TxCoordinateConv::GeoCoordinate2XYCoordinate(TxEllipsoid& oEllipsoid, TxGeoSystem& oGeoSystem, LDouble geoLan, 
													 LDouble geoLon, LDouble& X, LDouble& Y)
{
	// 평면 직각좌표->측량의 표현 방법으로 변경
	// 위도, 경도
	LDouble  phi     =  TxMath::Degree2Radian(geoLan);
	LDouble  Lambda  =  TxMath::Degree2Radian(geoLon);
	
	// 투영원점 위도, 경도
	LDouble  phi0    =  TxMath::Degree2Radian(oGeoSystem.latCenter);
	LDouble  Lambda0 =  TxMath::Degree2Radian(oGeoSystem.lonCenter);

	// 타원체 장반경
	LDouble  a = oEllipsoid.a;
	// 타원체 단반경
	LDouble  b = oEllipsoid.b;
	// 타원체 편평률
	LDouble  f = oEllipsoid.f;

	// 원점축척계수
	LDouble  k0 = oGeoSystem.sfactor;

	// Y축(East) 원점 가산값
	LDouble  deltaY = oGeoSystem.falseEast;
	// X축(North) 원점 가산값
	LDouble  deltaX = oGeoSystem.falseNorth;

	// 제1이심률
	LDouble  e   = oEllipsoid.e;
	LDouble  e2  = oEllipsoid.e2;
	LDouble  e4  = pow(e, (LDouble)4.0);
	LDouble  e6  = pow(e, (LDouble)6.0);

	//제2이심률
	LDouble  ed2  = (pow(a, (LDouble)2.0) - pow(b, (LDouble)2.0)) / pow(b, (LDouble)2.0);

	LDouble  T  = pow(tan(phi), (LDouble)2.0);
	LDouble  T2 = pow(T, (LDouble)2.0);

	LDouble  C  = e2 / ((LDouble)1.0-e2) * pow(cos(phi), (LDouble)2.0);
	LDouble  C2 = pow(C, (LDouble)2.0);

	LDouble  A  = (Lambda -Lambda0) * cos(phi);
	LDouble  A2 = pow(A, (LDouble)2.0);
	LDouble  A3 = pow(A, (LDouble)3.0);
	LDouble  A4 = pow(A, (LDouble)4.0);
	LDouble  A5 = pow(A, (LDouble)5.0);
	LDouble  A6 = pow(A, (LDouble)6.0);
	
	LDouble  N  = oEllipsoid.N(geoLan);
	LDouble  M  = oEllipsoid.M(geoLan);
	LDouble  M0 = oEllipsoid.M(oGeoSystem.latCenter);
	
	X = deltaY + k0 * N * (
		                     A + A3/6.0   * (1.0 - T + C) + 
								 A5/120.0 * (5.0 - 18.0 * T + T2 + 72.0 * C -58.0 * ed2) 
						  );
    Y = deltaX + k0 *     (
							 M - M0 + N * tan(phi) * (A2 / (LDouble)2.0 + A4 / 24.0 * (5.0 - T + 9.0 * C + 4.0 * C2) +
							 A6 / 720.0 * (61.0 - 58.0 * T + T2 + 600 * C - 330.0 * ed2))
						  );
}

void TxCoordinateConvert::Katec2WGS84( double x, double y, double& rLatitude, double& rLongitude )
{
    TxGRS80   GRS80Ellipsoid;
    TxBessel  BesselEllipsoid;
    TxTMKorea oTM(KorTmKatec);

    TxCoordinateConv conv;

    // 1, KATEC -> BESSEL 경위도
    LDouble lon, lat;
    conv.XYCoordinate2GeoCoordinate(BesselEllipsoid, oTM, x, y, lat, lon);

    // 2, BESSEL -> 3차원 직각좌표계
    LDouble wx, wy, wz;
    conv.GeoCoordinate2Cartesian(BesselEllipsoid, lat, lon, 0.0, wx, wy, wz);

    // 3, BESSEL -> GRS80 지심좌표계 변환
    LDouble gx, gy, gz;
    conv.TransMolodenskyBessel2GRS80(wx, wy, wz, gx, gy, gz);

    // 4, GRS80 지심좌표계 -> GRS80경위도 
    LDouble gLon, gLat, gh;
    conv.CartesianCoordinate2Geographic(GRS80Ellipsoid, gx, gy, gz, gLat, gLon, gh);

    rLongitude = gLon;
    rLatitude  = gLat;
}

void TxCoordinateConvert::WGS842Katec(double lat, double lon, double& rx, double& ry)
{
    TxGRS80   GRS80Ellipsoid;
    TxBessel  BesselEllipsoid;
    TxTMKorea oTM(KorTmKatec);

    TxCoordinateConv conv;

    // 1. GRS80 경위도 -> GRS80 지심좌표계
    LDouble gx, gy, gz;
    conv.GeoCoordinate2Cartesian(GRS80Ellipsoid, lat, lon, 0.0, gx, gy, gz);

    // 2. GRS80 -> BESSEL 지심좌표계 변환
    LDouble bx, by, bz;
    conv.TransMolodenskyGRS802Bessel(gx, gy, gz, bx, by, bz);

    // 3. BESSEL 지심좌표 -> BESSEL 경위도
    LDouble bLon, bLat, bh;
    conv.CartesianCoordinate2Geographic(BesselEllipsoid, bx, by, bz, bLat, bLon, bh);

    // 4. BESSEL 경위도 -> KATEC
    LDouble x, y;
    conv.GeoCoordinate2XYCoordinate(BesselEllipsoid, oTM, bLat, bLon, x, y);

    rx = x;
    ry = y;
}

void TxCoordinateConvert::WGS842UTMK( double lat, double lon, double& rx, double& ry )
{
    TxGRS80   oGRS80Ellipsoid;	
    TxTMKorea oUTMK(KorUTMK);
    TxCoordinateConv oConv;

    // 1. WGS84 경위도 -> UTM-K
    LDouble tx, ty;
    oConv.GeoCoordinate2XYCoordinate(oGRS80Ellipsoid, oUTMK, lat, lon, tx, ty);

    rx = tx;
    ry = ty;
}

void TxCoordinateConvert::UTMK2WGS84( double x, double y, double& rLatitude, double& rLongitude )
{
    TxGRS80   oGRS80Ellipsoid;	
    TxTMKorea oUTMK(KorUTMK);
    TxCoordinateConv oConv;

    LDouble gLon, gLat;
    oConv.XYCoordinate2GeoCoordinate(oGRS80Ellipsoid, oUTMK, x, y, gLat, gLon);

    rLatitude  = gLat;
    rLongitude = gLon;
}

void TxCoordinateConvert::WGS842UTMKMid( double lat, double lon, double& rx, double& ry )
{
    TxGRS80   oGRS80Ellipsoid;	
    TxTMKorea oUTMK(KorUTMKMid);
    TxCoordinateConv oConv;

    // 1. WGS84 경위도 -> UTM-K
    LDouble tx, ty;
    oConv.GeoCoordinate2XYCoordinate(oGRS80Ellipsoid, oUTMK, lat, lon, tx, ty);

    rx = tx;
    ry = ty;
}

void TxCoordinateConvert::UTMKMid2WGS84( double x, double y, double& rLatitude, double& rLongitude )
{
    TxGRS80   oGRS80Ellipsoid;	
    TxTMKorea oUTMK(KorUTMKMid);
    TxCoordinateConv oConv;

    LDouble gLon, gLat;
    oConv.XYCoordinate2GeoCoordinate(oGRS80Ellipsoid, oUTMK, x, y, gLat, gLon);

    rLatitude  = gLat;
    rLongitude = gLon;
}

double TxCoordinateConvert::NMEA2Degree( double dValue )
{
    double dDo  = (double) ((int) (dValue / 100.0));
    double dBun = dValue - dDo * 100.0;

    double dR   = dDo + dBun / 60.0;
    return dR;
}


/*
BOOL KATECtoWGS84(double x, double y)
{
CxGRS80 GRS80e;
CxBessel Bessele;
CxTMKorea oTM(korTmKatec);

CxCoordinateConv conv;

// 1, KATEC -> BESSEL 경위도
LDouble lon, lat;
conv.xXYCoordinate2GeoCoordinate(Bessele, oTM, x, y, lat, lon);

// 2, BESSEL -> 3차원 직각좌표계
LDouble wx, wy, wz;
conv.xGeoCoordinate2Cartesian(Bessele, lat, lon, 0.0, wx, wy, wz);

// 3, BESSEL -> GRS80 지심좌표계 변환
LDouble gx, gy, gz;
conv.xTransMolodenskyBessel2GRS80(wx, wy, wz, gx, gy, gz);

// 4, GRS80 지심좌표계 -> GRS80경위도 
LDouble gLon, gLat, gh;
conv.xCarCoordinate2Geographic(GRS80e, gx, gy, gz, gLat, gLon, gh);

m_dWgsLon = gLon;
m_dWgsLat = gLat;
return TRUE;
}

BOOL TMtoWGS84(double x, double y, long base)
{
CxGRS80 GRS80e;
CxBessel Bessele;
CxTMKorea oTM((xEnumGeoSystem)base);

CxCoordinateConv conv;

// 1, TM -> BESSEL 경위도
LDouble lon, lat;
conv.xXYCoordinate2GeoCoordinate(Bessele, oTM, x, y, lat, lon);

// 2, BESSEL -> 3차원 직각좌표계
LDouble wx, wy, wz;
conv.xGeoCoordinate2Cartesian(Bessele, lat, lon, 0.0, wx, wy, wz);

// 3, BESSEL -> GRS80 지심좌표계 변환
LDouble gx, gy, gz;
conv.xTransMolodenskyBessel2GRS80(wx, wy, wz, gx, gy, gz);

// 4, GRS80 지심좌표계 -> GRS80경위도 
LDouble gLon, gLat, gh;
conv.xCarCoordinate2Geographic(GRS80e, gx, gy, gz, gLat, gLon, gh);

m_dWgsLon = gLon;
m_dWgsLat = gLat;
return TRUE;
}

BOOL WGS84toKATEC(double lon, double lat)
{
CxGRS80 GRS80e;
CxBessel Bessele;
CxTMKorea oTM(korTmKatec);

CxCoordinateConv conv;

// 1. GRS80 경위도 -> GRS80 지심좌표계
LDouble gx, gy, gz;
conv.xGeoCoordinate2Cartesian(GRS80e, lat, lon, 0.0, gx, gy, gz);

// 2. GRS80 -> BESSEL 지심좌표계 변환
LDouble bx, by, bz;
conv.xTransMolodenskyGRS802Bessel(gx, gy, gz, bx, by, bz);

// 3. BESSEL 지심좌표 -> BESSEL 경위도
LDouble bLon, bLat, bh;
conv.xCarCoordinate2Geographic(Bessele, bx, by, bz, bLat, bLon, bh);

// 4. BESSEL 경위도 -> KATEC
LDouble x, y;
conv.xGeoCoordinate2XYCoordinate(Bessele, oTM, bLat, bLon, x, y);

m_dwKax = x;
m_dwKay = y;

return TRUE;
}

BOOL WGS84toTM(double lon, double lat, long base)
{
CxGRS80 GRS80e;
CxBessel Bessele;
CxTMKorea oTM((xEnumGeoSystem)base);

CxCoordinateConv conv;

// 1. GRS80 경위도 -> GRS80 지심좌표계
LDouble gx, gy, gz;
conv.xGeoCoordinate2Cartesian(GRS80e, lat, lon, 0.0, gx, gy, gz);

// 2. GRS80 -> BESSEL 지심좌표계 변환
LDouble bx, by, bz;
conv.xTransMolodenskyGRS802Bessel(gx, gy, gz, bx, by, bz);

// 3. BESSEL 지심좌표 -> BESSEL 경위도
LDouble bLon, bLat, bh;
conv.xCarCoordinate2Geographic(Bessele, bx, by, bz, bLat, bLon, bh);

// 4. BESSEL 경위도 -> KATEC
LDouble x, y;
conv.xGeoCoordinate2XYCoordinate(Bessele, oTM, bLat, bLon, x, y);

m_dwTmx = x;
m_dwTmy = y;

return TRUE;
}

BOOL TMtoKATEC(double tx, double ty, long base)
{
CxBessel Bessele;
CxTMKorea oKATEC(korTmKatec);
CxTMKorea oTM((xEnumGeoSystem)base);
CxCoordinateConv conv;

// 1. TM -> BESSEL 경위도
LDouble bLon, bLat;
conv.xXYCoordinate2GeoCoordinate(Bessele, oTM, tx, ty, bLat, bLon);

// 2. BESSEL 경위도 -> KATEC
LDouble kx, ky;
conv.xGeoCoordinate2XYCoordinate(Bessele, oKATEC, bLat, bLon, kx, ky);

m_dwKax = kx;
m_dwKay = ky;

return TRUE;

}

BOOL KATECtoTM(double kx, double ky, long base)
{
CxBessel Bessele;
CxTMKorea oKATEC(korTmKatec);
CxTMKorea oTM((xEnumGeoSystem)base);
CxCoordinateConv conv;

// 1. KATEC -> BESSEL 경위도
LDouble bLon, bLat;
conv.xXYCoordinate2GeoCoordinate(Bessele, oKATEC, kx, ky, bLat, bLon);

// 2. BESSEL 경위도 -> TM
LDouble tx, ty;
conv.xGeoCoordinate2XYCoordinate(Bessele, oTM, bLat, bLon, tx, ty);

m_dwTmx = tx;
m_dwTmy = ty;

return TRUE;
}

BOOL WGS84toUTMK(double lon, double lat)
{
CxGRS80 GRS80e;	
CxTMKorea oUTMK(korUTMK);
CxCoordinateConv conv;

// 1. WGS84 경위도 -> UTM-K
LDouble tx, ty;
conv.xGeoCoordinate2XYCoordinate(GRS80e, oUTMK, lat, lon, tx, ty);

m_xUTMK = tx;
m_yUTMK = ty;

return TRUE;
}

BOOL UTMKtoWGS84(double x, double y)
{
CxGRS80 GRS80e;
CxTMKorea oUTMK(korUTMK);
CxCoordinateConv conv;

// 1. KATEC -> BESSEL 경위도
LDouble gLon, gLat;
conv.xXYCoordinate2GeoCoordinate(GRS80e, oUTMK, x, y, gLat, gLon);

m_dWgsLon = gLon;
m_dWgsLat = gLat;
return TRUE;
}

*/