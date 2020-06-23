#include "QtWorkparameter.h"
#include "Project_gather.h"
#include <QDateEdit>
#include "Qttimedimage.h"
#include "QtRealtimeimage.h"
#include "QtAnt.h"
#include "QtCommandtest.h"
#include <QFile> 
#include "QtLogin.h"

#pragma execution_character_set("utf-8")
#pragma pack (1)

QtWorkparameter::QtWorkparameter(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ptaskplan_results = nullptr;
	return_plan_num = 0;

	ui.accomplish_time->setDisplayFormat("yyyy:MM:dd HH:mm:ss");
	ui.accomplish_time->setDateTime(QDateTime::currentDateTime().addDays(0));

	QDateTimeEdit* dateTimeEdit = ui.edit_cmdstarttime;
	ui.edit_cmdstarttime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
	dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(0));

	dateTimeEdit = ui.edit_cmdendtime;
	ui.edit_cmdendtime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
	dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(0));

	dateTimeEdit = ui.edit_usestarttime;
	ui.edit_usestarttime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
	dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(0));

	dateTimeEdit = ui.edit_useendtime;
	ui.edit_useendtime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
	dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(0));

	QMap<QString, int> camera;
	camera.insert("高分", 1);
	camera.insert("广角", 2);
	camera.insert("低分", 3);
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

	QMap<QString, int> ratio;
	ratio.insert("2048*2048", 1);
	ratio.insert("640*512", 2);
	ui.comboBox_ratio->clear();
	foreach(const QString & str, ratio.keys())
		ui.comboBox_ratio->addItem(str, ratio.value(str));

	QMap<QString, int> work_mode;
	work_mode.insert("记录", 1);
	work_mode.insert("记录压缩直传", 2);
	ui.comboBox_workmode->clear();
	foreach(const QString & str, work_mode.keys())
		ui.comboBox_workmode->addItem(str, work_mode.value(str));

	QMap<QString, int> compression_mode;
	compression_mode.insert("无压缩", 0);
	compression_mode.insert("有损压缩", 1);
	compression_mode.insert("无损压缩", 2);
	ui.comboBox_compressmode->clear();
	foreach(const QString & str, compression_mode.keys())
		ui.comboBox_compressmode->addItem(str, compression_mode.value(str));

	QMap<QString, int> task_work_mode;
	task_work_mode.insert("实时成像实时数传", 1);
	task_work_mode.insert("实时成像延时数传",2);
	task_work_mode.insert("延时成像延时数传", 3);
	task_work_mode.insert("实时交互操作", 4);
	ui.task_work_mode->clear();
	foreach(const QString & str, task_work_mode.keys())
		ui.task_work_mode->addItem(str, task_work_mode.value(str));

	QMap<QString, int> transfer_rate;
	transfer_rate.insert("5Mpbs", 1);
	transfer_rate.insert("40Mpbs", 2);
	ui.comboBox_transfer_rate->clear();
	foreach(const QString & str, transfer_rate.keys())
		ui.comboBox_transfer_rate->addItem(str, transfer_rate.value(str));

	QMap<QString, int> instruction_conversion;
	instruction_conversion.insert("允许", 0x55);
	instruction_conversion.insert("不许", 0);
	ui.comboBox_instruction_conversion->clear();
	foreach(const QString & str, instruction_conversion.keys())
		ui.comboBox_instruction_conversion->addItem(str, instruction_conversion.value(str));

	QMap<QString, int> allow_fixTime;
	allow_fixTime.insert("允许", 1);
	allow_fixTime.insert("不许", 2);
	ui.comboBox_charge_time->clear();
	foreach(const QString & str, allow_fixTime.keys())
		ui.comboBox_charge_time->addItem(str, allow_fixTime.value(str));

	connect(ui.btn_to_state, SIGNAL(clicked()), this, SLOT(btn_to_state()));
	connect(ui.btn_to_get_time, SIGNAL(clicked()), this, SLOT(btn_get_time()));
	connect(ui.btn_to_ant, SIGNAL(clicked()), this, SLOT(btn_to_ant()));
	
	connect(ui.btn_apply_author_code, SIGNAL(clicked()), this, SLOT(btn_post_author_code()));	
	connect(ui.btn_to_timed_image, SIGNAL(clicked()), this, SLOT(btn_to_timed_image()));
	connect(ui.btn_to_realtime_image, SIGNAL(clicked()), this, SLOT(btn_to_realtime_image()));

	manager_post_task_program = new QNetworkAccessManager(this);
	connect(manager_post_task_program, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_task_plan(QNetworkReply*)));

	manager_post_author_code = new QNetworkAccessManager(this);
	connect(manager_post_author_code, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_post_author_code(QNetworkReply*)));

	manager_get_time = new QNetworkAccessManager(this);
	connect(manager_get_time, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished_get_time(QNetworkReply*)));

	m_tcpClient = new QTcpSocket(this);
	connect(m_tcpClient, SIGNAL(connected()), this, SLOT(tcp_connected_success()));
	connect(m_tcpClient, SIGNAL(readyRead()), this, SLOT(tcp_readserver_data()));
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

