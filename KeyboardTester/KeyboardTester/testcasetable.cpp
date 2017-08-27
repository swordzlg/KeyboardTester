#include "testcasetable.h"
#include "public.h"
#include <cstdlib>
#include <ctime>
#include <cassert>

#define  QT_KEYPAD_NAVIGATION

CTestCaseTable::CTestCaseTable(QWidget* parent /*= 0*/)
	: QTableWidget(parent)
{
	qsrand((uint)time(NULL));

	m_bInit = false;
	setEditTriggers(QAbstractItemView::AllEditTriggers);
}

CTestCaseTable::~CTestCaseTable()
{

}

void CTestCaseTable::newTest()
{
	assert(m_bInit);

	_genTestCases();

	m_nCurrentGroupNo = 1;
	
	m_nCorretRowCount = 0;
	m_nWrongRowCount = 0;

	_loadData();

	//editItem(item(0,2));
	//setCurrentCell(0, 2);
}

void CTestCaseTable::initTable()
{
	// ��������������
	setColumnCount(3);
	setRowCount(kCaseCount + 1);

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

void CTestCaseTable::_loadData()
{
	const TestGroup& testGrp = m_testGroup[m_nCurrentGroupNo]; 
	for ( int i = 1; i <= kCaseCount; ++i)
	{
		setItem(i-1, 1, new CItemTestCase(testGrp.sample[i]));
		setItem(i-1, 2, new CItemTestInput(testGrp.input[i]));
	}
	setCurrentCell(0, 2);
}

//double CTestCaseTable::_calcScore()
//{
//	// �ҵ���������һ��
//	int nLastGroup = kGroupCount;
//	do
//	{
//		if (m_testGroup[nLastGroup].input.isEmpty())
//			break;
//	}while (--nLastGroup);
//
//	uint nLastGroupCorrectCount = m_testGroup[nLastGroup].correctRowCount;
//	double dScore = nLastGroupCorrectCount <= 1 ? 0.0 : nLastGroupCorrectCount * 0.2;
//	for (int i = 1; i < nLastGroup; ++i)
//	{
//		// ȫ�Բŵ÷�
//		if (m_testGroup[i].correctRowCount == kCaseCount)
//			dScore += 5.0;
//	}
//	return dScore;
//}

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
		int nLastGrp = kGroupCount;
		for ( ; nLastGrp > 0 && m_testGroup[nLastGrp].correctRowCount + m_testGroup[nLastGrp].wrongRowCount == 0; --nLastGrp);
		uint nLastCrpCorrectCnt = m_testGroup[nLastGrp].correctRowCount;
		double dTotScore = nLastCrpCorrectCnt < 2 ? 0.0 : nLastCrpCorrectCnt * 0.2;
		for (int i = 1; i < nLastGrp; ++i)
		{
			const TestGroup &grp = m_testGroup[i];
			double dScore;
			// �����һ�⣬������Ŀȫ�Բŵ÷�
			if (grp.correctRowCount == kCaseCount)
				dScore = 5.0;
			else
				dScore = 0.0;
			dTotScore += dScore;
			int nInputCnt = grp.correctRowCount + grp.wrongRowCount;
			if (nInputCnt)
				out << i << "\t\t\t" << dScore << "\t\t\t" << 1.0 * grp.correctRowCount / nInputCnt << "\t\t\t" << nInputCnt << "\t\t\t" << grp.correctRowCount << "\t\t\t" << grp.wrongRowCount << "\n\r";
			else
				out << i << "\t\t\t" << dScore << "\t\t\t" << "0" << "\t\t\t" << nInputCnt << "\t\t\t" << grp.correctRowCount << "\t\t\t" << grp.wrongRowCount << "\n\r";
		}
		// �������һ��
		{
			const TestGroup &grp = m_testGroup[nLastGrp];
			double dScore = 0.0;
			if (grp.correctRowCount > 1)
				dScore = grp.correctRowCount * 0.2;
			dTotScore += dScore;
			int nInputCnt = grp.correctRowCount + grp.wrongRowCount;
			if (nInputCnt)
				out << nLastGrp << "\t\t\t" << dScore << "\t\t\t" << 1.0 * grp.correctRowCount / nInputCnt << "\t\t\t" << nInputCnt << "\t\t\t" << grp.correctRowCount << "\t\t\t" << grp.wrongRowCount << "\n\r";
			else
				out << nLastGrp << "\t\t\t" << dScore << "\t\t\t" << "0" << "\t\t\t" << nInputCnt << "\t\t\t" << grp.correctRowCount << "\t\t\t" << grp.wrongRowCount << "\n\r";
		}
		out << CN_CODEC("�ܷ�: ") << dTotScore;

		QDesktopServices::openUrl(QUrl(logFileName));
	}
}

