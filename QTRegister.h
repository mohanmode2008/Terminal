#pragma once

#include <QWidget>
#include "ui_QTRegister.h"

#include <QTcpSocket>
#include <QString>

class QTRegister : public QWidget
{
	Q_OBJECT

public:
	QTRegister(QWidget *parent = Q_NULLPTR);
	~QTRegister();

private:
	Ui::QTRegister ui;
	QTRegister* register1;


private slots:
	void on_btn_switchlogin_clicked();
	void on_btnRegister_clicked();
	void btn_quit();


public:
	QTcpSocket* socket;

};