void QtWorkparameter::btn_to_realtime_image()
{
	//	m_tcpClient->write((char*)data_to_send, (total_validdata_length + 9));
	QtRealtimeimage* realtimeimage = new QtRealtimeimage;
	realtimeimage->show();
	this->hide();
}

void QtWorkparameter::btn_to_timed_image()
{
	//	m_tcpClient->write((char*)data_to_send, (total_validdata_length + 9));

	Qttimedimage* timedimage = new Qttimedimage;
	timedimage->show();
	this->hide();
}

void QtWorkparameter::btn_to_ant()
{
	QtAnt* ant = new QtAnt;
	ant->show();
	this->hide();
}

void QtWorkparameter::btn_get_time()
{
	QtLogin* login;
	login = new QtLogin;
	QString Bearer_string = login->get_id_token();
	Bearer_string.prepend("Bearer ");

	QString longitude = ui.lineEdit_longitude->text();
	float float_num = longitude.toFloat();
	if (float_num < -180 | float_num > 180)
	{
		ui.plainTextEdit->appendPlainText("经度输入有误！");
		return;
	}
	QString latitude = ui.lineEdit_latitude->text();
	float_num = latitude.toFloat();
	if (float_num < -180 | float_num > 180)
	{
		ui.plainTextEdit->appendPlainText("纬度输入有误！");
		return;
	}
	QString locationLongitude = "22.11";
	QString locationLatitude = "33.22";
	QDateTimeEdit* deadline = ui.accomplish_time;
	qint64 cmdstarttime = deadline->dateTime().toTime_t();
	cmdstarttime -= get_second_2010_to_1970();

	QByteArray  m_httpData;
	QJsonDocument m_httpDocum;
	QJsonObject _exampleObject;
	QJsonObject stationLocation, observedLocation;

	stationLocation.insert("longitude", locationLongitude);
	stationLocation.insert("latitude", locationLatitude);
	_exampleObject.insert("stationLocation", stationLocation);

	observedLocation.insert("longitude", longitude);
	observedLocation.insert("latitude", latitude);
	_exampleObject.insert("observedLocation", observedLocation);

	_exampleObject.insert("jobDeadline", cmdstarttime);

	m_httpDocum.setObject(_exampleObject);
	m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);
	QNetworkRequest netReq;
	netReq.setUrl(QUrl("http://192.168.2.88:8080/api/satellite/jobs/plan"));
	netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QByteArray  Bearer0_bytes = Bearer_string.toUtf8();
	netReq.setRawHeader("Authorization", Bearer0_bytes);
	manager_get_time->post(netReq, m_httpData);
}

static qint8 stationId;
void QtWorkparameter::finished_get_time(QNetworkReply* reply)
{	
	QByteArray bytes = reply->readAll();
	QJsonParseError json_error;
	QJsonDocument jsonDoc(QJsonDocument::fromJson(bytes, &json_error));

	quint32 period_start;
	quint32 period_end;
	int i = 0;

//	ui.plainTextEdit->appendPlainText(bytes);

	if (json_error.error != QJsonParseError::NoError)
	{
		qDebug() << "json error!";
		return;
	}
	if (jsonDoc.isArray())
	{
		QJsonArray array = jsonDoc.array();  // 转数组 
		int nSize = array.size();
		return_plan_num = nSize ;

		ptaskplan_results = new plan_results_t[ sizeof( plan_results_t) * nSize];

		for (i = 0; i < nSize; ++i)
		{
			qDebug() << array.at(i).toObject().value("satelliteId").toInt();
			qDebug() << array.at(i).toObject().value("stationId").toInt();

			stationId = array.at(i).toObject().value("stationId").toInt();

			qDebug() << array.at(i).toObject().value("commSyncWord").toString();

			ptaskplan_results->satelliteId = array.at(i).toObject().value("satelliteId").toInt();
			ptaskplan_results->stationId = array.at(i).toObject().value("stationId").toInt();
			ptaskplan_results->commSyncWord = array.at(i).toObject().value("commSyncWord").toString();

			if (array.at(i).toObject().value("period").isObject())
			{
				QJsonObject jsonObject = array.at(i).toObject().value("period").toObject();
				qDebug() << "start is " << jsonObject.value("start").toInt();
				qDebug() << "end is " << jsonObject.value("end").toInt();

				period_start = jsonObject.value("start").toString().toInt();
				ptaskplan_results->period_start = period_start;
				period_end = jsonObject.value("end").toString().toInt();
				ptaskplan_results->period_end = period_end;					

				QString datafrom_starttime = time_charge_sec_to_dataform(period_start);
				QMap<QString, int> map_data_time;				
				map_data_time.insert(datafrom_starttime, period_start);
				//ui.comboBox->clear();
				foreach(const QString & str, map_data_time.keys())
					ui.comboBox->addItem(str, map_data_time.value(str));
			}
			else
			{
				ui.plainTextEdit->appendPlainText("任务规划 返回时间格式有误！");
			}
			ptaskplan_results++;
		}
		ptaskplan_results -= i;
	}
}

