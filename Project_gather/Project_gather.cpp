#include "Project_gather.h"
#include <QMessageBox>
#include<Windows.h>
#include <QboxLayout.h>
#include "QtCommandtest.h"
#include "QtAnt.h"

#pragma execution_character_set("utf-8")

Project_gather::Project_gather(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.btn_to_data_trans, SIGNAL(clicked()), this, SLOT(tcp_connect_ground_detect_as_data_transmission()));
	connect(ui.btn_to_commandtest, SIGNAL(clicked()), this, SLOT(btn_cmdtest()));
	connect(ui.btn_to_ant, SIGNAL(clicked()), this, SLOT(btn_ant()));
	connect(ui.btn_to_connect_ground_ctl_as_client, SIGNAL(clicked()), this, SLOT(tcp_connect_ground_ctl_as_client()));
	connect(ui.btn_to_login, SIGNAL(clicked()), this, SLOT(btn_login()));
	connect(ui.btn_to_quit, SIGNAL(clicked()), this, SLOT(btn_quit()));
	connect(ui.btn_to_Disconnect, SIGNAL(clicked()), this, SLOT(btn_disconnect_ground_detect()));
	connect(ui.btn_to_connect_ant, SIGNAL(clicked()), this, SLOT(btn_connect_ant()));
	connect(ui.btn_to_power, SIGNAL(clicked()), this, SLOT(btn_power()));	

	connect(ui.checkBox_1, SIGNAL(stateChanged(int)), this, SLOT(check_box_1_status_change(int)));
	connect(ui.checkBox_2, SIGNAL(stateChanged(int)), this, SLOT(check_box_2_status_change(int)));
	connect(ui.checkBox_3, SIGNAL(stateChanged(int)), this, SLOT(check_box_3_status_change(int)));

	//查找可用的串口
	foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
	{
		QSerialPort serial;
		serial.setPort(info);
		if (serial.open(QIODevice::ReadWrite))
		{
			ui.portBox->addItem(serial.portName());
			serial.close();
		}
	}

	//设置波特率下拉菜单的第0项默认值
	ui.baudBox->setCurrentIndex(0);
}

void Project_gather::btn_login()
{
	login = new QtLogin;
	login->show();
	this->hide();
}

void Project_gather::btn_quit()
{
	this->close();
}

void Project_gather::btn_cmdtest()
{
	QtCommandtest* commandtest = new QtCommandtest;
	commandtest->show();
	this->hide();
}

void Project_gather::btn_ant()
{
	QtAnt* ant = new QtAnt;
	ant->show();
	this->hide();
}

void Project_gather::btn_power()
{
	Qtpower * power = new Qtpower;
	power->show();
	this->hide();
}

void Project_gather::btn_startdetection()
{
	timer = new QTimer;

	timer->setSingleShot(false);
	timer->start(5000);
	connect(timer, SIGNAL(timeout()), this, SLOT(timer_time_out()));
}

void Project_gather::timer_time_out()
{
	qDebug() << "TimerTimeOut";
	if (serial->isOpen() == false)
	{
		//断开连接
	}

	qint64 x = tcp_to_ground_ctl_handle->write("aaaa");

	if (x == -1)
	{
		ui.plainTextEdit->appendPlainText("关闭连接");
	}
	else
	{
		ui.plainTextEdit->appendPlainText("正常连接");
	}
}

/*---------到运控的TCP连接----------*/
static QTcpSocket* gtcp_to_ground_ctl_handle = Q_NULLPTR;
void Project_gather::tcp_connect_ground_ctl_as_client()
{
	tcp_to_ground_ctl_handle = new QTcpSocket(this);
	tcp_to_ground_ctl_handle->connectToHost("192.168.2.228", 10086);
	connect(tcp_to_ground_ctl_handle, SIGNAL(connected()), this, SLOT(tcp_connected_ground_ctl_success()));
	gtcp_to_ground_ctl_handle = tcp_to_ground_ctl_handle;
}

void Project_gather::tcp_connected_ground_ctl_success()
{
	ui.plainTextEdit->appendPlainText("连接运控中心服务器成功");
}

QTcpSocket* Project_gather::get_tcp_to_ground_handle()
{
	return gtcp_to_ground_ctl_handle;
}

/*---------到地检数传的TCP连接----------*/
QTcpSocket* gp_tcp_to_ground_detect_handle = Q_NULLPTR;
void Project_gather::tcp_connect_ground_detect_as_data_transmission()
{
	tcp_to_ground_detect_handle = new QTcpSocket(this);
	tcp_to_ground_detect_handle->connectToHost("192.168.2.228", 10086);
	connect(tcp_to_ground_detect_handle, SIGNAL(connected()), this, SLOT(tcp_connected_ground_detect_success()));
	gp_tcp_to_ground_detect_handle = tcp_to_ground_detect_handle;
}

void Project_gather::tcp_connected_ground_detect_success()
{
	ui.plainTextEdit->appendPlainText("连接地检数传成功");
}

QTcpSocket* Project_gather::get_ground_detect_handle()
{
	return gp_tcp_to_ground_detect_handle;
}

