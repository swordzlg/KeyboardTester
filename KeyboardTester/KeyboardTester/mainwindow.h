#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <vector>

#include "countdown.h"
#include "testcasetable.h"

using std::vector;

class CMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

	void resizeEvent(QResizeEvent * eve);

private:
    QLayout *getLayout();
    void getNumbers();
	void updateCurrentGroupTips();

	void customPtn(QPushButton &ptn);
    QLabel* getLabel(const QString& text);

public slots:
	void onStart();
	void onFinish();
	void onPrev();
	void onNext();
	void onCorrectAndWrongCnt(uint nCorrectCnt, uint nWrongCnt);

private:
	CCountdown  m_countdown;              // 剩余时间
	CTestCaseTable m_testCaseTable;
	QPushButton m_pbStart;
	QPushButton m_pbFinish;
	QPushButton m_pbNext;
	QPushButton m_pbPrev;
	QLabel      m_labCurGrpNum;             // 当前是第几题
	QLabel      m_labInputRowCount;           // 录入行数
	QLabel      m_labCorrectRowCount;         // 正确行数
	QLabel      m_labWrongRowCount;           // 错误行数
	QLineEdit   m_leGroupCount;               // 数据组数
	QLineEdit   m_leTimeLimitEveryGroup;      // 每组用时
	vector<double> m_vecTimeComsumedEveryGroup;
	uint		m_nCurrentGroupNo;				// 当前题号
};

#endif // MAINWINDOW_H