static uchar valid_data_to_send[100];
void QtWorkparameter::btn_post_author_code()
{	
	QtLogin* login;
	login = new QtLogin;
	QString Bearer_string = login->get_id_token();
	Bearer_string.prepend("Bearer ");

	qint32 satelliteId = 0;
	qint32 stationId = 0;
	quint8 cameraType = ui.comboBox_camera->currentData().toInt();
	quint8 visiontype = ui.comboBox_imagingtype->currentData().toInt();
	quint16 quantity = ui.lineEdit_number->text().toInt();
	quint8 imageresolution = ui.comboBox_ratio->currentData().toInt();
	quint8 imageframeFrequency = ui.comboBox_frame_rate->currentText().toInt();
	quint8 imageworkmode = ui.comboBox_workmode->currentData().toInt();
	quint8 compressionmode = ui.comboBox_compressmode->currentData().toInt();
	quint8 compressionratio = ui.comboBox_compressrate->currentText().toInt();

	//QString deviceid = login->get_dev_id();
	qint32 deviceid = 123456;
	QString observerlocation_longitude = "11.11";
	QString observerlocation_latitude= "22.22";
	QString observedlocation_longitude= ui.lineEdit_longitude->text();
	QString observedlocation_latitude = ui.lineEdit_latitude->text();
	QString transferlocation_longitude ;
	QString transferlocation_latitude;
	quint8 jobMode = ui.task_work_mode->currentData().toInt();
	quint8 transferRate = ui.comboBox_transfer_rate->currentData().toInt();

	QDateTime starttime = ui.edit_cmdstarttime->dateTime();
	QDateTime origin_utc = QDateTime::fromString("1970-01-01 08:00:00", "yyyy-MM-dd hh:mm:ss");
	QString  standard_utc_string = "2000-01-01 12:00:00";
	QDateTime standard_utc = QDateTime::fromString(standard_utc_string, "yyyy-MM-dd hh:mm:ss");

	qint64 nSeconds_standard_to_origin = origin_utc.secsTo(standard_utc);//获取距离1970-01-01 08:00:00的总秒数
	qint64 nSeconds_dateTime_to_origin = origin_utc.secsTo(starttime);//获取距离1970-01-01 08:00:00的总秒数

	quint32 jobStartTime_int = nSeconds_dateTime_to_origin - nSeconds_standard_to_origin;
	qint64 jobStartTime = 111111;

	QDateTime endtime = ui.edit_cmdendtime->dateTime();
	nSeconds_dateTime_to_origin = origin_utc.secsTo(endtime);//获取距离1970-01-01 08:00:00的总秒数
	quint32 jobDeadline_int = nSeconds_dateTime_to_origin - nSeconds_standard_to_origin;
	qint64 jobDeadline = 222222;
	quint8 instructionConversion = ui.comboBox_instruction_conversion->currentData().toInt();
	quint8 visionPriority = ui.comboBox_priority->currentText().toInt();
	quint8 allowFixTime = ui.comboBox_charge_time->currentData().toInt();
	qint64 operateStartTime = 333333;
	qint64 operateEndTime = 444444;
	/*--------------------------------------------*/
	QDateTime StartTime = ui.edit_usestarttime->dateTime();
	quint32 stime = StartTime.toTime_t();
	QDateTime Deadline = ui.edit_useendtime->dateTime();
	quint32 etime = Deadline.toTime_t();
	if (stime > etime)
	{
		ui.plainTextEdit->appendPlainText("时间设置大小有误！");
		return;
	}

	 StartTime = ui.dateEdit_start->dateTime();
	 stime = StartTime.toTime_t();
	 Deadline = ui.dateEdit_end->dateTime();
	 etime = Deadline.toTime_t();
	if (stime > etime)
	{
		ui.plainTextEdit->appendPlainText("时间设置大小有误！");
		return;
	}

	QByteArray  m_httpData;
	QJsonDocument m_httpDocum;
	QJsonObject _exampleObject;
	QJsonObject stationLocation, observedlocation, transferlocation;
	_exampleObject.insert("satelliteId", satelliteId);
	_exampleObject.insert("stationId", stationId);
	_exampleObject.insert("cameraType", cameraType);
	_exampleObject.insert("visionType", visiontype);
	_exampleObject.insert("quantity", quantity);
	_exampleObject.insert("imageResolution", imageresolution);
	_exampleObject.insert("imageFrameFrequency", imageframeFrequency);
	_exampleObject.insert("imageWorkMode", imageworkmode);
	_exampleObject.insert("compressionMode", compressionmode);
	_exampleObject.insert("compressionRatio", compressionratio);
	_exampleObject.insert("deviceId", deviceid);

	stationLocation.insert("longitude", observerlocation_longitude);
	stationLocation.insert("latitude", observerlocation_latitude);
	_exampleObject.insert("stationLocation", stationLocation);

	observedlocation.insert("longitude", observedlocation_longitude);
	observedlocation.insert("latitude", observedlocation_latitude);
	_exampleObject.insert("observedLocation", observedlocation);

	transferlocation.insert("longitude", transferlocation_longitude);
	transferlocation.insert("latitude", transferlocation_latitude);
	_exampleObject.insert("transferLocation", transferlocation);

	_exampleObject.insert("jobMode", jobMode);
	_exampleObject.insert("transferRate", transferRate);
	_exampleObject.insert("jobStartTime", jobStartTime);
	_exampleObject.insert("jobDeadline", jobDeadline);
	_exampleObject.insert("instructionConversion", instructionConversion);
	_exampleObject.insert("visionPriority", visionPriority);
	_exampleObject.insert("allowFixTime", allowFixTime);
	_exampleObject.insert("operateStartTime", operateStartTime);
	_exampleObject.insert("operateEndTime", operateEndTime);

	m_httpDocum.setObject(_exampleObject);
	m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);

	QNetworkRequest netReq;

	netReq.setUrl(QUrl("http://192.168.2.88:8080/api/satellite/jobs"));
	netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	QByteArray  Bearer0_bytes = Bearer_string.toUtf8();
	netReq.setRawHeader("Authorization", Bearer0_bytes);
	qDebug() << "btn_post_task_plan";
	manager_post_author_code->post(netReq, m_httpData);

	QDateTime origin_time = QDateTime::fromString("1970-01-01 08:00:00", "yyyy-MM-dd hh:mm:ss");
	QString begin = "2000-01-01 12:00:00";
	QDateTime begin_time = QDateTime::fromString(begin, "yyyy-MM-dd hh:mm:ss");
	qint64 nSeconds = origin_time.secsTo(begin_time);

	QtCommandtest QtCommandtest_work;
	uchar*  data_to_send = new uchar[100];

	memcpy(&data_to_send[0], &(ui.comboBox_camera->currentData()), 1);
	memcpy(&data_to_send[1], &(ui.comboBox_imagingtype->currentData()), 1);

	memcpy(&data_to_send[2],&quantity,2);
	memcpy(&data_to_send[4], &(ui.comboBox_ratio->currentData()), 1);
	quint8 frame_rate =  ui.comboBox_frame_rate->currentText().toInt();
	memset(&data_to_send[5], frame_rate, 1);
	memcpy(&data_to_send[6], &(ui.comboBox_workmode->currentData()), 1);
	memcpy(&data_to_send[7], &(ui.comboBox_compressmode->currentData()), 1);
	quint8 compress_rate = ui.comboBox_compressrate->currentText().toInt();
	memset(&data_to_send[8], compress_rate, 1);
	memset(&data_to_send[9], 0, 1);
	memset(&data_to_send[10], 0, 1);
	float uilineEdit_longitude = (ui.lineEdit_longitude->text()).toFloat();

	memcpy(&data_to_send[11], &uilineEdit_longitude, 4);
	float uilineEdit_latitude = (ui.lineEdit_latitude->text()).toFloat();

	memcpy(&data_to_send[15], &uilineEdit_latitude, 4);
	memset(&data_to_send[19], 0x11111111, 4);
	memset(&data_to_send[23], 0x22222222, 4);
	memset(&data_to_send[27], 0x33333333, 4);
	float term_longitude = (ui.Edit_term_longitude->text()).toFloat();

	memcpy(&data_to_send[31], &term_longitude, 4);
	float term_latitude = (ui.Edit_term_latitude->text()).toFloat();

	memcpy(&data_to_send[35], &term_latitude, 4);
	memcpy(&data_to_send[39], &(ui.task_work_mode->currentData()), 1);
	float pass_longitude = (ui.lineEdit_pass_longitude->text()).toFloat();

	memcpy(&data_to_send[40], &pass_longitude, 4);
	float pass_latitude = (ui.lineEdit_pass_latitude->text()).toFloat();

	memcpy(&data_to_send[44],&pass_latitude, 4);

	QDateTimeEdit* cmd_start_time = ui.edit_cmdstarttime;
	quint32 cmdstarttime = cmd_start_time->dateTime().toTime_t();
	cmdstarttime -= nSeconds;
	qDebug() << cmdstarttime;
	cmdstarttime = qToBigEndian(cmdstarttime);
	memcpy(&data_to_send[48], &cmdstarttime, 4);

	QDateTimeEdit* cmd_end_time = ui.edit_cmdendtime;
	quint32 cmdendtime = cmd_end_time->dateTime().toTime_t();
	cmdendtime -= nSeconds;
	cmdendtime = qToBigEndian(cmdendtime);
	memcpy(&data_to_send[52],&cmdendtime, 4);
	memcpy(&data_to_send[56], &(ui.comboBox_transfer_rate->currentData()), 1);
	memcpy(&data_to_send[57], &(ui.comboBox_instruction_conversion->currentData()), 1);
	quint8 priority = ui.comboBox_frame_rate->currentText().toInt();
	memset(&data_to_send[58], priority, 1);

	QDateTimeEdit* use_start_time = ui.edit_usestarttime;
	quint32 usestarttime = use_start_time->dateTime().toTime_t();
	usestarttime -= nSeconds;
	usestarttime = qToBigEndian(usestarttime);
	memcpy(&data_to_send[59], &usestarttime, 4);

	QDateTimeEdit* use_end_time = ui.edit_useendtime;
	quint32 useendtime = use_end_time->dateTime().toTime_t();
	useendtime -= nSeconds;
	useendtime = qToBigEndian(useendtime);
	memcpy(&data_to_send[63], &useendtime, 4);
	memset(&data_to_send[67], 2, 1);
	memset(&data_to_send[68], 0, 4);
	memset(&data_to_send[72], 0, 4);
	memset(&data_to_send[74],0, 4);
	memset(&data_to_send[78],0, 2);
	memset(&data_to_send[80],0, 1);
	memset(&data_to_send[81],0, 1);
	memset(&data_to_send[82],0, 1);
	memset(&data_to_send[83],0, 1);
	memset(&data_to_send[84],0, 1);
	memset(&data_to_send[85],0, 4);
	memset(&data_to_send[89],0, 4);
	memset(&data_to_send[93],0, 4);
	memset(&data_to_send[93], 0, 4);
	quint8 elevation = ui.comboBox_elevation->currentText().toInt();
	memcpy(&data_to_send[97], &elevation, 1);
	memset(&data_to_send[98], 0, 2);

	memcpy(valid_data_to_send, data_to_send, 100);
	qDebug() << elevation;

	qDebug() << "comboBox_elevation";
}

