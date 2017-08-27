#include "testcasetable.h"
#include "public.h"
#include <cstdlib>
#include <ctime>
#include <cassert>

#define  QT_KEYPAD_NAVIGATION

CTestCaseTable::CTestCaseTable()
{
	m_bInit = false;
	setEditTriggers(QAbstractItemView::AllEditTriggers);
}

CTestCaseTable::~CTestCaseTable()
{

}

void CTestCaseTable::newTest()
{
	assert(m_bInit);

	genTestCases();

	m_nGrpNum = 1;
	
	m_nCorretRowCnt = 0;
	m_nWrongRowCnt = 0;

	loadData();

	//editItem(item(0,2));
	//setCurrentCell(0, 2);
}

void CTestCaseTable::initTable()
{
	// ��������������
	setColumnCount(3);
	setRowCount(CASE_CNT + 1);

	// ���ñ�ͷ
	QStringList header; 
	header << CN_CODEC("���") << CN_CODEC("����") << CN_CODEC("¼��");
	setHorizontalHeaderLabels(header); 
	verticalHeader()->hide();

	// ���õ�һ��Ϊ���
	int i = 0; for (; i < rowCount() - 1; ++i)
	{
		setItem(i, 0, new CItemCaseNum(i + 1));
		setItem(i, 1, new CItemTestCase(""));
	}
	setItem(i, 0, new CItemCaseNum(CN_CODEC("�ϼ�")));
	setItem(i, 1, new CItemTestCase(""));

	m_bInit = true;	
}

void CTestCaseTable::loadData()
{
	const TESTGRP& testGrp = m_testGrp[m_nGrpNum]; 
	for ( int i = 1; i <= CASE_CNT; ++i)
	{
		setItem(i-1, 1, new CItemTestCase(testGrp.sample[i]));
		setItem(i-1, 2, new CItemTestInput(testGrp.input[i]));
	}
	setCurrentCell(0, 2);
}

double CTestCaseTable::calcScore()
{
	// �ҵ���������һ��
	int nLastGrp = GRP_CNT;
	do
	{
		if (m_testGrp[nLastGrp].input == 0)
			break;
	}while (--nLastGrp);

	uint nLastCrpCorrectCnt = m_testGrp[nLastGrp].correctRowCnt;
	double dScore = nLastCrpCorrectCnt < 2 ? 0.0 : nLastCrpCorrectCnt * 0.2;
	for (int i = 1; i < nLastGrp; ++i)
	{
		// ȫ�Բŵ÷�
		if (m_testGrp[i].correctRowCnt == CASE_CNT)
			dScore += 5.0;
	}
	return dScore;
}

