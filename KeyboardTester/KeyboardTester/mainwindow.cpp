#include "mainwindow.h"
#include "public.h"

CMainWindow::CMainWindow(QWidget *parent)
    : QWidget(parent)
{
	m_nCurrentGroupNo = 0;

	// 初始化控件
    m_pbStart.setText(CN_CODEC("开始"));
	customPtn(m_pbStart);
    m_pbFinish.setText(CN_CODEC("结束"));
	customPtn(m_pbFinish);
    m_pbNext.setText(CN_CODEC("下一题"));
	customPtn(m_pbNext);
    m_pbPrev.setText(CN_CODEC("上一题"));
	customPtn(m_pbPrev);

	updateCurrentGroupTips();
	onCorrectAndWrongCnt(0, 0);

    setLayout(getLayout());

    connect(&m_pbStart, SIGNAL(clicked()), this, SLOT(onStart()));
    connect(&m_pbFinish, SIGNAL(clicked()), this, SLOT(onFinish()));
    connect(&m_pbNext, SIGNAL(clicked()), this, SLOT(onNext()));
    connect(&m_pbPrev, SIGNAL(clicked()), this, SLOT(onPrev()));
	connect(&m_testCaseTable, SIGNAL(sigCorrectAndWrongCount(uint,uint)), this, SLOT(onCorrectAndWrongCnt(uint, uint)));
	connect(&m_countdown, SIGNAL(timeup()), &m_testCaseTable, SLOT(finish()));

	m_testCaseTable.initTable();

	//QFont font(CN_CODEC("宋体"), 12, QFont::Bold);
	//setFont(font);

	showMaximized();

	m_pbStart.setFocus();
}

CMainWindow::~CMainWindow()
{

}

QLayout* CMainWindow::getLayout()
{
    QHBoxLayout *pMainLayout = new QHBoxLayout;

    QGroupBox *pGrpBox = new QGroupBox;
    QVBoxLayout *pSubLayout0 = new QVBoxLayout;
    pSubLayout0->addWidget(getLabel(CN_CODEC("<font size=\"5\"> <b>剩余时间</b>")));
    pSubLayout0->addWidget(&m_countdown);
	pSubLayout0->addSpacing(15);
    pSubLayout0->addWidget(getLabel(CN_CODEC("<font size=\"5\"> <b>姓名</b>")));
    pSubLayout0->addWidget(getLabel(CN_CODEC("<font size=\"5\"> xixi")));
	pSubLayout0->addSpacing(10);
    pSubLayout0->addWidget(getLabel(CN_CODEC("<font size=\"5\"> <b>试题名称</b>")));
    pSubLayout0->addWidget(getLabel(CN_CODEC("<font size=\"5\"> 小键盘练习")));
	pSubLayout0->addSpacing(10);
    pSubLayout0->addWidget(getLabel(CN_CODEC("<font size=\"5\"> <b>题目数量</b>")));
    pSubLayout0->addWidget(getLabel(CN_CODEC("<font size=\"5\"> 26")));
	pSubLayout0->addStretch(Qt::Vertical);
    pGrpBox->setLayout(pSubLayout0);

    QVBoxLayout *pSubLayout1 = new QVBoxLayout;
    QHBoxLayout *pSubLayout1_0 = new QHBoxLayout;
    pSubLayout1_0->addWidget(&m_pbStart);
    pSubLayout1_0->addStretch(Qt::Horizontal);
    pSubLayout1_0->addWidget(&m_pbFinish);
    QHBoxLayout *pSubLayout1_1 = new QHBoxLayout;
    pSubLayout1_1->addWidget(&m_pbPrev);
    pSubLayout1_1->addStretch(Qt::Horizontal);
    pSubLayout1_1->addWidget(&m_pbNext);
    pSubLayout1->addLayout(pSubLayout1_0);
	pSubLayout1->addStretch(Qt::Vertical);
	pSubLayout1->addWidget(&m_labCurGrpNum);
	pSubLayout1->addStretch(Qt::Vertical);
//	pSubLayout1->addWidget(getLabel(CN_CODEC("\n\n题目个数：\n\t共有26题，每题25个录入项\n\n评分标准：\n\t数字录入答案正确的每题5分，答案错误的不计分。\n\t"
//		"竞赛结束前最后一题虽未完成全部录入，但录入\n结果正确的，应按实际录入的行数计算成绩，每行得0.2分，\n不足两行的不计分。\n")));
	pSubLayout1->addWidget(getLabel(CN_CODEC("<font size=\"6\" color=#309E69> 题目个数：<br>  共有26题，每题25个录入项<br><br>评分标准：<br>数字录入答案正确的每题5分，<br>答案错误的不计分。<br>"
		"竞赛结束前最后一题虽未完成<br>全部录入，但录入结果正确的，<br>应按实际录入的行数计算成绩，<br>每行得0.2分，不足两行的不计<br>分。")));
	pSubLayout1->addStretch(Qt::Vertical);	
    pSubLayout1->addWidget(&m_labInputRowCount);
    pSubLayout1->addWidget(&m_labCorrectRowCount);
    pSubLayout1->addWidget(&m_labWrongRowCount);
	pSubLayout1->addStretch(Qt::Vertical);	
	pSubLayout1->addLayout(pSubLayout1_1);

    pMainLayout->addWidget(pGrpBox);
    pMainLayout->addWidget(&m_testCaseTable);
    pMainLayout->addLayout(pSubLayout1);
	pMainLayout->setStretch(0, 1);
	pMainLayout->setStretch(1, 8);
	pMainLayout->setStretch(2, 2);

    return pMainLayout;
}