void QtWorkparameter::finished_post_author_code(QNetworkReply* reply)
{
	QByteArray bytes = reply->readAll();
	QString string;
	string.prepend(bytes);
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

	//ui.plainTextEdit->appendPlainText(bytes);

	if (reply->error() == QNetworkReply::NoError)
	{
		ui.plainTextEdit->appendPlainText(QString(reply->readAll()));
	}
	else
	{
		qDebug() << "statusCode is err!";
	}
	reply->deleteLater();

	QJsonParseError jsonParserError;
	QJsonDocument   jsonDocument = QJsonDocument::fromJson(bytes, &jsonParserError);

	if (!jsonDocument.isNull() && jsonParserError.error == QJsonParseError::NoError)
	{
		qDebug() << "文件解析成功\n";
		if (jsonDocument.isObject())
		{
			QJsonObject jsonObject = jsonDocument.object();

			qDebug() << "task id is " << jsonObject.value("id").toInt()
				<< "\n";
			qDebug() << "authCode is " << jsonObject.value("authCode").toString()
				<< "\n";
			qDebug() << "jobPriority is " << jsonObject.value("jobPriority").toInt()
				<< "\n";
		}
	}	
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
	return 1;
}

uchar*  QtWorkparameter::valid_data_to_send_data()
{
	return valid_data_to_send;
}

