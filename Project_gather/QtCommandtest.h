#pragma once

#include <QWidget>
#include "ui_QtCommandtest.h"

#include <QtNetwork>

#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>

#include <QAbstractSocket>
class QTcpSocket;

#pragma pack (1)

#include <vector>

typedef struct telecontrol_frame_up
{
	quint8 satellite_syn_byte[4];		
	quint8 pattern_byte;
	quint8 authorize_byte[4];
	quint8 cmd_style;
	quint8 single_number;
	quint8 cmd_code;
	quint8 ACK_cmdattribute_multiframe;
	quint32 cmd_run_time;
	quint16 multiframe_count;
}telecontrol_frame_up_t;

typedef struct telecontrol_frame_down
{
	quint16 CRC_byte;
	quint16 frame_end_byte;
}telecontrol_frame_down_t;

typedef  struct validdata {
	telecontrol_frame_up_t telecontrol_frame_up;
//	quint8 validdata_len;
	quint8 validdata;
	telecontrol_frame_down_t telecontrol_frame_down;
}validdata_byte_1_t;

typedef  struct validdata_2 {
	telecontrol_frame_up_t telecontrol_frame_up;
	quint8 validdata_len;
	quint8 validdata[2];
	telecontrol_frame_down_t telecontrol_frame_down;
}validdata_byte_2_t;

/*---------------Ò£²âÊý¾ÝÖ¡----------------------*/

typedef struct rev_valid_data_head
{
	quint8 total_data_len[2];
	quint8 cmd_type;
	quint8 single_number;
	quint8 cmd_code;
	quint8 ACK_type;
	quint8 ACK_result;
}rev_valid_data_head_t;

typedef struct log_download
{
	quint16 total_len;
	quint8 total_len_now;
	quint16 frmae_serial;
	quint32 log_notes_1;
	quint32 log_notes_time_1;
	quint8 log_type_1;
	quint8 log_content_1[100];
	quint32 log_notes_2;
	quint32 log_notes_time_2;
	quint8 log_type_2;
	quint8 log_content_2[100];

}log_download_t;

typedef struct visit_record_download
{
	quint16 total_len;
	quint8 total_len_now;
	quint16 frmae_serial;
	quint32 visit_record_1;
	quint32 visit_record_time_1;
	quint8 visit_record_type_1;
	quint8 visit_record_content_1[100];
	quint32 visit_record_2;
	quint32 visit_record_time_2;
	quint8 visit_record_type_2;
	quint8 visit_record_content_2[100];
}visit_record_download_t;

typedef struct short_message_download
{
	quint8 total_len;
	quint16 message_serial;
	quint32 message_rev_time;
	quint32 send_terminal_ID;
	quint32 send_usr_ID;
	quint32 rev_usr_ID;
	quint8 message_record[140];
}short_message_download_t;

class QtCommandtest : public QWidget
{
	Q_OBJECT

public:
	QtCommandtest(QWidget *parent = Q_NULLPTR);
	~QtCommandtest();

	QMenu* menu[10];
	QAction* act[10];
	QMenuBar* menuBar;
	QStatusBar* status;

	quint16 crc16_ccitt(int len, quint8* data);
	quint32 byteOrderChange32Bit(quint32* data);
	quint16 byteOrderChange16Bit(quint16 data);

public slots:
	void trigerMenu(QAction* act);

	void btn_to_gather();
	void btn_clr_cmd();
	void btn_to_selectlist();
	void btn_to_del_select();
	void btn_to_buildup_validdata();
	qint8 btn_to_send_cmd();

	qint8 select_lineeditEnable();
	qint8 update_ReadServer_data();	
	void lineeditEnable();
	void connected_success();

	void btn_to_connect_tcp();

	void by_udp_connect_to_sky();

	void udp_rev_data_from_sky();

	void udp_send_device_reset_data();

	void udp_send_signal_pata_data();

	void udp_send_data();


private:
	Ui::QtCommandtest ui;

	QString select_in_list;
	QString select_in_cmd;
	int row_in_cmd;
	QMap<QString, int> map_name_to_code;

	QMap<QString, int> map_name_to_validdatalength;

	telecontrol_frame_up_t telecontrol_frame_up;

	telecontrol_frame_down_t telecontrol_frame_down;

	quint8 ACK_reply;
	quint8 cmd_attribute;
	quint8 single_multiframe;
	quint16 CRC_byte;

	QTcpSocket* m_tcpClient;

	QUdpSocket* sender1;


	/*-----------Ò£²â½âÎö¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª*/
	quint32* UTC_time = NULL;
	quint32* sys_run_time = NULL;
	quint16* sys_restart_time = NULL;
	quint16* sys_reset_time = NULL;
	quint16* cmd_err_cnt = NULL;
	quint16* cmd_cnt = NULL;

	log_download_t log_download;
	visit_record_download_t visit_record_download;
	short_message_download_t short_message_download;

};


typedef struct rev_data
{

	quint8 satellite_syn_byte[4];
	quint8 authorize_byte[4];
	quint8 frame_head[7];
	quint8 usr_id[4];
	quint8 terminal_id[4];
	quint8 valid_data[231];
	quint8 fault_tolerance[2];
}rev_data_t;

