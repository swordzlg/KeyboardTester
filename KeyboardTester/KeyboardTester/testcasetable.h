#ifndef TESTCASETABLE_H
#define TESTCASETABLE_H

#include <QtWidgets>
#include <vector>
#include "public.h"

using std::vector;
using std::pair;

typedef struct _TestGrp
{
	uint correctRowCnt;
	uint wrongRowCnt;
	QString sample[CASE_CNT + 1];	// 从1开始计算
	QString input[CASE_CNT + 1];	// 从1开始计算

	_TestGrp()
		: correctRowCnt(0)
		, wrongRowCnt(0)
	{

	}

	void clear()
	{
		correctRowCnt = wrongRowCnt = 0;
		for (int i = 1; i <= CASE_CNT; ++i)
		{
			sample[i].clear();
			input[i].clear();
		}
	}

}TESTGRP;

class CItemCaseNum : public QTableWidgetItem
{
public:
	CItemCaseNum(uint i)
	{
		setText(QString::number(i));
		setFeature();
	}
	CItemCaseNum(const QString &str)
	{
		setText(str);
		setFeature();
	}
	void setFeature()
	{
		setTextAlignment(Qt::AlignCenter);

		setFlags(flags() ^ Qt::ItemIsSelectable ^ Qt::ItemIsEditable ^ Qt::ItemIsUserCheckable);

		QFont font;
		font.setBold(false);
		setFont(font);
	}
	~CItemCaseNum() { }

};

class CItemTestCase : public QTableWidgetItem
{
public:
	CItemTestCase(const QString &str)
	{
		setText(str);
		setTextAlignment(Qt::AlignRight);
		setFlags(flags() ^ Qt::ItemIsSelectable ^ Qt::ItemIsEditable);
//		setFlags(flags() ^ Qt::ItemIsEditable);

		QFont font;
		font.setPixelSize(20);
		font.setBold(false);
		setFont(font);
	}
	~CItemTestCase() { }
};

class CItemTestInput : public QTableWidgetItem
{
public:
	CItemTestInput(const QString &str)
	{
		setText(str);
		setTextAlignment(Qt::AlignRight);
		//setFlags(flags() ^ Qt::ItemIsUserCheckable);
		//		setFlags(flags() ^ Qt::ItemIsEditable);

		QFont font;
		font.setPixelSize(20);
		font.setBold(false);
		setFont(font);
	}
	~CItemTestInput() { }
};

class CTestCaseTable : public QTableWidget
{
	Q_OBJECT

public:
    CTestCaseTable();
    virtual ~CTestCaseTable();
	
	void initTable();
	void newTest();
    void showNextGrp(); // 显示下一组题目
    void showPrevGrp(); // 显示上一组题目

	void keyReleaseEvent(QKeyEvent *keyEvent);
	void resizeEvent(QResizeEvent *event);

private:
    void genTestCases();
	void loadData();
	double calcScore();

	bool m_bInit;
    //uint m_nGrpNumInProcess;	// 目前正进行测试的组号，从1开始计算
	//uint m_nGrpNumOnShow;		// 目前正展示的组号，从1开始计算
	uint m_nGrpNum;				// 当前组号
	uint m_nCorretRowCnt;		// 正确行数
	uint m_nWrongRowCnt;		// 错误行数
    TESTGRP m_testGrp[GRP_CNT + 1];			// 测试用例组，下标从1开始

signals:
	void sigCorrectAndWrongCnt(uint, uint);
	void sigTestFinished(uint nScore);

public slots:
	void finish();
};

#endif // TESTCASETABLE_H