QString QtWorkparameter::time_charge_sec_to_dataform(quint64 sec)
{
	QDateTime origin_time = QDateTime::fromString("1970-01-01 08:00:00", "yyyy-MM-dd hh:mm:ss");
	QDateTime current_time = QDateTime::currentDateTime();//显示时间，格式为：年-月-日 时：分：秒 周几

	QString begin = "2000-01-01 12:00:00";

	QDateTime begin_time = QDateTime::fromString(begin, "yyyy-MM-dd hh:mm:ss");

	qint64 nSeconds = origin_time.secsTo(begin_time);//获取距离1970-01-01 08:00:00的总秒数

	//QString str1 = ui->line1->text();//读取n数量n
	qint64 num = sec;
	qint64 ms = nSeconds + num;
	QDateTime dateTime = QDateTime::fromTime_t(ms);
	QString str = dateTime.toString("yyyy-MM-dd hh:mm:ss");

//	qDebug() << dateTime << endl;
//	qDebug() << str;
//	str = str.mid(5);
	return str;
}

qint64 QtWorkparameter::get_second_2010_to_1970()
{
	QDateTime origin_time = QDateTime::fromString("1970-01-01 08:00:00", "yyyy-MM-dd hh:mm:ss");
	QString begin = "2000-01-01 12:00:00";
	QDateTime begin_time = QDateTime::fromString(begin, "yyyy-MM-dd hh:mm:ss");
	qint64 nSeconds = origin_time.secsTo(begin_time);
	return nSeconds;
}

