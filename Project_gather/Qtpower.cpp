#include "Qtpower.h"
#include <QMessageBox> 
#include "Project_gather.h"


Qtpower::Qtpower(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(warning_pop()));
	connect(ui.btn_to_state, SIGNAL(clicked()), this, SLOT(btn_to_state()));
	connect(ui.btn_to_init_timer, SIGNAL(clicked()), this, SLOT(init_timer()));
	connect(ui.btn_to_stop_timer, SIGNAL(clicked()), this, SLOT(stop_timer()));
	
	
	
}

Qtpower::~Qtpower()
{
}

void Qtpower::warning_pop()
{
	QMessageBox::warning(NULL, "warning", "Do you want to process?", QMessageBox::Ok | QMessageBox::Cancel , QMessageBox::Ok);

}

void Qtpower::btn_to_state()
{
	Project_gather* Project_login = new Project_gather;
	Project_login->show();
	this->hide();
}

void Qtpower::init_timer()
{
	timer = new QTimer;

	timer->setSingleShot(false);

	timer->start(5000);

	connect(timer, SIGNAL(timeout()), this, SLOT(timer_time_out()));
}

void Qtpower::timer_time_out()
{
	qDebug() << "TimerTimeOut";
	warning_pop();
}

void Qtpower::stop_timer()
{
	qDebug() << "TimerTimestop";

    if (timer->isActive())
		timer->stop();  
}

quint16 Qtpower::get_crc16_modbus(char * ptr,int len)
{
	quint16 crc = 0xffff;
	if (len == 0)
	{
		return 0;
	}
	for (int i = 0; i < len; i++)
	{
		crc ^= *(ptr + i);
		for (int k = 0; k < 8; k++)
		{
			if ((crc & 0x01) != 0)
			{
				crc >>= 1;
				crc ^= 0xa001;
			}
			else
			{
				crc >>= 1;
			}
		}
	}
	return crc;
}

void Qtpower::btn_shake_hand()
{
	power_t shake_hand_frame;

	shake_hand_frame.frmae_head = 0xdd;
	shake_hand_frame.soft_version = 0x0d;
	shake_hand_frame.cmd_code0 = 0x00;
	shake_hand_frame.cmd_code1 = 0x0a;
	shake_hand_frame.data_len = 0x03;
	shake_hand_frame.id_len = 2;
	shake_hand_frame.usr_id = 0x0001;
	shake_hand_frame.frame_crc = get_crc16_modbus((char*)&shake_hand_frame.soft_version,7);
	shake_hand_frame.end_code = 0x77;
}

void Qtpower::shake_hand_reply()
{
	QByteArray buf;
	//buf = serial->readAll();
	if (!buf.isEmpty())
	{
		QString str = buf;
		//ui->txtRead->appendPlainText(str);
	}

	QDataStream out(&buf, QIODevice::ReadWrite);    //将字节数组读入
	shake_hand_reply_t shake_hand_reply;
	while (!out.atEnd())
	{
		out >> shake_hand_reply.cmd_code0;
		out >> shake_hand_reply.cmd_code1;
		out >> shake_hand_reply.dev_type;
		out >> shake_hand_reply.soft_version;
		out >> shake_hand_reply.soft_data;
		out >> shake_hand_reply.dev_serial0;
		out >> shake_hand_reply.dev_serial1;
		out >> shake_hand_reply.dev_serial2;
		out >> shake_hand_reply.mcu_id0;
		out >> shake_hand_reply.mcu_id1;
		out >> shake_hand_reply.mcu_id2;
		out >> shake_hand_reply.author_code;
		out >> shake_hand_reply.protocol_type;
		out >> shake_hand_reply.protocol_version;
	}
	buf.clear();
}

void Qtpower::btn_BMS_data()
{
	quint8* apply_data_frame;
	apply_data_frame = new quint8[2];

	memset(apply_data_frame,0x0303,2);
}

void Qtpower::BMS_data_reply()
{
	BMS_data_reply_t BMS_data_reply;

	QByteArray buf;
	//buf = serial->readAll();
	if (!buf.isEmpty())
	{
		QString str = buf;
	//	ui->txtRead->appendPlainText(str);
	}

	QDataStream out(&buf, QIODevice::ReadWrite);    //将字节数组读入
	while (!out.atEnd())
	{
		out >> BMS_data_reply.cmd_code0;
		out >> BMS_data_reply.cmd_code1;
		out >> BMS_data_reply.voltage;
		out >> BMS_data_reply.Current;
		out >> BMS_data_reply.capacity;
		out >> BMS_data_reply.SOC;
		out >> BMS_data_reply.loop_time;
		out >> BMS_data_reply.string_num;
		out >> BMS_data_reply.proportionate_state;
		out >> BMS_data_reply.proportionate_state_high;
		out >> BMS_data_reply.protect_state;
		out >> BMS_data_reply.run_state;
		out >> BMS_data_reply.NTC_num;

		BMS_data_reply.NTC_condect = new quint8[BMS_data_reply.NTC_num];
		for (quint16 i = 0; i < BMS_data_reply.NTC_num; i++)
		{
			out >> BMS_data_reply.NTC_condect[i];
		}
		out >> BMS_data_reply.protect_time;
		out >> BMS_data_reply.restart_time;
		out >> BMS_data_reply.temp_protect_inform;

		BMS_data_reply.temp_serial = new quint8[BMS_data_reply.NTC_num/2];
		for (quint16 i = 0; i < BMS_data_reply.NTC_num/2; i++)
		{
			out >> BMS_data_reply.temp_serial[i];
		}
	}

	quint8 over_voltage = BMS_data_reply.protect_state & 0x01;
	quint8 under_voltage = BMS_data_reply.protect_state & 0x01;

	buf.clear();
}

void Qtpower::btn_BMS_serial_voltage_data()
{
	BMS_serial_voltage_data_t BMS_serial_voltage_data;
	BMS_serial_voltage_data.cmd_code0 = 0x04;
	BMS_serial_voltage_data.cmd_code0 = 0x04;
}

void Qtpower::BMS_serial_voltage_data_replay()
{
	QByteArray buf;
	//buf = serial->readAll();
	if (!buf.isEmpty())
	{
		QString str = buf;
	//	ui->txtRead->appendPlainText(str);
	}

	QDataStream out(&buf, QIODevice::ReadWrite);    //将字节数组读入
	quint16 battery_serial;

	while (!out.atEnd())
	{
		out >> battery_serial;
	}
	quint16 *voltage = new quint16[battery_serial];

	for (quint16 i = 0; i < battery_serial; i++)
	{
		out >> voltage[i];
	}
}

void Qtpower::btn_BMS_control_instruction_on()
{
	quint16 control_instruction_on = 0x0301;
}

void Qtpower::btn_BMS_control_instruction_off()
{
	quint16 control_instruction_on = 0x0313;
}

void Qtpower::BMS_control_instruction_replay()
{
	QByteArray buf;
	//buf = serial->readAll();
	if (!buf.isEmpty())
	{
		QString str = buf;
	//	ui->txtRead->appendPlainText(str);
	}

	QDataStream out(&buf, QIODevice::ReadWrite);    //将字节数组读入
	quint16 control_instruction_result;

	while (!out.atEnd())
	{
		out >> control_instruction_result;
	}
}