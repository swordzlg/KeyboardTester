#ifndef TESTCASETABLE_H
#define TESTCASETABLE_H

#include <QtWidgets>
#include <vector>
#include "public.h"

using std::vector;
using std::pair;

struct TestGroup
{
	uint correctRowCount;
	uint wrongRowCount;
	QString sample[kCaseCount + 1];	// 从1开始计算
	QString input[kCaseCount + 1];	// 从1开始计算

	TestGroup()
		: correctRowCount(0)
		, wrongRowCount(0)
	{

	}

	void clear()
	{
		correctRowCount = wrongRowCount = 0;
		for (int i = 1; i <= kCaseCount; ++i)
		{
			sample[i].clear();
			input[i].clear();
		}
	}
};

class CItemCaseNum : public QTableWidgetItem
{
public:
	CItemCaseNum(uint i)
	{
		_init();
		setText(QString::number(i));
	}

	CItemCaseNum(const QString &str)
	{
		_init();
		setText(str);
	}

	~CItemCaseNum() { }

private:
	void _init()
	{
		setTextAlignment(Qt::AlignCenter);

		setFlags(flags() ^ Qt::ItemIsSelectable ^ Qt::ItemIsEditable ^ Qt::ItemIsUserCheckable);

		QFont font;
		font.setBold(false);
		setFont(font);
	}
};

class CItemTestCase : public QTableWidgetItem
{
public:
	CItemTestCase(const QString &str)
	{
		setText(str);
		setTextAlignment(Qt::AlignRight);
		setFlags(flags() ^ Qt::ItemIsSelectable ^ Qt::ItemIsEditable);

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
	explicit CTestCaseTable(QWidget* parent = 0);
	virtual ~CTestCaseTable();
	
	void initTable();
	void newTest();
	void showNextGroup(); // 显示下一组题目
	void showPrevGroup(); // 显示上一组题目

	virtual void keyReleaseEvent(QKeyEvent *keyEvent);
	virtual void resizeEvent(QResizeEvent *event);

private:
	void _genTestCases();
	void _loadData();
	//double _calcScore();

signals:
	void sigCorrectAndWrongCount(uint, uint);
	void sigTestFinished(uint nScore);
	void sigTestGroupFinished();

public slots:
	void finish();

private:
	bool m_bInit;
	uint m_nCurrentGroupNo;		// 当前组号
	uint m_nCorretRowCount;		// 正确行数
	uint m_nWrongRowCount;		// 错误行数
	TestGroup m_testGroup[kGroupCount + 1];		// 测试用例组，下标从1开始
};

#endif // TESTCASETABLE_H