void QtWorkparameter::btn_tcp_to_ground_as_station_ctl()
{
	tcp_to_ground_as_station = new QTcpSocket(this);
	tcp_to_ground_as_station->connectToHost("192.168.1.254", 8888);

	connect(tcp_to_ground_as_station, SIGNAL(connected()), this, SLOT(station_connected_ground_success()));
	connect(tcp_to_ground_as_station, SIGNAL(readyRead()), this, SLOT(station_rev_ground_read_data()));
}

void QtWorkparameter::station_connected_ground_success()
{
	ui.plainTextEdit->appendPlainText("连接服务器成功 as station!");

	connect(ui.btn_to_Identification_2, SIGNAL(clicked()), this, SLOT(tcp_send_station_identity_data()));
}

void QtWorkparameter::tcp_send_station_identity_data()
{
	quint8* data_to_send;
	data_to_send = new quint8(16);

	QString msg_type;
	msg_type = "hshs";
	memcpy(data_to_send, "hshs", 4); 
	memcpy(data_to_send + 4, "2020", 4);  //设备类型
	memcpy(data_to_send + 8, "0023", 4); //用户ID
	memcpy(data_to_send + 12, "a0a0", 4); //设备ID
	tcp_to_ground_ctl->write((char*)data_to_send, 16);
}

static qint8 as_station_transform_data = 0;
void QtWorkparameter::station_rev_ground_read_data()
{
	QByteArray temp = tcp_to_ground_as_station->readAll();

	QString rev_data = temp;

	static int hsok_num = 0;

	if (rev_data.compare("auth_ok"))
	{
		ui.plainTextEdit->appendPlainText("station 身份合法！");
		as_station_transform_data = 1;
	}
	else if (rev_data.compare("auth_fail"))
	{
		ui.plainTextEdit->appendPlainText("station 数据错误！");
	}
	else
	{
		ui.plainTextEdit->appendPlainText("返回错误数据");
	}
}

void QtWorkparameter::by_udp_connect_to_sky()
{
	sender = new QUdpSocket(this);

	sender->bind(10087, QUdpSocket::ShareAddress);

	connect(sender, SIGNAL(readyRead()), this, SLOT(udp_rev_data_from_sky()));

	qDebug() << "have initial!";
}

void QtWorkparameter::udp_rev_data_from_sky() //处理等待的数据报
{
	while (sender->hasPendingDatagrams()) //拥有等待的数据报
	{
		QByteArray datagram; //拥于存放接收的数据报
		datagram.resize(sender->pendingDatagramSize());
		sender->readDatagram(datagram.data(), datagram.size());
		unsigned short size = datagram.size();
		unsigned char msg[10] = { 0 };
		memcpy(msg, datagram.data(), size);
		unsigned short arr = msg[0];
		arr = (arr << 8) + msg[1];
		qDebug() << msg[0] << msg[1] << msg[2] << msg[3];
	}
}

