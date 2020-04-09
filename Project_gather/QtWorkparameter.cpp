#include "QtWorkparameter.h"

#include "Project_gather.h"

#include <QDateEdit>

#include "Qttimedimage.h"
#include "QtRealtimeimage.h"

#pragma execution_character_set("utf-8")


QtWorkparameter::QtWorkparameter(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QMap<QString, int> camera;
	camera.insert("高分", 1);
	camera.insert("低分", 2);
	camera.insert("广角", 3);
	camera.insert("红外", 4);

	ui.comboBox_camera->clear();
	foreach(const QString & str, camera.keys())
		ui.comboBox_camera->addItem(str, camera.value(str));

	QMap<QString, int> imagingtype;
	imagingtype.insert("单张", 1);
	imagingtype.insert("多张", 2);
	imagingtype.insert("视频", 3);
		
	ui.comboBox_imagingtype->clear();
	foreach(const QString & str, imagingtype.keys())
		ui.comboBox_imagingtype->addItem(str, imagingtype.value(str));

	connect(ui.btn_to_state, SIGNAL(clicked()), this, SLOT(btn_to_state()));
	connect(ui.btn_to_task_plan, SIGNAL(clicked()), this, SLOT(btn_post_task_plan()));
	connect(ui.btn_apply_author_code, SIGNAL(clicked()), this, SLOT(btn_post_author_code()));	
	connect(ui.btn_to_timed_image, SIGNAL(clicked()), this, SLOT(btn_to_timed_image()));
	connect(ui.btn_to_realtime_image, SIGNAL(clicked()), this, SLOT(btn_to_realtime_image()));

	manager_post_task_program = new QNetworkAccessManager(this);
	connect(manager_post_task_program, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_task_plan(QNetworkReply*)));

	manager_post_author_code = new QNetworkAccessManager(this);
	connect(manager_post_author_code, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_author_code(QNetworkReply*)));

	m_tcpClient = new QTcpSocket(this);
	connect(m_tcpClient, SIGNAL(connected()), this, SLOT(tcp_connected_success()));
	connect(m_tcpClient, SIGNAL(readyRead()), this, SLOT(tcp_readserver_data()));

	m_tcpClient->connectToHost("192.168.0.102", 10086);
}

QtWorkparameter::~QtWorkparameter()
{

}

void QtWorkparameter::btn_to_state()
{
	Project_gather* Project_login = new Project_gather;
	Project_login->show();
	this->hide();
}

void QtWorkparameter::btn_to_quit()
{
	this->close();
}

void QtWorkparameter::btn_post_task_plan()
{
	QString longitude = ui.lineEdit_longitude->text();
	QString latitude = ui.lineEdit_latitude->text();

	//int EWcount = ui.comboBox_orientationEW->count();

	QString orientationEW = ui.comboBox_orientationEW->currentText();
	QString orientationSN = ui.comboBox_orientationSN->currentText();

	// QString camera  = ui.comboBox_camera->currentText();
	   
	QString camerastring = ui.comboBox_camera->currentData().toString();//项关联的数据

	//ui.plainTextEdit->appendPlainText(cpyName);

	QDate dt = ui.dateEdit->date();

	//	text();
	QString dtstring = dt.toString("yyyy-MM-dd");

	QString totaltring = longitude;

	totaltring.append(orientationEW);
	totaltring.append(latitude);
	totaltring.append(orientationSN);
	totaltring.append(camerastring);
	totaltring.append(dtstring);

	ui.plainTextEdit->appendPlainText(totaltring);
/*
	QByteArray  m_httpData;
	QJsonDocument m_httpDocum;
	QJsonObject _exampleObject;
	_exampleObject.insert("login", login);
	_exampleObject.insert("password", password);
	_exampleObject.insert("phone", phone);
	_exampleObject.insert("smsCode", smsCode);
	_exampleObject.insert("firstName", firstName);
	_exampleObject.insert("lastName", lastName);
	_exampleObject.insert("birthday", birthday);
	_exampleObject.insert("email", email);
	_exampleObject.insert("companyName", companyName);
	_exampleObject.insert("companyPosition", companyPosition);
	_exampleObject.insert("companyAddress", companyAddress);

	//post请求的数据是Json格式。首先创建QJsonObject对象，并插入数据。
	m_httpDocum.setObject(_exampleObject);
	m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);
	// 转成Json格式
	QNetworkRequest netReq;
	netReq.setUrl(QUrl("http://192.168.1.254:8080/api/register"));
	netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	// 将用户名和密码发送至web服务器进行验证
	qDebug() << "btn_post_task_plan";
	manager_post_task_program->post(netReq, m_httpData);
*/
}

