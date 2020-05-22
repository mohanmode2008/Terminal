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
	QNetworkAccessManager* manager_post_login;

	QString  id_token;

	QNetworkReply* reply;

	QNetworkAccessManager * manager_get_logout;

	QNetworkAccessManager * manager_get_id;

	QNetworkAccessManager * manager_get_regiter_dev;

	QNetworkAccessManager networkAccessManager_get_dev;

	QNetworkAccessManager *regiser_dev_handle;

	QFile* file;
	QString get_usr_id();
	QString get_local_ip();
	QString get_local_mac();

private slots:
	void btn_post_login();
	void finished_post_login(QNetworkReply*);

	void btn_return_status_show();
	void btn_register();

	void btn_post_logout();
	void finished_post_logout();

	void btn_quit();
	void btn_configurate_imageparameter();

	void btn_get_usrid();
	void finished_get_usrid(QNetworkReply* reply);

	void btn_post_regiser_dev();
	void finished_post_regiser_dev(QNetworkReply* reply);

	void btn_get_dev_interface();
	void finished_get_dev(QNetworkReply* reply);

};


