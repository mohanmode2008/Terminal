#include <QtWidgets/QMainWindow>
#include "ui_Project_gather.h"
#include <QTcpSocket>
#include <QString>
#include <QDataStream>
#include <QByteArray>
#include "QTLogin.h"

class Project_gather : public QMainWindow
{
	Q_OBJECT

public:
	Project_gather(QWidget* parent = Q_NULLPTR);
	QTcpSocket* socket;
	
private slots:
	void on_btnConServer_clicked();
	void on_btnDiscon_clicked();
	void slotConnect();
	void readyread();
	void on_btnSend_clicked();
	void btn_login();
	void btn_startdetection();
	void btn_quit();
	void btn_cmdtest();
	void btn_to_ant();

private:
	Ui::Project_gatherClass ui;
	QtLogin* login;

};