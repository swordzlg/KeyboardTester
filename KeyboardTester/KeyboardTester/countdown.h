#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QtWidgets>

class CCountdown : public QLabel
{
    Q_OBJECT
public:
    explicit CCountdown();
    ~CCountdown();

	void setTotTime(uint nSec);
    void start();
    void resume();
    void stop();

private:
    int m_nLeftTime;    // 剩余时间
    QTimer m_timer;

	void showTime();

signals:
    void timeup();

private slots:
    void updateTime();  // 更新剩余时间
};

#endif // COUNTDOWN_H
