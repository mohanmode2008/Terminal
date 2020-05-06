#pragma once

#include <QWidget>
#include "ui_QtAnt.h"

#include <QAbstractSocket>
class QTcpSocket;

class QtAnt : public QWidget
{
	Q_OBJECT

public:
	QtAnt(QWidget *parent = Q_NULLPTR);
	~QtAnt();

private:
	Ui::QtAnt ui;

	QTcpSocket* m_tcpClient;

	QTimer* m_timer;
	

public slots:
	void btn_to_gather();
	void btn_to_reset();
	void btn_to_set_angle();
	void btn_to_set_speed();
	void btn_to_stop_ant();
	void btn_to_decide_ant_direction();
	void btn_to_decide_ant_pitch();
	void btn_to_inquire_ant();
	void btn_to_inquire_gps();

	void btn_to_connect_ant();
	void connected_success();
	void readserver_data();

	void TimerTimeOut();
	void InitTimer();
};