void Project_gather::btn_disconnect_ground_detect()
{
	tcp_to_ground_detect_handle->disconnectFromHost();
	connect(tcp_to_ground_detect_handle, SIGNAL(disconnected()), this, SLOT(tcp_disconnected_ground_detect()));
}

void Project_gather::tcp_disconnected_ground_detect()
{
	ui.plainTextEdit->appendPlainText("断开地检数传");
}

/*---------到天线的TCP连接----------*/
QTcpSocket* gp_tcp_to_ant_handle;
void  Project_gather::btn_connect_ant()
{
	tcp_to_ant_handle = new QTcpSocket(this);
	QString line_ip = ui.line_ip->text();
	QString line_port = ui.line_port->text();
	int num = line_port.toInt();
	tcp_to_ant_handle->connectToHost(line_ip, num);

	connect(tcp_to_ant_handle, SIGNAL(connected()), this, SLOT(connected_ant_success()));
	gp_tcp_to_ant_handle = tcp_to_ant_handle;
}

void  Project_gather::connected_ant_success()
{
	ui.plainTextEdit->appendPlainText("连接天线成功");
}

QTcpSocket* Project_gather::get_ant_handle()
{
	return gp_tcp_to_ant_handle;
}

void Project_gather::check_box_1_status_change(int state)
{
	if (state == Qt::Checked) // "选中"
	{
		 select_wire = 1;
		 select_wireless = 0;
		 select_shortmessage = 0;
		 ui.checkBox_2->setCheckState(Qt::Unchecked);
		 ui.checkBox_3->setCheckState(Qt::Unchecked);

		ui.plainTextEdit->appendPlainText("有线Checked");
	}
	else if (state == Qt::PartiallyChecked) // "半选"
	{
		ui.plainTextEdit->appendPlainText("PartiallyChecked");
	}
	else // 未选中 - Qt::Unchecked
	{
		ui.plainTextEdit->appendPlainText("有线Unchecked");
	}
}

void Project_gather::check_box_2_status_change(int state)
{
	if (state == Qt::Checked) // "选中"
	{
		select_wire = 0;
		select_wireless = 1;
		select_shortmessage = 0;
		ui.checkBox_1->setCheckState(Qt::Unchecked);
		ui.checkBox_3->setCheckState(Qt::Unchecked);
		ui.plainTextEdit->appendPlainText("无线Checked");
	}
	else if (state == Qt::PartiallyChecked) // "半选"
	{
		ui.plainTextEdit->appendPlainText("PartiallyChecked");
	}
	else // 未选中 - Qt::Unchecked
	{
		ui.plainTextEdit->appendPlainText("无线Unchecked");
	}
}

void Project_gather::check_box_3_status_change(int state)
{
	if (state == Qt::Checked) // "选中"
	{
		select_wire = 0;
		select_wireless = 0;
		select_shortmessage = 1;
		ui.checkBox_1->setCheckState(Qt::Unchecked);
		ui.checkBox_2->setCheckState(Qt::Unchecked);
		ui.plainTextEdit->appendPlainText("短消息Checked");
	}
	else if (state == Qt::PartiallyChecked) // "半选"
	{
		ui.plainTextEdit->appendPlainText("PartiallyChecked");
	}
	else // 未选中 - Qt::Unchecked
	{
		ui.plainTextEdit->appendPlainText("短消息Unchecked");
	}
}

void Project_gather::btn_open_serial()
{
	serial = new QSerialPort;
	if (ui.OpenSerialButton->text() == tr("打开串口"))
	{
		//设置串口名
		serial->setPortName(ui.portBox->currentText());
		//打开串口
		serial->open(QIODevice::ReadWrite);
		//设置波特率
		serial->setBaudRate(QSerialPort::Baud115200);
		//设置数据为
		switch (ui.dataBox->currentIndex())
		{
		case 0:
			serial->setDataBits(QSerialPort::Data8);
			break;
		default:
			break;
		}
		//设置校验位
		switch (ui.checkBox->currentIndex())
		{
		case 0:
			serial->setParity(QSerialPort::NoParity);
			break;
		default:
			break;
		}
		//设置停止为
		switch (ui.stopBox->currentIndex())
		{
		case 0:
			serial->setStopBits(QSerialPort::OneStop);
			break;
		case 1:
			serial->setStopBits(QSerialPort::TwoStop);
			break;
		default:
			break;
		}
		//设置流控制
		serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制

		//关闭设置菜单使能
		ui.portBox->setEnabled(false);
		ui.dataBox->setEnabled(false);
		ui.checkBox->setEnabled(false);
		ui.stopBox->setEnabled(false);
		ui.baudBox->setEnabled(false);
		ui.OpenSerialButton->setText("关闭串口");
		QObject::connect(serial, &QSerialPort::readyRead, this, &Project_gather::serial_Read_from_beidou);
	}
	else
	{
		//关闭串口
		serial->clear();
		serial->close();
		serial->deleteLater();

		//恢复使能
		ui.portBox->setEnabled(true);
		ui.baudBox->setEnabled(true);
		ui.dataBox->setEnabled(true);
		ui.checkBox->setEnabled(true);
		ui.stopBox->setEnabled(true);
		ui.OpenSerialButton->setText("打开串口");
	}
}

