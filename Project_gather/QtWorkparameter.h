#pragma once

#include <QWidget>
#include "ui_QtWorkparameter.h"
#include <QtNetwork>
#include <QNetworkRequest>
#include <QAbstractSocket>

class QTcpSocket;

typedef struct plan_results {
	qint8  satelliteId;
	qint8  stationId;
	QString  commSyncWord;
	qint64  period_start;
	qint64  period_end;
}plan_results_t;

class QtWorkparameter : public QWidget
{
	Q_OBJECT

public:
	QtWorkparameter(QWidget *parent = Q_NULLPTR);
	~QtWorkparameter();

	uchar* valid_data_to_send_data();

	plan_results_t * ptaskplan_results;

private:
	Ui::QtWorkparameter ui;

	QNetworkAccessManager* manager_post_task_program;
	QNetworkAccessManager* manager_post_author_code;

	QNetworkAccessManager* manager_get_time;

	QTcpSocket* m_tcpClient;
	QTcpSocket* tcp_to_ground_ctl;
	QTcpSocket* tcp_to_ground_as_station;

	QUdpSocket* sender;

	QString time_charge_sec_to_dataform(quint64 sec);

	qint64 get_second_2010_to_1970();

	quint16 return_plan_num;

private slots:
	void btn_to_state();
	void btn_to_quit();
	void btn_to_realtime_image();
	void btn_to_timed_image();
	void btn_to_ant();

	void btn_get_time();

	void finished_get_time(QNetworkReply*);

	void btn_post_author_code();
	void finished_post_author_code(QNetworkReply*);

	quint8 tcp_readserver_data();
	void tcp_connected_success();

	void btn_tcp_to_ground_as_station_ctl();

	void tcp_send_station_identity_data();

	void station_connected_ground_success();

	void station_rev_ground_read_data();

	void by_udp_connect_to_sky();

	void udp_rev_data_from_sky();

	void udp_send_device_reset_data();

	void udp_send_signal_pata_data();

	void udp_send_data();

	void test();
	void comboBox_currentIndexChanged(int);
	void get_gps_data();
};

