#include "stdafx.h"
#include "TxVector.h"
#include <cmath>
#include "TxMath.h"

TxVector::TxVector(void)
{
    i = 0.0;
    j = 0.0;
    k = 0.0;
}

TxVector::TxVector( double ii, double jj, double kk/*=0*/ )
{
    i = ii;
    j = jj;
    k = kk;
}

TxVector::TxVector( const TxVector& right )
{
    i = right.i;
    j = right.j;
    k = right.k;
}

TxVector::~TxVector(void)
{
}

TxVector TxVector::operator=( const TxVector& right )
{	
    i = right.i;
    j = right.j;
    k = right.k;
    return *this;
}

TxVector TxVector::operator+( const TxVector & right )
{	
    TxVector result;
    result.i = i + right.i;
    result.j = j + right.j;
    result.k = k + right.k;

    return result;
}

TxVector TxVector::operator-( const TxVector & right )
{	
    TxVector result;
    result.i = i - right.i;
    result.j = j - right.j;
    result.k = k - right.k;

    return result;
}

bool TxVector::operator==( const TxVector& right ) 
{
    return i == right.i && j == right.j && k == right.k;
}


void TxVector::Set( double ii, double jj, double kk/*=0.0*/ )
{
    i = ii;
    j = jj;
    k = kk;
}

void TxVector::SetI( double ii )
{
    i = ii;
}

void TxVector::SetJ( double jj )
{
    j = jj;
}

void TxVector::SetK( double kk )
{
    k = kk;
}

double TxVector::GetI()
{
    return i;
}

double TxVector::GetJ()
{
    return j;
}

double TxVector::GetK()
{
    return k;
}

void TxVector::Normalize()
{
    double dLength = sqrt( pow( i, 2 ) + pow( j, 2 ) + pow( k, 2 ) );
    if( dLength <= 0 )
        return;

    i = i/dLength;
    j = j/dLength;
    k = k/dLength;
}

double TxVector::InnerProduct( const TxVector& right )
{
    return i*right.i + j*right.j + k*right.k;
}

TxVector TxVector::OuterProduct( const TxVector& right )
{
    TxVector result;

    result.i = j*right.k - k*right.j;
    result.j = k*right.i - i*right.k;
    result.k = i*right.j - j*right.i;

    result.Normalize();
    return result;
}

double TxVector::Length()
{
    double dLength = sqrt( pow( i, 2 ) + pow( j, 2 ) + pow( k, 2 ) );
    return dLength;
}

void TxVector::SetLength( double dLength )
{
    Normalize();
    i = i * dLength;
    j = j * dLength;
    k = k * dLength;
}

double TxVector::Azimuth()
{
    //★ 북쪽을 기준으로 시계방향
    return atan2(i, j);
}

double TxVector::AngleBetween( const TxVector& left, const TxVector& right )
{
    TxVector oLeft(left);
    TxVector oRight(right);

    oLeft.Normalize();
    oRight.Normalize();

    double dCosTheta = oLeft.InnerProduct(oRight);
    double dTheta    = acos(dCosTheta);
    return dTheta;
}

TxVector TxVector::Multiply( double dScalar, const TxVector& right )
{
    TxVector oResult;
    oResult.i = dScalar * right.i;
    oResult.j = dScalar * right.j;
    oResult.k = dScalar * right.k;

    return oResult;
}

void TxVector::Multiply( double dScalar )
{
    i = dScalar * i;
    j = dScalar * j;
    k = dScalar * k;
}

void TxVector::Negative()
{
    i = -1.0 * i;
    j = -1.0 * j;
    k = -1.0 * k;
}

void TxVector::RotateClockwise( double a_dAngle, bool bDegree )
{    
    double dAngle(a_dAngle);

    if (bDegree) {
        dAngle = TxMath::Mod(a_dAngle, 360.0);
        dAngle = TxMath::Degree2Radian(-dAngle);
    }    
    else {
        dAngle = -dAngle;
    }

    Rotate(dAngle, false);
}

void TxVector::RotateUnClockwise( double a_dAngle, bool bDegree/*=false*/ )
{
    Rotate(a_dAngle, bDegree);
}

void TxVector::Rotate( double a_dAngle, bool bDegree )
{    
    double dAngle(a_dAngle);

    if (bDegree) {
        dAngle = TxMath::Mod(a_dAngle, 360.0);
        dAngle = TxMath::Degree2Radian(dAngle);
    } 

    //★ 반시계 방향 회전 공식

    double dX = cos(dAngle) * i - sin(dAngle) * j;
    double dY = sin(dAngle) * i + cos(dAngle) * j;

    i = dX;
    j = dY;
}

TxVector TxVector::PlusOper( const TxVector& left, const TxVector& right )
{
    TxVector oResult;
    oResult.i = left.i + right.i;
    oResult.j = left.j + right.j;
    oResult.k = left.k + right.k;

    return oResult;
}

TxVector TxVector::MinusOper( const TxVector& left, const TxVector& right )
{
    TxVector oResult;
    oResult.i = left.i - right.i;
    oResult.j = left.j - right.j;
    oResult.k = left.k - right.k;

    return oResult;
}

