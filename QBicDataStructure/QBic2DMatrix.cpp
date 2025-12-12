#include "QBic2DMatrix.h"


QBic2DMatrix::QBic2DMatrix(void)
{
	m_nRow = 0;
	m_nCol = 0;
	m_vecMatrix.clear();
}

QBic2DMatrix::QBic2DMatrix( int nRow, int nCol )
{
	m_nRow = nRow;
	m_nCol = nCol;
	m_vecMatrix.clear();
	m_vecMatrix.assign(nRow, std::vector<double>(nCol, 0));
}

QBic2DMatrix::QBic2DMatrix( const QBic2DMatrix &p )
{
	m_nRow = p.m_nRow;
	m_nCol = p.m_nCol;
	m_vecMatrix.clear();
	m_vecMatrix = p.m_vecMatrix;
}

QBic2DMatrix::QBic2DMatrix( std::vector<std::vector<double>> vecMatrix )
{
	m_nRow = 0;
	m_nCol = (int)vecMatrix.size();

	if (m_nCol > 0)
	{
		m_nRow = (int)vecMatrix[0].size();
	}

	m_vecMatrix.clear();
	m_vecMatrix = vecMatrix;
}

QBic2DMatrix::~QBic2DMatrix(void)
{

}

bool QBic2DMatrix::Multiply( QBic2DMatrix right, QBic2DMatrix& oResult )
{
	if (m_nCol == 0|| m_nRow == 0 || right.m_nCol == 0 || right.m_nRow == 0)
	{
		return false;
	}

	if (m_nCol != right.m_nRow)
	{
		return false;
	}

	oResult.Clear(m_nRow, right.m_nCol);

	for (size_t i = 0; i < m_nRow; i++)
	{
		for (size_t j = 0; j < m_nCol; j++)
		{
			for (size_t k = 0; k < right.m_nCol; k++)
			{
				oResult.m_vecMatrix[i][k] += m_vecMatrix[i][j] * right.m_vecMatrix[j][k];
			}
		}
	}

	return true;
}

void QBic2DMatrix::Clear()
{
	this->m_nCol = 0;
	this->m_nRow = 0;
	this->m_vecMatrix.clear();
}

void QBic2DMatrix::Clear(int nRow, int nCol)
{
	m_nRow = nRow;
	m_nCol = nCol;
	m_vecMatrix.clear();
	m_vecMatrix.assign(nRow, std::vector<double>(nCol, 0));
}

bool QBic2DMatrix::AddData( int nRow, int nCol, double dValue )
{
	if (nRow >= m_nRow)
	{
		return false;
	}

	if (nCol >= m_nCol)
	{
		return false;
	}

	m_vecMatrix[nRow][nCol] = dValue;

	return true;
}

void QBic2DMatrix::AddRow( double dValue /*= 0*/ )
{
	m_nRow = m_nRow + 1;

	std::vector<double> vecRow(m_nCol, dValue);
	m_vecMatrix.push_back(vecRow);
}

void QBic2DMatrix::AddCol( double dValue /*= 0*/ )
{
	m_nCol = m_nCol + 1;

	for (int i = 0; i < m_nRow; i++)
	{
		std::vector<double>& vecCol = m_vecMatrix[i];
		vecCol.push_back(dValue);
	}
}

void QBic2DMatrix::CreateVecterToOneRowMatrix( std::vector<double> vecData )
{
	Clear();
	m_nRow = 1;
	m_nCol = (int)vecData.size();
	m_vecMatrix.push_back(vecData);
}

void QBic2DMatrix::CreateVecterToOneColumnMatrix( std::vector<double> vecData )
{
	Clear();
	m_nCol = 1;
	m_nRow = (int)vecData.size();

	for (size_t i = 0; i < vecData.size(); i++)
	{
		double dValue = vecData[i];
		std::vector<double> vecTemp;
		vecTemp.push_back(dValue);
		m_vecMatrix.push_back(vecTemp);
	}
}

int QBic2DMatrix::GetRowCnt()
{
	return m_nRow;
}

int QBic2DMatrix::GetColCnt()
{
	return m_nCol;
}

double QBic2DMatrix::GetMatrixData( int nRow, int nCol )
{
	if (nRow >= m_nRow)
	{
		return 0.0;
	}

	if (nCol >= m_nCol)
	{
		return 0.0;
	}

	return m_vecMatrix[nRow][nCol];
}

void QBic2DMatrix::MinusUpToZeroAllData()
{
	for (size_t i = 0; i < m_vecMatrix.size(); i++)
	{
		std::vector<double>& vecCol = m_vecMatrix[i];

		for (size_t j = 0; j < vecCol.size(); j++)
		{
			double& dValue = vecCol[j];

			if (dValue < 0)
			{
				dValue = 0;
			}
		}
	}
}
