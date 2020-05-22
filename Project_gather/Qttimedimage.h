#pragma once

#include <QWidget>
#include "ui_Qttimedimage.h"

#include <QtNetwork>
#include <QAbstractSocket>
class QTcpSocket;

#pragma pack(1)

class Qttimedimage : public QWidget
{
	Q_OBJECT

public:
	Qttimedimage(QWidget *parent = Q_NULLPTR);
	~Qttimedimage();

private:
	Ui::Qttimedimage ui;

	QTcpSocket* ground_detect_handle;

private slots:
	void btn_connect_ground_detect_revpoint();

	void tcp_rev_data_from_ground_detect();

	void tcp_send_data_trans_req_to_ground_detect();

	void btn_to_project_gather();
};

typedef struct data_trans_req_frame
{
	quint16 frame_head;
	quint8 frame_type;
	quint16 frame_len;
	quint16 frame_data;
	quint8 frame_sum;
}data_trans_req_frame_t;

typedef struct data_tran_frame
{
	quint16 frame_head;
	quint8 frame_type;
	quint16 frame_len;
	quint8 frame_aisle;
	quint8 frame_serial;
	quint64 frame_time;
	quint8* frame_data;
	quint8 frame_sum;
}data_tran_frame_t;
