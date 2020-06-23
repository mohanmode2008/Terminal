#include <QtWidgets/QMainWindow>
#include "ui_Project_gather.h"
#include <QTcpSocket>
#include <QString>
#include <QDataStream>
#include <QByteArray>
#include "QTLogin.h"
#include "Qtpower.h"

//引入qt中串口通信需要的头文件
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


class Project_gather : public QMainWindow
{
	Q_OBJECT

public:
	Project_gather(QWidget* parent = Q_NULLPTR);

	QTcpSocket* tcp_to_ground_ctl_handle;
	QTcpSocket* get_tcp_to_ground_handle();

	QTcpSocket* tcp_to_ground_detect_handle;
	QTcpSocket* get_ground_detect_handle();	

	QTcpSocket* tcp_to_ant_handle;
	QTcpSocket* get_ant_handle();
	
private slots:
	void btn_login();
	void btn_startdetection();
	void btn_quit();
	void btn_cmdtest();
	void btn_ant();
	void btn_power();

	void tcp_connect_ground_ctl_as_client();
	void tcp_connected_ground_ctl_success();

	void tcp_connect_ground_detect_as_data_transmission();
	void tcp_connected_ground_detect_success();

	void btn_disconnect_ground_detect();
	void tcp_disconnected_ground_detect();

	void btn_connect_ant();
	void connected_ant_success();

	void check_box_1_status_change(int);
	void check_box_2_status_change(int);
	void check_box_3_status_change(int);

	void btn_read_cardnumber();
	void btn_read_signalstate();
	void btn_read_locationdata();
	void btn_send_msgdata();
	void btn_get_local_time();
	void serial_Read_from_beidou();
	void btn_open_serial();
	void timer_time_out();

private:
	Ui::Project_gatherClass ui;
	QtLogin* login;	

	quint8 select_wire;
	quint8 select_wireless;
	quint8 select_shortmessage;

	QSerialPort* serial;//全局的串口对象

	QTimer* timer;

	void analyze_card_ID(QByteArray buffer);
	void analyze_signal_state(QByteArray buffer);
	void analyze_locationdata(QByteArray buffer);	
	void analyze_msgdata(QByteArray buffer);
	void analyze_local_time(QByteArray buffer);
	
	
};