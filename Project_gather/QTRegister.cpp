#include "QTLogin.h"
#include "QTRegister.h"

QTRegister::QTRegister(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->socket = new QTcpSocket(this);
}

QTRegister::~QTRegister()
{
}

void QTRegister::on_btn_switchlogin_clicked ()
{
	QtLogin* Project_login = new QtLogin;
	Project_login->show();
	this->hide();
}


void QTRegister::on_btnRegister_clicked()

{

	QString str0 = ui.lineEdit->text();
	QString str1 = ui.lineEdit_2->text();
//	QString str2 = ui.repeatpassword->text();
	//QString str3 = ui.lineEdit_4->text();

	QByteArray arr;

	str0.append(str1);
	//str0.append(str2);
//	str0.append(str3);

	this->socket->connectToHost("192.168.0.70", 80100, QTcpSocket::ReadWrite);

	QDataStream dst(&arr, QIODevice::ReadWrite);  //QDataStream�Ǽ̳���QIODevice������ ����Ҫ ���ö�дȨ��

	dst << QString::fromLocal8Bit("�����ڿͻ��ˣ�") << str0;  //����һ��Ҫ���ú���������

	socket->write(arr);

}

void QTRegister::btn_quit()
{
	this->close();
}