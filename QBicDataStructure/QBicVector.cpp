#include "QBicVector.h"
#include <cmath>
#include "QBicMath.h"

QBicVector::QBicVector(void)
{
    i = 0.0;
    j = 0.0;
    k = 0.0;
}

QBicVector::QBicVector( double ii, double jj, double kk/*=0*/ )
{
    i = ii;
    j = jj;
    k = kk;
}

QBicVector::QBicVector( const QBicVector& right )
{
    i = right.i;
    j = right.j;
    k = right.k;
}

QBicVector::~QBicVector(void)
{
}

QBicVector QBicVector::operator=( const QBicVector& right )
{	
    i = right.i;
    j = right.j;
    k = right.k;
    return *this;
}

QBicVector QBicVector::operator+( const QBicVector & right )
{	
    QBicVector result;
    result.i = i + right.i;
    result.j = j + right.j;
    result.k = k + right.k;

    return result;
}

QBicVector QBicVector::operator-( const QBicVector & right )
{	
    QBicVector result;
    result.i = i - right.i;
    result.j = j - right.j;
    result.k = k - right.k;

    return result;
}

bool QBicVector::operator==( const QBicVector& right ) 
{
    return i == right.i && j == right.j && k == right.k;
}


void QBicVector::Set( double ii, double jj, double kk/*=0.0*/ )
{
    i = ii;
    j = jj;
    k = kk;
}

void QBicVector::SetI( double ii )
{
    i = ii;
}

void QBicVector::SetJ( double jj )
{
    j = jj;
}

void QBicVector::SetK( double kk )
{
    k = kk;
}

double QBicVector::GetI()
{
    return i;
}

double QBicVector::GetJ()
{
    return j;
}

double QBicVector::GetK()
{
    return k;
}

void QBicVector::Normalize()
{
    double dLength = sqrt( pow( i, 2 ) + pow( j, 2 ) + pow( k, 2 ) );
    if( dLength <= 0 )
        return;

    i = i/dLength;
    j = j/dLength;
    k = k/dLength;
}

double QBicVector::InnerProduct( const QBicVector& right )
{
    return i*right.i + j*right.j + k*right.k;
}

QBicVector QBicVector::OuterProduct( const QBicVector& right )
{
    QBicVector result;

    result.i = j*right.k - k*right.j;
    result.j = k*right.i - i*right.k;
    result.k = i*right.j - j*right.i;

    result.Normalize();
    return result;
}

double QBicVector::Length()
{
    double dLength = sqrt( pow( i, 2 ) + pow( j, 2 ) + pow( k, 2 ) );
    return dLength;
}

void QBicVector::SetLength( double dLength )
{
    Normalize();
    i = i * dLength;
    j = j * dLength;
    k = k * dLength;
}

double QBicVector::AzimuthAngle()
{
    return atan2(i, j);
}

double QBicVector::AngleBetween( const QBicVector& left, const QBicVector& right )
{
    QBicVector oLeft(left);
    QBicVector oRight(right);

    oLeft.Normalize();
    oRight.Normalize();

    double dCosTheta = oLeft.InnerProduct(oRight);
    double dTheta    = acos(dCosTheta);
    return dTheta;
}

QBicVector QBicVector::Multiply( double dScalar, const QBicVector& right )
{
    QBicVector oResult;
    oResult.i = dScalar * right.i;
    oResult.j = dScalar * right.j;
    oResult.k = dScalar * right.k;

    return oResult;
}

void QBicVector::Multiply( double dScalar )
{
    i = dScalar * i;
    j = dScalar * j;
    k = dScalar * k;
}

void QBicVector::Negative()
{
    i = -1.0 * i;
    j = -1.0 * j;
    k = -1.0 * k;
}

void QBicVector::RotateClockwise( double a_dAngle, bool bDegree )
{    
    double dAngle(a_dAngle);

    if (bDegree) {
        dAngle = QBicMath::Mod(a_dAngle, 360.0);
        dAngle = QBicMath::Degree2Radian(-dAngle);
    }    
    else {
        dAngle = -dAngle;
    }

    Rotate(dAngle, false);
}

void QBicVector::RotateUnClockwise( double a_dAngle, bool bDegree/*=false*/ )
{
    Rotate(a_dAngle, bDegree);
}

void QBicVector::Rotate( double a_dAngle, bool bDegree )
{   
    double dAngle(a_dAngle);
    if (bDegree) {
        dAngle = QBicMath::Mod(a_dAngle, 360.0);
        dAngle = QBicMath::Degree2Radian(dAngle);
    } 

    //★ 반시계 방향 회전 공식

    double dX = cos(dAngle) * i - sin(dAngle) * j;
    double dY = sin(dAngle) * i + cos(dAngle) * j;

    i = dX;
    j = dY;
}

QBicVector QBicVector::PlusOper( const QBicVector& left, const QBicVector& right )
{
    QBicVector oResult;
    oResult.i = left.i + right.i;
    oResult.j = left.j + right.j;
    oResult.k = left.k + right.k;

    return oResult;
}

QBicVector QBicVector::MinusOper( const QBicVector& left, const QBicVector& right )
{
    QBicVector oResult;
    oResult.i = left.i - right.i;
    oResult.j = left.j - right.j;
    oResult.k = left.k - right.k;

    return oResult;
}

