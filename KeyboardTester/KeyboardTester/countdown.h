#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QtWidgets>

// 倒计时
class CCountdown : public QLabel
{
    Q_OBJECT
public:
    explicit CCountdown();
    ~CCountdown();

	void setTotalTime(uint nSec);
    void start();
    void resume();
    void stop();

private:
	void showTime();

signals:
    void timeup();

private slots:
    void updateTime();  // 更新剩余时间

private:
	int m_nLeftTime;    // 剩余时间
	QTimer m_timer;
};

#endif // COUNTDOWN_H
