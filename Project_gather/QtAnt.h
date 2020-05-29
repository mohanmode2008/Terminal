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

	QTcpSocket* tcp_to_ant_handle;
	QTcpSocket* tcp_to_ground_detect_handle;

	QTimer* timer;

	quint8 check_sum1(quint8* a, int len);
	
	void analyze_param_setting_req(QByteArray bytearray);
	void analyze_dev_status(QByteArray bytearray);

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

	void init_timer();
	void timer_time_out();

	void btn_set_rev_ant_data();
	void read_ant_data();

	void btn_set_param_format_to_data_server();
	void btn_set_ground_detect_rev();
	void tcp_rev_data_from_ground_detect();
};

typedef struct data_trans_req_frame
{
	quint16 frame_head;
	quint8 frame_type;
	quint16 frame_len;
	quint16 frame_data;
	quint8 frame_sum;
}data_trans_req_frame_t;