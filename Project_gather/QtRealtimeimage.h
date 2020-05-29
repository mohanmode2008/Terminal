#pragma once

#include <QWidget>
#include "ui_QtRealtimeimage.h"
#include <QTcpSocket> 
#include <QUdpSocket> 
#pragma pack(1)

class QtRealtimeimage : public QWidget
{
	Q_OBJECT

public:
	QtRealtimeimage(QWidget *parent = Q_NULLPTR);
	~QtRealtimeimage();

public slots:
	void btn_to_east();
	void btn_to_west();
	void btn_to_south();
	void btn_to_north();
	void btn_gather();

	void btn_connect_data_server();
	void connect_data_server_success();
	void read_data_server();

	void tcp_send_data_trans_req_to_data_server();
	void tcp_set_param_format_to_data_server();

	void btn_show_picture();

	void btn_udp_connect_to_sky();
	void udp_rev_data_from_sky();

private:
	Ui::QtRealtimeimage ui;

	float float_longitude;
	float float_latitude;

	QTcpSocket* tcpClient;
	QUdpSocket* sender1;

	quint8 check_sum(quint8* a, int len);

	void udp_send_data(quint8* total_data_to_send, quint16 total_data_len);

};

typedef struct data_tran_frame_realtime
{
	quint16 frame_head;
	quint8 frame_type;
	quint16 frame_len;
	quint8 frame_aisle;
	quint8 frame_serial;
	quint64 frame_time;
	quint8* frame_data;
	quint8 frame_sum;
}data_tran_frame_realtime_t;

typedef struct data_trans_req_frame_realtime
{
	quint16 frame_head;
	quint8 frame_type;
	quint16 frame_len;
	quint16 frame_data;
	quint8 frame_sum;
}data_trans_req_frame_realtime_t;
