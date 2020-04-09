#pragma once

#include <QWidget>
#include "ui_QtLogin.h"

#include "QTRegister.h"
#include "QtWorkparameter.h"

#include <QTcpSocket>
#include <QString>


class QtLogin : public QWidget
{
	Q_OBJECT

public:
	QtLogin(QWidget *parent = Q_NULLPTR);
	~QtLogin();

	QString get_id_token();

private:
	Ui::QtLogin ui;

	QNetworkRequest netReq;
	QNetworkAccessManager* networkAccessManager_post_login;

	QString  id_token;

	QNetworkReply* reply;

	QNetworkAccessManager manager_get_logout;

	QFile* file;

private slots:
	void btn_post_login();
	void finished_post_login(QNetworkReply*);

	void btn_return_status();
	void btn_to_register();

	void btn_post_logout();
	void finished_post_logout();

	void btn_to_quit();
	void btn_to_Workparameter();
};


