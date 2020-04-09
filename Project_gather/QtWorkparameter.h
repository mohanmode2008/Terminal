#pragma once

#include <QWidget>
#include "ui_QtWorkparameter.h"
#include <QtNetwork>

#include <QAbstractSocket>
class QTcpSocket;

class QtWorkparameter : public QWidget
{
	Q_OBJECT

public:
	QtWorkparameter(QWidget *parent = Q_NULLPTR);
	~QtWorkparameter();

private:
	Ui::QtWorkparameter ui;

	QNetworkAccessManager* manager_post_task_program;
	QNetworkAccessManager* manager_post_author_code;

	QTcpSocket* m_tcpClient;

private slots:
	void btn_to_state();
	void btn_to_quit();
	void btn_to_realtime_image();
	void btn_to_timed_image();

	void btn_post_task_plan();
	void finished_post_task_plan(QNetworkReply*);

	void btn_post_author_code();
	void finished_post_author_code(QNetworkReply*);

	qint8 tcp_readserver_data();
	void tcp_connected_success();
};