void CMainWindow::getNumbers()
{
	
}

void CMainWindow::customPtn(QPushButton &ptn)
{
	QFont fnt;
	fnt.setPointSize(12);

	ptn.setFont(fnt);

	ptn.setFixedSize(100, 30);
}

QLabel* CMainWindow::getLabel(const QString& text)
{
	QLabel *lab = new QLabel(text);

	return lab;
}

void CMainWindow::updateCurrentGroupTips()
{
	QString strTips = QString(CN_CODEC("<b><font size=\"5\"> 当前为：第<font color=\"red\">%1<font color=\"black\">/<font color=\"blue\">26<font color=\"black\">题</b>")).arg(m_nCurrentGroupNo);
	m_labCurGrpNum.setText(strTips);
}

// 每次start都是一次全新的测试，不存在中途暂停的功能
void CMainWindow::onStart()
{
	m_nCurrentGroupNo = 1;
	updateCurrentGroupTips();

	m_countdown.setTotalTime(kTestTime);
	m_countdown.start();

	m_testCaseTable.setFocus();
	m_testCaseTable.newTest();
}

void CMainWindow::onFinish()
{
	m_countdown.stop();
	m_testCaseTable.finish();
}

void CMainWindow::onPrev()
{
	if (m_nCurrentGroupNo <= 0)
		return;

	m_testCaseTable.showPrevGroup();

	--m_nCurrentGroupNo;

	updateCurrentGroupTips();
}

void CMainWindow::onNext()
{
	if (m_nCurrentGroupNo >= kGroupCount)
		return;

	m_testCaseTable.showNextGroup();

	++m_nCurrentGroupNo;

	updateCurrentGroupTips();
}

void CMainWindow::onCorrectAndWrongCnt(uint nCorrectCnt, uint nWrongCnt)
{
	QString strCorrect, strWrong, strInput;
	const QString c_strInputTips = CN_CODEC("<font size=\"5\"> 录入行数： ");
	const QString c_strCorrectTips = CN_CODEC("<font size=\"5\"> 正确行数： ");
	const QString c_strWrongTips = CN_CODEC("<font size=\"5\"> 错误行数： ");
	QTextStream textStream;

	textStream.setString(&strInput);
	textStream << c_strInputTips << nCorrectCnt + nWrongCnt;
	m_labInputRowCount.setText(strInput);

	textStream.setString(&strCorrect);
	textStream << c_strCorrectTips << nCorrectCnt;
	m_labCorrectRowCount.setText(strCorrect);

	textStream.setString(&strWrong);
	textStream << c_strWrongTips << nWrongCnt;
	m_labWrongRowCount.setText(strWrong);
}

void CMainWindow::resizeEvent(QResizeEvent * eve)
{
	
}