void CTestCaseTable::finish()
{
	QString strCurPath = QDir::currentPath();
	QString strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
	QString logFileName = strCurPath + "/" + strTime + ".txt";
	QFile log(logFileName);

	if (log.open(QIODevice::WriteOnly))
	{
		QTextStream out(&log);
		out << CN_CODEC("���\t\t") << CN_CODEC("�÷�\t\t") << CN_CODEC("׼ȷ��\t\t") << CN_CODEC("¼��\t\t") << CN_CODEC("��ȷ\t\t") << CN_CODEC("����\t\t") << "\n\r";

		// �ҵ���������һ��
		int nLastGrp = GRP_CNT;
		for ( ; nLastGrp > 0 && m_testGrp[nLastGrp].correctRowCnt + m_testGrp[nLastGrp].wrongRowCnt == 0; --nLastGrp);
		uint nLastCrpCorrectCnt = m_testGrp[nLastGrp].correctRowCnt;
		double dTotScore = nLastCrpCorrectCnt < 2 ? 0.0 : nLastCrpCorrectCnt * 0.2;
		for (int i = 1; i < nLastGrp; ++i)
		{
			const TESTGRP &grp = m_testGrp[i];
			double dScore;
			// �����һ�⣬������Ŀȫ�Բŵ÷�
			if (grp.correctRowCnt == CASE_CNT)
				dScore = 5.0;
			else
				dScore = 0.0;
			dTotScore += dScore;
			int nInputCnt = grp.correctRowCnt + grp.wrongRowCnt;
			if (nInputCnt)
				out << i << "\t\t\t" << dScore << "\t\t\t" << 1.0 * grp.correctRowCnt / nInputCnt << "\t\t\t" << nInputCnt << "\t\t\t" << grp.correctRowCnt << "\t\t\t" << grp.wrongRowCnt << "\n\r";
			else
				out << i << "\t\t\t" << dScore << "\t\t\t" << "0" << "\t\t\t" << nInputCnt << "\t\t\t" << grp.correctRowCnt << "\t\t\t" << grp.wrongRowCnt << "\n\r";
		}
		// �������һ��
		{
			const TESTGRP &grp = m_testGrp[nLastGrp];
			double dScore = 0.0;
			if (grp.correctRowCnt > 1)
				dScore = grp.correctRowCnt * 0.2;
			dTotScore += dScore;
			int nInputCnt = grp.correctRowCnt + grp.wrongRowCnt;
			if (nInputCnt)
				out << nLastGrp << "\t\t\t" << dScore << "\t\t\t" << 1.0 * grp.correctRowCnt / nInputCnt << "\t\t\t" << nInputCnt << "\t\t\t" << grp.correctRowCnt << "\t\t\t" << grp.wrongRowCnt << "\n\r";
			else
				out << nLastGrp << "\t\t\t" << dScore << "\t\t\t" << "0" << "\t\t\t" << nInputCnt << "\t\t\t" << grp.correctRowCnt << "\t\t\t" << grp.wrongRowCnt << "\n\r";
		}
		out << CN_CODEC("�ܷ�: ") << dTotScore;

		QDesktopServices::openUrl(QUrl(logFileName));
	}
}

void CTestCaseTable::genTestCases()
{
	qsrand((uint)time(NULL));
	for (uint i = 1; i <= GRP_CNT; ++i)
	{
		for (uint j = 1; j <= CASE_CNT; ++j)
		{
			QString strNum;
			int nNumLen = qrand() % 6 + 1; // ���ֳ���6~10
			if (qrand() % 10 > 7)	// ����
				strNum.push_back('-');
			strNum.push_back((char)(qrand() % 9 + 1 + '0'));	// ��һ������
			--nNumLen;
			while (nNumLen--)
			{
				if (nNumLen == 1)
					strNum.push_back('.');
				strNum.push_back((char)(qrand() % 9 + '0'));
			}
			m_testGrp[i].sample[j] = strNum;
		}
	}

}

void CTestCaseTable::showNextGrp()
{
	if (m_nGrpNum >= GRP_CNT)
		return ;

	++m_nGrpNum;
	loadData();
}

void CTestCaseTable::showPrevGrp()
{
	if (m_nGrpNum <= 1)
		return ;

	--m_nGrpNum;
	loadData();
}

void CTestCaseTable::keyReleaseEvent(QKeyEvent *keyEvent)
{
	if (currentColumn() != 2)
		return ;

	if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
	{
		int nCurCol = currentColumn();
		int nCurRow = currentRow();

		QString strInput = currentItem()->text();
		if (strInput.length() >= 2)
			strInput.insert(strInput.length() - 2, '.');
		setItem(nCurRow, nCurCol, new CItemTestInput(strInput));

		QString strAnswer = m_testGrp[m_nGrpNum].sample[nCurRow + 1];

		if (strInput == strAnswer)
		{
			++m_nCorretRowCnt;
		}
		else
		{
			++m_nWrongRowCnt;
		}
		emit sigCorrectAndWrongCnt(m_nCorretRowCnt, m_nWrongRowCnt);

		m_testGrp[m_nGrpNum].input[nCurRow + 1] = strInput;

		// �������һ����
		if (nCurRow + 1 == CASE_CNT)
		{
			// �������һ��
			if (m_nGrpNum < GRP_CNT)
				showNextGrp();
			else
				finish();
		}
		else
		{
			setCurrentCell(nCurRow + 1, nCurCol);
		}
	}
}

void CTestCaseTable::resizeEvent(QResizeEvent *event)
{
	setColumnWidth(0, width() / 3 - 1);
	setColumnWidth(1, width() / 3 - 1);
	setColumnWidth(2, width() / 3 - 1);


}