void Project_gather::btn_read_cardnumber()
{
	QByteArray send_data("$CCICA,0,00*7B\r\n");
	serial->write(send_data);
}

void Project_gather::btn_read_signalstate()
{
	QByteArray send_data("$CCRMO,BSI,2,0*26\r\n");
	serial->write(send_data);
}

void Project_gather::btn_read_locationdata()
{
	QByteArray send_data("$CCDWA,0000000,V,1,L,,0,,,0*65\r\n");
	serial->write(send_data);
}

void Project_gather::btn_send_msgdata()
{
	QString send_str = ui.send_str->text();
	QByteArray send_data("$CCTXA,0242286,1,1,A468656C6C6FB1B1B6B7*7F");
	int N = send_data.lastIndexOf(",");
	QByteArray aa = send_data.mid(N);
	int  aa_length = aa.length();
	int M = aa.lastIndexOf("*");
	QByteArray bytes = send_str.toLatin1();
	aa.replace(0, M, bytes);
	send_data.replace(N, aa_length, aa);

	serial->write(send_data);
}

void Project_gather::btn_get_local_time()
{
	QByteArray send_data("$CCRMO,ZDA,2,0*21\r\n");
	serial->write(send_data);
}

void Project_gather::serial_Read_from_beidou()
{
	QString buffer_1;
	QByteArray buffer = serial->readAll();
	
	if (!buffer.isEmpty())
	{  
		if (buffer.startsWith("$BDICI"))
		{
			analyze_card_ID(buffer);
		}
		else if (buffer.startsWith("$BDBSI"))
		{
			analyze_signal_state(buffer);
		}
		else if (buffer.startsWith("$BDFKI"))
		{
			QString string = buffer;
			QString wave_speed1 = string.section(',',2,1);
			if (wave_speed1 =="Y")
			{
				ui.plainTextEdit->appendPlainText("定位指令执行成功!");
			}			
		}
		else if (buffer.startsWith("$BDDWR"))
		{
			analyze_locationdata(buffer);
		}
		else if (buffer.startsWith("$BDFK"))
		{
			QString string = buffer;
			QString wave_speed1 = string.section(',', 2, 1);
			if (wave_speed1 == "Y")
			{
				ui.plainTextEdit->appendPlainText("短消息指令执行成功!");
			}
		}
		else if (buffer.startsWith("$BDTXR"))
		{
			analyze_msgdata(buffer);
		}
		else if (buffer.startsWith("$BDZDA"))
		{
			analyze_local_time(buffer);
		}
	}
	buffer.clear();
}

void Project_gather::analyze_card_ID(QByteArray buffer)
{
	QString string = buffer;   // QByteArray转QString方法1

	QString card_ID = string.section(',', 1, 1);

	ui.plainTextEdit->appendPlainText("card_ID");
	ui.plainTextEdit->appendPlainText(card_ID);
}

void Project_gather::analyze_signal_state(QByteArray buffer)
{
	QString string = buffer;
	QString wave_speed1 = string.section(',', 3, 1);
	QString wave_speed2 = string.section(',', 4, 1);
	ui.plainTextEdit->appendPlainText(wave_speed1);
	ui.plainTextEdit->appendPlainText(wave_speed2);
}

void Project_gather::analyze_locationdata(QByteArray buffer)
{
	QString string = buffer;
	QString latitude = string.section(',', 4, 1);
	QString latitude_direction = string.section(',', 5, 1);
	QString longitude = string.section(',', 4, 1);
	QString longitude_direction = string.section(',', 5, 1);

	ui.plainTextEdit->appendPlainText(latitude);
	ui.plainTextEdit->appendPlainText(latitude_direction);
	ui.plainTextEdit->appendPlainText(longitude);
	ui.plainTextEdit->appendPlainText(longitude_direction);
}

void Project_gather::analyze_msgdata(QByteArray buffer)
{
	QString string = buffer;
	QString send_addr = string.section(',', 2, 1);
	QString message_type = string.section(',', 3, 1);
	QString message_context = string.section(',', 5, -1);
	message_context = string.indexOf('*');

	ui.plainTextEdit->appendPlainText(send_addr);
	ui.plainTextEdit->appendPlainText(message_type);
	ui.plainTextEdit->appendPlainText(message_context);
}

void Project_gather::analyze_local_time(QByteArray buffer)
{
	QString string = buffer;
	QString UTC_time = string.section(',', 2, 1);
	QString day = string.section(',', 3, 1);
	QString month = string.section(',', 4, 1);
	QString year = string.section(',', 5, 1);

	ui.plainTextEdit->appendPlainText("UTC_time" + UTC_time);
	ui.plainTextEdit->appendPlainText("day" + day);
	ui.plainTextEdit->appendPlainText("month" + month);
	ui.plainTextEdit->appendPlainText("year"+ year);
}

