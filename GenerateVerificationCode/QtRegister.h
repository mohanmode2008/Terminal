#pragma once

#include <QWidget>
#include "ui_QtRegister.h"

#include <QtNetwork>

#include "QtLogin.h"
#include "GenerateVerificationCode.h"

class QtRegister : public QWidget
{
	Q_OBJECT

public:
	QtRegister(QWidget *parent = Q_NULLPTR);
	~QtRegister();

private:
	Ui::QtRegister ui;

	QNetworkRequest netReq;
	QNetworkAccessManager* networkAccessManager_post_register;
	QNetworkAccessManager* networkAccessManager_post_code;
	QNetworkAccessManager* networkAccessManager_post_edit;

	qint8 return_post_state;

public slots:
	void btn_post_register();
	void finished_post_register(QNetworkReply*);
	int btn_post_getcode();
	void finished_post_getcode(QNetworkReply*);

	void turn_verification();
	void turn_login();

	int btn_post_edit();
	void finished_post_edit(QNetworkReply*);
};
