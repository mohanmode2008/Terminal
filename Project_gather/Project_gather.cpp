/*



TCP Client



代码环境：VS2017_Qt5.12.4



*/

#include "Project_gather.h"

#include <QMessageBox>

#include<Windows.h>

#include <QboxLayout.h>

#include "QtCommandtest.h"


Project_gather::Project_gather(QWidget* parent)

	: QMainWindow(parent)

{

	ui.setupUi(this);



	this->socket = new QTcpSocket(this);

}



void Project_gather::on_btnConServer_clicked()

{

	this->socket->connectToHost("192.168.0.70", 80100, QTcpSocket::ReadWrite);

	connect(this->socket, SIGNAL(connected()), this, SLOT(slotConnect()));

}



void Project_gather::slotConnect()

{

	ui.statusBar->showMessage(QString::fromLocal8Bit("连接服务器成功"));

	connect(this->socket, SIGNAL(readyRead()), this, SLOT(readyread()));

}



void Project_gather::on_btnDiscon_clicked()

{

	this->socket->close();

}



void Project_gather::readyread()

{

	QByteArray arr = socket->readAll();

	QDataStream* dst = new QDataStream(&arr, QIODevice::ReadOnly);

	QString str1;

	QString str2;

	(*dst) >> str1 >> str2;



	//为了突出聊天效果，两条消息换行显示

	ui.plainTextEdit->appendPlainText(str1);

	ui.plainTextEdit->appendPlainText(str2);

	ui.plainTextEdit->appendPlainText("");

}



//发送消息给服务器

void Project_gather::on_btnSend_clicked()

{

	QString str = ui.lineEdit->text();

	QByteArray arr;

	QDataStream dst(&arr, QIODevice::ReadWrite);  //QDataStream是继承于QIODevice的所以 必须要 设置读写权限

	dst << QString::fromLocal8Bit("来自于客户端：") << str;  //这里一定要设置好数据类型

	socket->write(arr);

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

void Project_gather::btn_first_clicked()
{
	QtCommandtest* Project_login = new QtCommandtest;
	Project_login->show();
	this->hide();
}
