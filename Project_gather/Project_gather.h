#include <QtWidgets/QMainWindow>
#include "ui_Project_gather.h"
#include <QTcpSocket>
#include <QString>
#include <QDataStream>
#include <QByteArray>
#include "QTLogin.h"
#include "Qtpower.h"


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

private:
	Ui::Project_gatherClass ui;
	QtLogin* login;	
};