#include "QtLogin.h"

#include "Project_gather.h"
#include "QTRegister.h"


#include <QMessageBox>

#include<Windows.h>

#include <QboxLayout.h>

QtLogin::QtLogin(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	this->socket = new QTcpSocket(this);

}

QtLogin::~QtLogin()
{
}

void QtLogin::on_btn_returnstatus_clicked ()
{
	Project_gather* Project_login = new Project_gather;
	Project_login->show();
	this->hide();
}

void QtLogin::on_btn_register_clicked ()
{
	Register1 = new QTRegister;
	Register1->show();
	this->hide();
}


void QtLogin::on_btn_login_send_clicked()

{

	QString str0 = ui.Edit_name->text();
	QString str1 = ui.Edit_password->text();

	//Project_gather* Project_login = new Project_gather;

	QByteArray arr;

	str0.append(str1);

	this->socket->connectToHost("192.168.0.70", 80100, QTcpSocket::ReadWrite);

	//ui.textEdit->setText(str0);
	//	appendPlainText(str2);
	QDataStream dst(&arr, QIODevice::ReadWrite);  //QDataStream是继承于QIODevice的所以 必须要 设置读写权限

	dst << QString::fromLocal8Bit("来自于客户端：") << str0;  //这里一定要设置好数据类型


		QByteArray bytes = str0.toUtf8();

	  ui.textEdit->setText(bytes);

		

		socket->write(arr);

}

void QtLogin::btn_quit()
{
	this->close();
}
