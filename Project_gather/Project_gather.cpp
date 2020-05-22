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
}

void Project_gather::btn_login()
{
	login = new QtLogin;
	login->show();
	this->hide();
}

void Project_gather::btn_startdetection()
{
	ui.btn_wirednetwork->setChecked(1);
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

/*---------���˿ص�TCP����----------*/
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
	ui.plainTextEdit->appendPlainText("�����˿����ķ������ɹ�");
}

QTcpSocket* Project_gather::get_tcp_to_ground_handle()
{
	return gtcp_to_ground_ctl_handle;
}

/*---------���ؼ�������TCP����----------*/
QTcpSocket* gp_tcp_to_ground_detect_handle;
void Project_gather::tcp_connect_ground_detect_as_data_transmission()
{
	tcp_to_ground_detect_handle = new QTcpSocket(this);
	tcp_to_ground_detect_handle->connectToHost("192.168.2.228", 10086);
	connect(tcp_to_ground_detect_handle, SIGNAL(connected()), this, SLOT(tcp_connected_ground_detect_success()));
	gp_tcp_to_ground_detect_handle = tcp_to_ground_detect_handle;
}

void Project_gather::tcp_connected_ground_detect_success()
{
	ui.plainTextEdit->appendPlainText("���ӵؼ������ɹ�");
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
	ui.plainTextEdit->appendPlainText("�Ͽ��ؼ�����");
}

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
	ui.plainTextEdit->appendPlainText("�������߳ɹ�");
}

QTcpSocket* Project_gather::get_ant_handle()
{
	return gp_tcp_to_ant_handle;
}