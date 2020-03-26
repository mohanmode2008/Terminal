/*



TCP Client



���뻷����VS2017_Qt5.12.4



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

	ui.statusBar->showMessage(QString::fromLocal8Bit("���ӷ������ɹ�"));

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



	//Ϊ��ͻ������Ч����������Ϣ������ʾ

	ui.plainTextEdit->appendPlainText(str1);

	ui.plainTextEdit->appendPlainText(str2);

	ui.plainTextEdit->appendPlainText("");

}



//������Ϣ��������

void Project_gather::on_btnSend_clicked()

{

	QString str = ui.lineEdit->text();

	QByteArray arr;

	QDataStream dst(&arr, QIODevice::ReadWrite);  //QDataStream�Ǽ̳���QIODevice������ ����Ҫ ���ö�дȨ��

	dst << QString::fromLocal8Bit("�����ڿͻ��ˣ�") << str;  //����һ��Ҫ���ú���������

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
