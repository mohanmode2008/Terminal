#pragma once

#include <QWidget>
#include "ui_QTRegister.h"

#include <QTcpSocket>
#include <QString>

#include <QtNetwork>
#include "QtLogin.h"

class QTRegister : public QWidget
{
	Q_OBJECT

public:
	QTRegister(QWidget *parent = Q_NULLPTR);
	~QTRegister();

private:
	Ui::QTRegister ui;
	QTRegister* register1;

	QNetworkRequest netReq;
	QNetworkAccessManager* manager_post_code;
	QNetworkAccessManager* manager_post_register;
	QNetworkAccessManager* manager_post_edit;

	qint8 return_post_state;

	QFile *file;// ("D:/VS_QT/project_gather/operation_record.txt");

public slots:
	void btn_post_register();
	void finished_post_register(QNetworkReply*);
	int btn_post_getcode();
	void finished_post_getcode(QNetworkReply*);

	void btn_login();

	int btn_post_edit();
	void finished_post_edit(QNetworkReply*);
	void btn_quit();
};


