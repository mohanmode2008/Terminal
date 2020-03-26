#pragma once

#include <QWidget>
#include "ui_QtLogin.h"

#include <QtNetwork>



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



public slots:
	void btn_post_login();
	void finished_post_login(QNetworkReply*);

	void turn_verification();
	void turn_register();

	void pushButton_get_clicked_logout();
	void get_logout_finished();
};
