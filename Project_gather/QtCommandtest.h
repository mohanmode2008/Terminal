#pragma once

#include <QWidget>
#include "ui_QtCommandtest.h"



#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>


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

	qint16 crc16_ccitt(qint8 data, qint16 crc);

public slots:
	void trigerMenu(QAction* act);

	void btn_first_clicked();

	void lineeditEnable();

	void btn_to_selectlist();
	void btn_to_del_select();
	void select_lineeditEnable();
	void btn_to_buildup_validdata();


private:
	Ui::QtCommandtest ui;

	QString select_in_list;
	QString select_in_cmd;
	int row_in_cmd;
	QMap<QString, int> map_name_to_code;

	QString data_stream;
		QString conduct_code;
		QString sequence_start_byte;
		QString sequence_end_byte;
		QString protect_sequence;

		QString up_data_frame;
			QString satellite_syn_byte;
			QString authorize_byte;
			QString pattern_byte;
			QString cmd_type;
			QString single_serial_number;
			QString cmd_code;
			QString thirteenth_byte_to_string;
				qint8 ACK_reply;
				qint8 cmd_attribute;
				qint8 single_multiframe;
			QString cmd_run_time;
			QString multiframe_count;

			QString cmd_valid_data;

			QString CRC_byte;
			QString frame_end_byte;

};