void CTestCaseTable::_genTestCases()
{
	for (uint i = 1; i <= kGroupCount; ++i)
	{
		int nNegativeCount = 5;
		for (uint j = 1; j <= kCaseCount; ++j)
		{
			QString strNum;
			int nNumLen = qrand() % 4 + 6; // ���ֳ���6~11
			// ����
			if (nNegativeCount != 0 &&
				(qrand() % 10 > 7 || j + nNegativeCount == kCaseCount))
			{
				--nNegativeCount;
				strNum.push_back('-');
			}
			strNum.push_back((char)(qrand() % 9 + 1 + '0'));	// ��һ�����ֲ�Ϊ0
			--nNumLen;
			while (nNumLen--)
			{
				if (nNumLen == 1)
					strNum.push_back('.');
				strNum.push_back((char)(qrand() % 9 + '0'));
			}
			m_testGroup[i].sample[j] = strNum;
		}
	}

}

void CTestCaseTable::showNextGroup()
{
	if (m_nCurrentGroupNo >= kGroupCount)
		return ;

	++m_nCurrentGroupNo;
	m_nCorretRowCount = m_testGroup[m_nCurrentGroupNo].correctRowCount;
	m_nWrongRowCount = m_testGroup[m_nCurrentGroupNo].wrongRowCount;
	_loadData();
}

void CTestCaseTable::showPrevGroup()
{
	if (m_nCurrentGroupNo <= 1)
		return ;

	--m_nCurrentGroupNo;
	m_nCorretRowCount = m_testGroup[m_nCurrentGroupNo].correctRowCount;
	m_nWrongRowCount = m_testGroup[m_nCurrentGroupNo].wrongRowCount;
	_loadData();
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

		const QString strAnswer = m_testGroup[m_nCurrentGroupNo].sample[nCurRow + 1];
		const QString strLastInput = m_testGroup[m_nCurrentGroupNo].input[nCurRow + 1];

		// ��ǰ���ǵ�һ��������
		if (m_nCorretRowCount + m_nWrongRowCount == nCurRow)
		{
			if (strInput == strAnswer)
				++m_nCorretRowCount;
			else if (!strInput.isEmpty())
				++m_nWrongRowCount;
		}
		// ��ǰ��֮ǰ�Ѿ�������
		else if (strInput != strLastInput)
		{
			// ɾ������
			if (strInput.isEmpty())
			{
				if (strLastInput == strAnswer)
					--m_nCorretRowCount;
				else
					--m_nWrongRowCount;
			}
			else
			{
				if (strInput == strAnswer)
				{
					++m_nCorretRowCount;
					--m_nWrongRowCount;
				}
				else
				{
					--m_nCorretRowCount;
					++m_nWrongRowCount;
				}
			}
		}

		emit sigCorrectAndWrongCount(m_nCorretRowCount, m_nWrongRowCount);

		m_testGroup[m_nCurrentGroupNo].input[nCurRow + 1] = strInput;

		// �������һ��
		if (nCurRow + 1 == kCaseCount)
			emit sigTestGroupFinished();
		else
		{
			setCurrentCell(strInput.isEmpty() ? nCurRow : nCurRow + 1, nCurCol);
		}
	}
}

void CTestCaseTable::resizeEvent(QResizeEvent *event)
{
	setColumnWidth(0, width() / 3 - 1);
	setColumnWidth(1, width() / 3 - 1);
	setColumnWidth(2, width() / 3 - 1);
}