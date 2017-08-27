#include "widget.h"
#include "public.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
	m_nCurGrpNum = 0;

	// ��ʼ���ؼ�
    m_pbStart.setText(CN_CODEC("��ʼ"));
	customPtn(m_pbStart);
    m_pbFinish.setText(CN_CODEC("����"));
	customPtn(m_pbFinish);
    m_pbNext.setText(CN_CODEC("��һ��"));
	customPtn(m_pbNext);
    m_pbPrev.setText(CN_CODEC("��һ��"));
	customPtn(m_pbPrev);

	updateCurGrpTips();
	onCorrectAndWrongCnt(0, 0);

    setLayout(getLayout());

    connect(&m_pbStart, SIGNAL(clicked()), this, SLOT(onStart()));
    connect(&m_pbFinish, SIGNAL(clicked()), this, SLOT(onFinish()));
    connect(&m_pbNext, SIGNAL(clicked()), this, SLOT(onNext()));
    connect(&m_pbPrev, SIGNAL(clicked()), this, SLOT(onPrev()));
	connect(&m_testCaseTable, SIGNAL(sigCorrectAndWrongCnt(uint,uint)), this, SLOT(onCorrectAndWrongCnt(uint, uint)));
	connect(&m_countdown, SIGNAL(timeup()), &m_testCaseTable, SLOT(finish()));

	m_testCaseTable.initTable();

	//QFont font(CN_CODEC("����"), 12, QFont::Bold);
	//setFont(font);

	showMaximized();

	m_pbStart.setFocus();
}

Widget::~Widget()
{

}

QLayout* Widget::getLayout()
{
    QHBoxLayout *pMainLayout = new QHBoxLayout;

    QGroupBox *pGrpBox = new QGroupBox;
    QVBoxLayout *pSubLayout0 = new QVBoxLayout;
    pSubLayout0->addWidget(getLabel(CN_CODEC("<font size=\"5\"> <b>ʣ��ʱ��</b>")));
    pSubLayout0->addWidget(&m_countdown);
	pSubLayout0->addSpacing(15);
    pSubLayout0->addWidget(getLabel(CN_CODEC("<font size=\"5\"> <b>����</b>")));
    pSubLayout0->addWidget(getLabel(CN_CODEC("<font size=\"5\"> xixi")));
	pSubLayout0->addSpacing(10);
    pSubLayout0->addWidget(getLabel(CN_CODEC("<font size=\"5\"> <b>��������</b>")));
    pSubLayout0->addWidget(getLabel(CN_CODEC("<font size=\"5\"> С������ϰ")));
	pSubLayout0->addSpacing(10);
    pSubLayout0->addWidget(getLabel(CN_CODEC("<font size=\"5\"> <b>��Ŀ����</b>")));
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
//	pSubLayout1->addWidget(getLabel(CN_CODEC("\n\n��Ŀ������\n\t����26�⣬ÿ��25��¼����\n\n���ֱ�׼��\n\t����¼�����ȷ��ÿ��5�֣��𰸴���Ĳ��Ʒ֡�\n\t"
//		"��������ǰ���һ����δ���ȫ��¼�룬��¼��\n�����ȷ�ģ�Ӧ��ʵ��¼�����������ɼ���ÿ�е�0.2�֣�\n�������еĲ��Ʒ֡�\n")));
	pSubLayout1->addWidget(getLabel(CN_CODEC("<font size=\"6\" color=#309E69> ��Ŀ������<br>  ����26�⣬ÿ��25��¼����<br><br>���ֱ�׼��<br>����¼�����ȷ��ÿ��5�֣�<br>�𰸴���Ĳ��Ʒ֡�<br>"
		"��������ǰ���һ����δ���<br>ȫ��¼�룬��¼������ȷ�ģ�<br>Ӧ��ʵ��¼�����������ɼ���<br>ÿ�е�0.2�֣��������еĲ���<br>�֡�")));
	pSubLayout1->addStretch(Qt::Vertical);	
    pSubLayout1->addWidget(&m_labInputRowCnt);
    pSubLayout1->addWidget(&m_labCorrectRowCnt);
    pSubLayout1->addWidget(&m_labWrongRowCnt);
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

void Widget::getNumbers()
{
	
}

void Widget::customPtn(QPushButton &ptn)
{
	QFont fnt;
	fnt.setPointSize(12);

	ptn.setFont(fnt);

	ptn.setFixedSize(100, 30);
}

QLabel* Widget::getLabel(const QString& text)
{
	QLabel *lab = new QLabel(text);

	return lab;
}

void Widget::updateCurGrpTips()
{
	QString strTips = QString(CN_CODEC("<b><font size=\"5\"> ��ǰΪ����<font color=\"red\">%1<font color=\"black\">/<font color=\"blue\">26<font color=\"black\">��</b>")).arg(m_nCurGrpNum);
	m_labCurGrpNum.setText(strTips);
}

// ÿ��start����һ��ȫ�µĲ��ԣ���������;��ͣ�Ĺ���
void Widget::onStart()
{
	m_nCurGrpNum = 1;
	updateCurGrpTips();

	m_countdown.setTotTime(TEST_TIME);
	m_countdown.start();

	m_testCaseTable.setFocus();
	m_testCaseTable.newTest();
}

void Widget::onFinish()
{
	m_countdown.stop();
	m_testCaseTable.finish();
}

void Widget::onPrev()
{
	if (m_nCurGrpNum <= 0)
		return;

	m_testCaseTable.showPrevGrp();

	--m_nCurGrpNum;

	updateCurGrpTips();
}

void Widget::onNext()
{
	if (m_nCurGrpNum >= GRP_CNT)
		return;

	m_testCaseTable.showNextGrp();

	++m_nCurGrpNum;

	updateCurGrpTips();
}

void Widget::onCorrectAndWrongCnt(uint nCorrectCnt, uint nWrongCnt)
{
	QString strCorrect, strWrong, strInput;
	const QString c_strInputTips = CN_CODEC("<font size=\"5\"> ¼�������� ");
	const QString c_strCorrectTips = CN_CODEC("<font size=\"5\"> ��ȷ������ ");
	const QString c_strWrongTips = CN_CODEC("<font size=\"5\"> ���������� ");
	QTextStream textStream;

	textStream.setString(&strInput);
	textStream << c_strInputTips << nCorrectCnt + nWrongCnt;
	m_labInputRowCnt.setText(strInput);

	textStream.setString(&strCorrect);
	textStream << c_strCorrectTips << nCorrectCnt;
	m_labCorrectRowCnt.setText(strCorrect);

	textStream.setString(&strWrong);
	textStream << c_strWrongTips << nWrongCnt;
	m_labWrongRowCnt.setText(strWrong);
}

void Widget::resizeEvent(QResizeEvent * eve)
{
	
}