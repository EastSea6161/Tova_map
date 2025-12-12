#pragma once
#include "IncludeQBicDataStructure.h"

class QBic2DMatrix
{
public:
	QBic2DMatrix();
	QBic2DMatrix(int nRow, int nCol);
	QBic2DMatrix(const QBic2DMatrix &p);
	QBic2DMatrix(std::vector<std::vector<double>> vecMatrix);
	~QBic2DMatrix(void);

protected:
	int m_nRow;
	int m_nCol;

public:
	//주의 : Vector Index는 0부터 시작함
	std::vector<std::vector<double>> m_vecMatrix;

public:
	void Clear();
	void Clear(int nRow, int nCol);
	bool Multiply(QBic2DMatrix right, QBic2DMatrix& oResult);
	

	bool AddData(int nRow, int nCol, double dValue);
	void AddRow(double dValue = 0);
	void AddCol(double dValue = 0);

	double GetMatrixData(int nRow, int nCol);
	int    GetRowCnt();
	int	   GetColCnt();


	void CreateVecterToOneRowMatrix(std::vector<double> vecData);
	void CreateVecterToOneColumnMatrix(std::vector<double> vecData);
	

public:
	void MinusUpToZeroAllData();
	
};