void QtWorkparameter::udp_send_device_reset_data()
{
	unsigned char msg[10] = { 0,1,2,3 };
	QString  send_data = ui.lineEdit_2->text();

	send_data = "7EA58C9101011111999988";
	char* ch;
	QByteArray ba = send_data.toLatin1();
	ch = ba.data();

	quint8* data_to_send;
	data_to_send = new uchar[11];
	
	quint32 smg_head = 0x7EA58C91;
	quint16 smg_type = 0x0101;
	quint16 smg_len  = 0x2;
	//quint16 smg_content = 0x9999;
	quint8  smg_aisle = 0x01;
	quint8  smg_reset = 0xFF;
	quint8  smg_crc = 0x88;

	memcpy(data_to_send, &smg_head,4);
	memcpy(data_to_send+4, &smg_type, 2);
	memcpy(data_to_send + 6, &smg_len, 2);
	memcpy(data_to_send + 8, &smg_aisle, 1);
	memcpy(data_to_send + 8, &smg_aisle, 1);
	memcpy(data_to_send + 10, &smg_crc, 1);

	sender->writeDatagram((char*)data_to_send, 11, QHostAddress("192.168.0.70"), 10086);
}

void QtWorkparameter::udp_send_signal_pata_data()
{
	quint8* data_to_send;
	data_to_send = new uchar[21];
	memset(data_to_send,0,21);
	quint32 smg_head = 0x7EA58C91;
	quint16 smg_type = 0x0101;
	quint16 smg_len = 12;
	quint8  smg_aisle = 0x01;
	quint8  smg_launch_switch = 0x01;
	quint8  smg_modulation_switch = 0x01;
	quint8  smg_crc = 0x88;

	memcpy(data_to_send, &smg_head, 4);
	memcpy(data_to_send + 4, &smg_type, 2);
	memcpy(data_to_send + 6, &smg_len, 2);
	memcpy(data_to_send + 8, &smg_aisle, 1);
	memcpy(data_to_send + 9, &smg_launch_switch, 1);
	memcpy(data_to_send + 10, &smg_modulation_switch, 1);
	memcpy(data_to_send + 21, &smg_crc, 1);

	sender->writeDatagram((char*)data_to_send, 21, QHostAddress("192.168.0.70"), 10086);
}

void QtWorkparameter::udp_send_data()
{
	quint8* data_to_send;
	data_to_send = new uchar[21];
	memset(data_to_send, 0, 21);
	quint32 smg_head = 0x7EA58C91;
	quint16 smg_type = 0x0101;
	quint16 smg_len = 12;
	quint8  smg_aisle = 0x01;
	quint8  smg_launch_switch = 0x01;
	quint8  smg_modulation_switch = 0x01;

	quint8  smg_crc = 0x88;

	memcpy(data_to_send, &smg_head, 4);
	memcpy(data_to_send + 4, &smg_type, 2);
	memcpy(data_to_send + 6, &smg_len, 2);
	memcpy(data_to_send + 8, &smg_aisle, 1);
	memcpy(data_to_send + 9, &smg_launch_switch, 1);
	memcpy(data_to_send + 10, &smg_modulation_switch, 1);
	memcpy(data_to_send + 21, &smg_crc, 1);

	sender->writeDatagram((char*)data_to_send, 21, QHostAddress("192.168.0.70"), 10086);
}