void QtWorkparameter::finished_post_task_plan(QNetworkReply* reply)
{
/*	QByteArray bytes = reply->readAll();
	QString string;
	string.prepend(bytes);
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

	if (reply->error() == QNetworkReply::NoError)
	{
		ui.plainTextEdit->appendPlainText(QString(reply->readAll()));
	}
	else
	{
		qDebug() << "statusCode is err!";
	}
	reply->deleteLater();

	QJsonDocument doc = QJsonDocument::fromJson(bytes);
	//判断是否对象
	if (doc.isObject())
	{
		QJsonObject obj = doc.object();
		id_token1 = obj["id_token"].toString();
	}
	else
	{
		qDebug() << "doc is not Object";
	}
	*/
}


void QtWorkparameter::btn_post_author_code()
{

	//QString imagingtype = ui.comboBox_imagingtype->currentText();
	QString imagingnumber = ui.comboBox_imagingnumber->currentText();
	QString imagingtime = ui.comboBox_imagingtime->currentText();

	int videotime = ui.Slider_videotime->value();
	QString  videotimestring = QString::number(videotime);


	QString imagingtypestring = ui.comboBox_imagingtype->currentData().toString();//项关联的数据

	QString totaltring = imagingtypestring;
	totaltring.append(imagingnumber);
	totaltring.append(imagingtime);
	totaltring.append(videotimestring);

	ui.plainTextEdit->appendPlainText(totaltring);
/*
	QByteArray  m_httpData;
	QJsonDocument m_httpDocum;
	QJsonObject _exampleObject;
	_exampleObject.insert("login", login);
	_exampleObject.insert("password", password);
	_exampleObject.insert("phone", phone);
	_exampleObject.insert("smsCode", smsCode);
	_exampleObject.insert("firstName", firstName);
	_exampleObject.insert("lastName", lastName);
	_exampleObject.insert("birthday", birthday);
	_exampleObject.insert("email", email);
	_exampleObject.insert("companyName", companyName);
	_exampleObject.insert("companyPosition", companyPosition);
	_exampleObject.insert("companyAddress", companyAddress);

	//post请求的数据是Json格式。首先创建QJsonObject对象，并插入数据。
	m_httpDocum.setObject(_exampleObject);
	m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);
	// 转成Json格式
	QNetworkRequest netReq;
	netReq.setUrl(QUrl("http://192.168.1.254:8080/api/register"));
	netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	// 将用户名和密码发送至web服务器进行验证
	qDebug() << "btn_post_task_plan";
	manager_post_task_program->post(netReq, m_httpData);
*/
}

void QtWorkparameter::finished_post_author_code(QNetworkReply* reply)
{
/*	QByteArray bytes = reply->readAll();
	QString string;
	string.prepend(bytes);
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

	if (reply->error() == QNetworkReply::NoError)
	{
		ui.plainTextEdit->appendPlainText(QString(reply->readAll()));
	}
	else
	{
		qDebug() << "statusCode is err!";
	}
	reply->deleteLater();

	QJsonDocument doc = QJsonDocument::fromJson(bytes);
	//判断是否对象
	if (doc.isObject())
	{
		QJsonObject obj = doc.object();
		id_token1 = obj["id_token"].toString();
	}
	else
	{
		qDebug() << "doc is not Object";
	}
	*/
}

void QtWorkparameter::btn_to_realtime_image()
{
	m_tcpClient->write((char*)data_to_send, (total_validdata_length + 9));

	QtRealtimeimage* realtimeimage = new QtRealtimeimage;
	realtimeimage->show();
	this->hide();
}

void QtWorkparameter::btn_to_timed_image()
{
	m_tcpClient->write((char*)data_to_send, (total_validdata_length + 9));

	Qttimedimage* timedimage = new Qttimedimage;
	timedimage->show();
	this->hide();
}

void  QtWorkparameter::tcp_connected_success()
{
	ui.plainTextEdit->appendPlainText("连接服务器成功");
}

quint8  QtWorkparameter::tcp_readserver_data()
{
	quint8 valid_data_len;
	QByteArray temp = m_tcpClient->readAll();
	QDataStream out(&temp, QIODevice::ReadWrite);    //将字节数组读入
//	rev_valid_data_head_t rev_valid_data_head;
}