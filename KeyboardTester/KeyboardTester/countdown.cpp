#include "countdown.h"

CCountdown::CCountdown()
{
    m_timer.setInterval(1000);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateTime()));

	QFont fnt;
	fnt.setPointSize(20);
	setFont(fnt);

	setStyleSheet("QLabel { color : red; }");
}

CCountdown::~CCountdown()
{

}

void CCountdown::setTotTime(uint nSec)
{
	static const auto MAX_TOTTIME = 60 * 60;
	m_nLeftTime = nSec < MAX_TOTTIME ? nSec : MAX_TOTTIME;
	showTime();
}

void CCountdown::start()
{
    m_timer.start();
}

void CCountdown::resume()
{
    m_timer.start();
}

void CCountdown::stop()
{
    m_timer.stop();
}

void CCountdown::showTime()
{
	QString strTime;

	int nHour = m_nLeftTime / 3600;
	int nMin = (m_nLeftTime % 3600) / 60;
	int nSec = m_nLeftTime % 60;
	strTime.sprintf("%02d:%02d:%02d", nHour, nMin, nSec);

	setText(strTime);
}

void CCountdown::updateTime()
{
    --m_nLeftTime;

    showTime();
       
    if (!m_nLeftTime)
	{
		m_timer.stop();
		emit timeup();
	}
}


