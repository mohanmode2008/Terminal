#pragma once

#include <QWidget>
#include "ui_Qtpower.h"

class Qtpower : public QWidget
{
	Q_OBJECT

public:
	Qtpower(QWidget *parent = Q_NULLPTR);
	~Qtpower();

private:
	Ui::Qtpower ui;

	QTimer* timer;

	quint16 get_crc16_modbus(char* ptr, int len);

	void shake_hand_reply();
	void BMS_data_reply();
	void BMS_serial_voltage_data_replay();

	void BMS_control_instruction_replay();

private slots:
	void warning_pop();
	void btn_to_state();

	void init_timer();
	void timer_time_out();
	void stop_timer();
	void btn_shake_hand();

	void btn_BMS_data();
	void btn_BMS_serial_voltage_data();
	void btn_BMS_control_instruction_on();
	void btn_BMS_control_instruction_off();

};

typedef struct power
{
	quint8 frmae_head;
	quint8 soft_version;
	quint8 cmd_code0;
	quint8 cmd_code1;
	quint8 data_len;
	quint8 id_len;
	quint16 usr_id;
	quint8* data_contect;
	quint16 frame_crc;
	quint8 end_code;
}power_t;

typedef struct shake_hand_reply
{
	quint8 cmd_code0;
	quint8 cmd_code1;
	quint16 dev_type;
	quint16 soft_version;
	quint16 soft_data;
	quint16 dev_serial0;
	quint16 dev_serial1;
	quint16 dev_serial2;
	quint32 mcu_id0;
	quint32 mcu_id1;
	quint32 mcu_id2;
	quint64 author_code;
	quint8 protocol_type;
	quint16 protocol_version;
}shake_hand_reply_t;

typedef struct BMS_data_reply
{
	quint8 cmd_code0;
	quint8 cmd_code1;
	quint16 voltage;
	quint16 Current;
	quint16 capacity;
	quint16 SOC;
	quint16 loop_time;
	quint8 string_num;
	quint16 proportionate_state;
	quint16 proportionate_state_high;
	quint16 protect_state;
	quint8 run_state;
	quint8 NTC_num;
	quint8* NTC_condect;
	quint16 protect_time;
	quint16 restart_time;
	quint16 temp_protect_inform;
	quint8* temp_serial;
}BMS_data_reply_t;

typedef struct BMS_serial_voltage_data
{
	quint8 cmd_code0;
	quint8 cmd_code1;
}BMS_serial_voltage_data_t;