void QtWorkparameter::test()
{
	QFile loadFile("D:\\11.json");

	if (!loadFile.open(QIODevice::ReadOnly))
	{
		qDebug() << "could't open projects json";
		return;
	}

	QByteArray allData = loadFile.readAll();
	loadFile.close();


	QByteArray bytes;
	bytes = allData;

	QJsonParseError json_error;
	QJsonDocument jsonDoc(QJsonDocument::fromJson(bytes, &json_error));

	quint32 period_start;
	quint32 period_end;
	if (json_error.error != QJsonParseError::NoError)
	{
		qDebug() << "json error!";
		return;
	}
	if (jsonDoc.isArray())
	{
		QJsonArray array = jsonDoc.array();  // 转数组 
		int nSize = array.size();
		return_plan_num = nSize;

		ptaskplan_results = new plan_results_t[sizeof(plan_results_t) * nSize];
		qDebug() << "plan_results_t";
		qDebug() << sizeof(plan_results_t) * nSize;
		for (int i = 0; i < nSize; ++i)
		{
			qDebug() << array.at(i).toObject().value("satelliteId").toString();
			qDebug() << array.at(i).toObject().value("stationId").toString();

			stationId = array.at(i).toObject().value("stationId").toString();

			qDebug() << array.at(i).toObject().value("commSyncWord").toString();

			ptaskplan_results[i].satelliteId = array.at(i).toObject().value("satelliteId").toString();
			ptaskplan_results[i].stationId = array.at(i).toObject().value("stationId").toString();
			ptaskplan_results[i].commSyncWord = array.at(i).toObject().value("commSyncWord").toString();

			if (array.at(i).toObject().value("period").isObject())
			{
				QJsonObject jsonObject = array.at(i).toObject().value("period").toObject();

				period_start = jsonObject.value("start").toString().toInt();
				ptaskplan_results[i].period_start = QString::number(period_start, 10);
				period_end = jsonObject.value("end").toString().toInt();
				ptaskplan_results[i].period_end = QString::number(period_end, 10);

				QString start_num = QString::number(i, 10);
				QMap<QString, int> map_data_time;
				map_data_time.insert("时间段" + start_num, i);
				//ui.comboBox->clear();
				foreach(const QString & str, map_data_time.keys())
					ui.comboBox->addItem(str, map_data_time.value(str));

			
			}
			else
			{
				ui.plainTextEdit->appendPlainText("任务规划 返回时间格式有误！");
			}
			//ptaskplan_results++;
		}		
	}

	connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBox_currentIndexChanged(int)));
}

void QtWorkparameter::comboBox_currentIndexChanged(int index)
{
	QString zone = ui.comboBox->currentData().toString();//项关联的数据
	ui.plainTextEdit->appendPlainText(":区号=" + zone);

	qDebug() << ptaskplan_results[index].satelliteId;
	ui.lineEdit->setText(ptaskplan_results[index].satelliteId);
	qDebug() << ptaskplan_results[index].stationId;
	qDebug() << ptaskplan_results[index].commSyncWord;
	quint32 period_start;
	quint32 period_end;
	period_start = ptaskplan_results[index].period_start.toInt();
	QString datafrom_period_start = time_charge_sec_to_dataform(period_start);
	period_end = ptaskplan_results[index].period_end.toInt();
	QString datafrom_period_end = time_charge_sec_to_dataform(period_end);

	QDateTimeEdit* dateTimeEdit = ui.dateEdit_start;
	ui.dateEdit_start->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
	QDateTime  time = QDateTime::fromString(datafrom_period_start, "yyyy-MM-dd hh:mm:ss");
	dateTimeEdit->setDateTime(time);

	dateTimeEdit = ui.dateEdit_end;
	ui.dateEdit_end->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
	time = QDateTime::fromString(datafrom_period_end, "yyyy-MM-dd hh:mm:ss");
	dateTimeEdit->setDateTime(time);
}

void QtWorkparameter::get_gps_data()
{
	QFile loadFile("D:\\gpsdata.txt");

	if (!loadFile.open(QIODevice::ReadOnly))
	{
		qDebug() << "could't open projects json";
		return;
	}

	QByteArray allData = loadFile.readAll();
	loadFile.close();

	quint32 aa = allData.lastIndexOf("$GPGGA",-1);
	quint32 bb = allData.lastIndexOf("$GPRMC", -1);

	QByteArray str2  = allData.mid(aa, bb);
	QString gps_data = str2;
	QString  latitude = gps_data.section(",", 2, 2);
	QString  latitude_NS = gps_data.section(",",3, 3);
	QString longitude = gps_data.section(",", 4, 4);
	QString longitude_EW = gps_data.section(",", 5, 5);
	QString elevation = gps_data.section(",", 9, 9);
	float longtude_f;
	float latitude_f;
	float elevation_f;

	if (latitude_NS.compare("N") == 0)
	{
		latitude_f = latitude.toFloat() / 100;
	}
	else
	{
		latitude_f = -latitude.toFloat() / 100;
	}

	if (longitude_EW.compare("E") == 0)
	{
		longtude_f = longitude.toFloat() / 100;
	}
	else
	{
		longtude_f = -longitude.toFloat() / 100;
	}
	elevation_f = elevation.toFloat();

	QString data = QString("%1").arg(latitude_f);
	ui.Edit_term_longitude->setText(data);
	longtude_f = -longtude_f;
	 data = QString("%1").arg(longtude_f);
	ui.Edit_term_latitude->setText(data);
	
	qDebug() << latitude_f;
	qDebug() << longtude_f;
	qDebug() << elevation_f;
}