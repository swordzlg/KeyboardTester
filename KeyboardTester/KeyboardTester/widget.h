#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>
#include <vector>

#include "countdown.h"
#include "testcasetable.h"

using std::vector;

typedef vector<double> DoubleVer;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

	void resizeEvent(QResizeEvent * eve);

private:
    QLayout *getLayout();
    void getNumbers();
	void updateCurGrpTips();

	void customPtn(QPushButton &ptn);
    QLabel* getLabel(const QString& text);

    CCountdown  m_countdown;              // 剩余时间
    CTestCaseTable m_testCaseTable;
    QPushButton m_pbStart;
    QPushButton m_pbFinish;
    QPushButton m_pbNext;
    QPushButton m_pbPrev;
    QLabel      m_labCurGrpNum;             // 当前是第几题
    QLabel      m_labInputRowCnt;           // 录入行数
    QLabel      m_labCorrectRowCnt;         // 正确行数
    QLabel      m_labWrongRowCnt;           // 错误行数
    QLineEdit   m_leGroupCnt;               // 数据组数
    QLineEdit   m_leTimeLimitEveryGrp;      // 每组用时
    DoubleVer   m_vecTimeComsumedEveryGrp;
	uint		m_nCurGrpNum;				// 当前题号

public slots:
	void onStart();
	void onFinish();
	void onPrev();
	void onNext();
	void onCorrectAndWrongCnt(uint nCorrectCnt, uint nWrongCnt);
};

#endif // WIDGET_H
