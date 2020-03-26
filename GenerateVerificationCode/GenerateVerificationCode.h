#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GenerateVerificationCode.h"

#include <QRandomGenerator>

#include <QtNetwork>



class GenerateVerificationCode : public QMainWindow
{
	Q_OBJECT

public:
	GenerateVerificationCode(QWidget *parent = Q_NULLPTR);

	void Get(QUrl u);

public slots:
	void btn_GenerateCode();
//	void btn_putstorage();
	void btn_get_clicked();
	void finished_get();

	void finished_storage_post(QNetworkReply*);
	void btn_post_storage_clicked();

	void btn_post_toppouring_clicked();
	void finished_toppouring_post(QNetworkReply* reply);

	void btn_post_del_clicked();
	void finished_del_post(QNetworkReply* reply);

	void turn_register();
	void turn_login();


private:
	Ui::GenerateVerificationCodeClass ui;

	QString str1;  
	QString str3;
	QString str2;
	QString str;
	QString crccheck;

	QList<QString> aList;

	QNetworkRequest netReq;
	QNetworkAccessManager manager;
	QUrl url;
	QNetworkReply* reply;
	QString html_text;
	QNetworkAccessManager* networkAccessManager_storage;
	QNetworkAccessManager* networkAccessManager_toppouring;
	QNetworkAccessManager* networkAccessManager_del;
	//QNetworkAccessManager* networkAccessManager_;
};
