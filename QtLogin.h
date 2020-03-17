#pragma once

#include <QWidget>
#include "ui_QtLogin.h"

#include "QTRegister.h"

#include <QTcpSocket>
#include <QString>


class QtLogin : public QWidget
{
	Q_OBJECT

public:
	QtLogin(QWidget *parent = Q_NULLPTR);
	~QtLogin();

private:
	Ui::QtLogin ui;

	QTRegister* Register1;

private slots:
	void on_btn_returnstatus_clicked();
	void on_btn_register_clicked();
	void on_btn_login_send_clicked();
	void btn_quit();


public:
	QTcpSocket* socket;
};


