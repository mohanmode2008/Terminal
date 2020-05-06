#include "QtWorkparameter.h"

#include "Project_gather.h"

#include <QDateEdit>

#include "Qttimedimage.h"
#include "QtRealtimeimage.h"

#include "QtCommandtest.h"

#include <QFile> 

#include "QtLogin.h"

#pragma execution_character_set("utf-8")
#pragma pack (1)



QtWorkparameter::QtWorkparameter(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

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

	connect(ui.btn_to_ground_ctl, SIGNAL(clicked()), this, SLOT(tcp_to_yunkong_as_client()));

//	m_tcpClient->connectToHost("192.168.0.103", 10086);
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

void QtWorkparameter::btn_get_time()
{
	QtLogin* login_success;
	login_success = new QtLogin;
	QString Bearer_string = login_success->get_id_token();

	QString longitude = ui.lineEdit_longitude->text();
	QString latitude = ui.lineEdit_latitude->text();

	//QString token = Bearer_string;
	QString token ="123456abc";
	QString observedLongitude = "22.11";
	QString observedLatitude= "33.22";
	QDateTimeEdit* deadline = ui.accomplish_time;
	quint32 cmdstarttime = deadline->dateTime().toTime_t();
	cmdstarttime -= get_second_2010_to_1970();
//	qDebug() << cmdstarttime;
//	cmdstarttime = QtCommandtest_work.byteOrderChange32Bit(&cmdstarttime);
//	memcpy(&data_to_send[48], &cmdstarttime, 4);

	QByteArray  m_httpData;
	QJsonDocument m_httpDocum;
	QJsonObject _exampleObject;
	QJsonObject stationLocation, observedLocation;

	_exampleObject.insert("jobDeadline",QString::number(cmdstarttime, 16));

	stationLocation.insert("longitude", longitude);
	stationLocation.insert("latitude", latitude);
	_exampleObject.insert("observerLocation", stationLocation);

	observedLocation.insert("longitude", observedLongitude);
	observedLocation.insert("latitude", observedLatitude);
	_exampleObject.insert("observerLocation", observedLocation);

	m_httpDocum.setObject(_exampleObject);
	m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);

	QNetworkRequest netReq;
	if (!token.isEmpty()) {
		netReq.setRawHeader("authorization", token.toStdString().c_str());
	}
	netReq.setUrl(QUrl("http://192.168.1.254:8080/api/register"));
	netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	manager_post_task_program->post(netReq, m_httpData);

}

QString stationId;
void QtWorkparameter::finished_get_time(QNetworkReply* reply)
{
	QByteArray bytes = reply->readAll();
	QJsonParseError json_error;
	QJsonDocument jsonDoc(QJsonDocument::fromJson(bytes, &json_error));
	if (json_error.error != QJsonParseError::NoError)
	{
		qDebug() << "json error!";
		exit(1);
	}
	if (jsonDoc.isArray())
	{
		QJsonArray array = jsonDoc.array();  // 转数组 
		int nSize = array.size();

		pplan_results = new plan_results[ sizeof(struct plan_results) * nSize];

		for (int i = 0; i < nSize; ++i)
		{
			qDebug() << array.at(i).toObject().value("satelliteId").toString();
			qDebug() << array.at(i).toObject().value("stationId").toString();

			stationId = array.at(i).toObject().value("stationId").toString();

			qDebug() << array.at(i).toObject().value("commSyncWord").toString();

			pplan_results->satelliteId = array.at(i).toObject().value("satelliteId").toString();
			pplan_results->stationId = array.at(i).toObject().value("stationId").toString();
			pplan_results->commSyncWord = array.at(i).toObject().value("commSyncWord").toString();

			if (array.at(i).toObject().value("period").isObject())
			{
				QJsonObject jsonObject = array.at(i).toObject().value("period").toObject();
				qDebug() << "start is " << jsonObject.value("start").toString().toInt();
				qDebug() << "end is " << jsonObject.value("end").toString().toInt();

				pplan_results->period_start = jsonObject.value("start").toString().toInt();
				pplan_results->period_end = jsonObject.value("end").toString().toInt();
			}
			pplan_results++;
		}
	}
}

static uchar valid_data_to_send[100];
void QtWorkparameter::btn_post_author_code()
{	
	//QString imagingtype = ui.comboBox_imagingtype->currentText();
//	QString imagingnumber = ui.comboBox_imagingnumber->currentText();
//	QString imagingtime = ui.comboBox_imagingtime->currentText();

//	int videotime = ui.Slider_videotime->value();
//	QString  videotimestring = QString::number(videotime);


//	QString imagingtypestring = ui.comboBox_imagingtype->currentData().toString();//项关联的数据

//	ui.plainTextEdit->appendPlainText(totaltring);
	/*--------------------------------------------*/
	QtLogin* login_success;
	login_success = new QtLogin;
	QString Bearer_string = login_success->get_id_token();
	//QString token = Bearer_string;
	QString token = "123456";
	QString satelliteId = "0";
	QString stationId = "0";
	QString cameratype = ui.comboBox_camera->currentData().toString();
	QString visiontype = ui.comboBox_imagingtype->currentData().toString();
	QString quantity = ui.lineEdit_number->text();
	QString imageresolution = ui.comboBox_ratio->currentData().toString();
	QString imageframeFrequency = ui.comboBox_frame_rate->currentText();
	QString imageworkmode = ui.comboBox_workmode->currentData().toString();
	QString compressionmode = ui.comboBox_compressmode->currentData().toString();
	QString compressionratio = ui.comboBox_compressrate->currentText();
	QString deviceid = "123456";
	QString observerlocation_longitude = "11.11";
	QString observerlocation_latitude= "22.22";
	QString observedlocation_longitude= ui.lineEdit_longitude->text();
	QString observedlocation_latitude = ui.lineEdit_latitude->text();
	QString transferlocation_longitude ;
	QString transferlocation_latitude;
	QString jobMode = ui.task_work_mode->currentData().toString();
	QString transferRate = ui.comboBox_transfer_rate->currentData().toString();
	QString jobStartTime = "111111";
	QString jobDeadline = "222222";
	QString instructionConversion = ui.comboBox_instruction_conversion->currentData().toString();
	QString visionPriority = ui.comboBox_priority->currentText();
	QString allowFixTime = ui.comboBox_charge_time->currentData().toString();
	QString operateStartTime = "333333";
	QString operateEndTime = "444444";
	/*--------------------------------------------*/

	QByteArray  m_httpData;
	QJsonDocument m_httpDocum;
	QJsonObject _exampleObject;
	QJsonObject stationLocation, observedlocation, transferlocation;
	_exampleObject.insert("satelliteId", satelliteId);
	_exampleObject.insert("stationId", stationId);
	_exampleObject.insert("cameraType", cameratype);
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
	_exampleObject.insert("observerLocation", stationLocation);

	observedlocation.insert("longitude", observedlocation_longitude);
	observedlocation.insert("latitude", observedlocation_latitude);
	_exampleObject.insert("observerLocation", observedlocation);

	transferlocation.insert("longitude", transferlocation_longitude);
	transferlocation.insert("latitude", transferlocation_latitude);
	_exampleObject.insert("observerLocation", transferlocation);

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

	if (!token.isEmpty()) {
		netReq.setRawHeader("authorization", token.toStdString().c_str());
	}
	netReq.setUrl(QUrl("http://192.168.1.254:8080/api/register"));
	netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	qDebug() << "btn_post_task_plan";
	manager_post_task_program->post(netReq, m_httpData);



	QDateTime origin_time = QDateTime::fromString("1970-01-01 08:00:00", "yyyy-MM-dd hh:mm:ss");
	QString begin = "2000-01-01 12:00:00";
	QDateTime begin_time = QDateTime::fromString(begin, "yyyy-MM-dd hh:mm:ss");
	qint64 nSeconds = origin_time.secsTo(begin_time);

	QtCommandtest QtCommandtest_work;
	uchar*  data_to_send = new uchar[100];
	//qDebug() << ui.comboBox_camera->currentData();
	//qDebug() << ui.comboBox_camera->currentData().toString();
	memcpy(&data_to_send[0], &(ui.comboBox_camera->currentData()), 1);
	memcpy(&data_to_send[1], &(ui.comboBox_imagingtype->currentData()), 1);
	quint16 iquantity = quantity.toInt();
	memcpy(&data_to_send[2],&iquantity,2); 
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
	//uilineEdit_longitude = QtCommandtest_work.byteOrderChange32Bit(uilineEdit_longitude);
	memcpy(&data_to_send[11], &uilineEdit_longitude, 4);
	float uilineEdit_latitude = (ui.lineEdit_latitude->text()).toFloat();
	//uilineEdit_latitude = QtCommandtest_work.byteOrderChange32Bit(uilineEdit_latitude);
	memcpy(&data_to_send[15], &uilineEdit_latitude, 4);
	memset(&data_to_send[19], 0x11111111, 4);
	memset(&data_to_send[23], 0x22222222, 4);
	memset(&data_to_send[27], 0x33333333, 4);
	float term_longitude = (ui.Edit_term_longitude->text()).toFloat();
	//term_longitude = QtCommandtest_work.byteOrderChange32Bit(term_longitude);
	memcpy(&data_to_send[31], &term_longitude, 4);
	float term_latitude = (ui.Edit_term_latitude->text()).toFloat();
	//term_latitude = QtCommandtest_work.byteOrderChange32Bit(term_latitude);
	memcpy(&data_to_send[35], &term_latitude, 4);
	memcpy(&data_to_send[39], &(ui.task_work_mode->currentData()), 1);
	float pass_longitude = (ui.lineEdit_pass_longitude->text()).toFloat();
	//pass_longitude = QtCommandtest_work.byteOrderChange32Bit(pass_longitude);
	memcpy(&data_to_send[40], &pass_longitude, 4);
	float pass_latitude = (ui.lineEdit_pass_latitude->text()).toFloat();
	//pass_latitude = QtCommandtest_work.byteOrderChange32Bit(pass_latitude);
	memcpy(&data_to_send[44],&pass_latitude, 4);
//	qint32  cmdstarttime = (ui.edit_cmdstarttime->text()).toInt();
	QDateTimeEdit* cmd_start_time = ui.edit_cmdstarttime;
	quint32 cmdstarttime = cmd_start_time->dateTime().toTime_t();
	cmdstarttime -= nSeconds;
	qDebug() << cmdstarttime;
	cmdstarttime = QtCommandtest_work.byteOrderChange32Bit(&cmdstarttime);
	memcpy(&data_to_send[48], &cmdstarttime, 4);
//	qint32  cmdendtime = (ui.edit_cmdendtime->text()).toInt();
	QDateTimeEdit* cmd_end_time = ui.edit_cmdendtime;
	quint32 cmdendtime = cmd_end_time->dateTime().toTime_t();
	cmdendtime -= nSeconds;
	cmdendtime = QtCommandtest_work.byteOrderChange32Bit(&cmdendtime);
	memcpy(&data_to_send[52],&cmdendtime, 4);
	memcpy(&data_to_send[56], &(ui.comboBox_transfer_rate->currentData()), 1);
	memcpy(&data_to_send[57], &(ui.comboBox_instruction_conversion->currentData()), 1);
	quint8 priority = ui.comboBox_frame_rate->currentText().toInt();
	memset(&data_to_send[58], priority, 1);
//	qint32  usestarttime = (ui.edit_usestarttime->text()).toInt();
	QDateTimeEdit* use_start_time = ui.edit_usestarttime;
	quint32 usestarttime = use_start_time->dateTime().toTime_t();
	usestarttime -= nSeconds;
	usestarttime = QtCommandtest_work.byteOrderChange32Bit(&usestarttime);
	memcpy(&data_to_send[59], &usestarttime, 4);
//	qint32  useendtime = (ui.edit_useendtime->text()).toInt();
	QDateTimeEdit* use_end_time = ui.edit_useendtime;
	quint32 useendtime = use_end_time->dateTime().toTime_t();
	useendtime -= nSeconds;
	useendtime = QtCommandtest_work.byteOrderChange32Bit(&useendtime);
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

			qDebug() << "id is " << jsonObject.value("id").toString().toInt()
				<< "\n";
			qDebug() << "authCode is " << jsonObject.value("authCode").toString().toInt()
				<< "\n";
			qDebug() << "jobPriority is " << jsonObject.value("jobPriority").toString().toInt()
				<< "\n";
		}
	}
	
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

void QtWorkparameter::json_test()
{
	QFile file( "D:/VS_QT/test.json");
	if (!file.open(QIODevice::ReadWrite)) {
		qDebug() << "文件打开失败!\n";
		exit(1);
	}
	qDebug() << "文件打开成功\n";

	QJsonParseError jsonParserError;
	QJsonDocument   jsonDocument =	QJsonDocument::fromJson(file.readAll(), &jsonParserError);

	if (!jsonDocument.isNull() &&jsonParserError.error == QJsonParseError::NoError)
	{
		qDebug() << "文件解析成功\n";
/*		if (jsonDocument.isObject())
		{
			QJsonObject jsonObject = jsonDocument.object();
			
				qDebug() << "id is " << jsonObject.value("id").toString().toInt()
					<< "\n";
				qDebug() << "satelliteId is " << jsonObject.value("satelliteId").toString().toInt()
					<< "\n";
				qDebug() << "userId is " << jsonObject.value("userId").toString().toInt()
					<< "\n";
				qDebug() << "commSyncWord is " << jsonObject.value("commSyncWord").toString().toInt()
					<< "\n";
				qDebug() << "authCode is " << jsonObject.value("authCode").toString().toInt()
					<< "\n";
		}
		*/

		if (jsonDocument.isArray())
		{ 
			QJsonArray array = jsonDocument.array();  // 转数组 
			int nSize = array.size();
			for (int i = 0; i < nSize; ++i)
			{
				qDebug() << array.at(i).toObject().value("satelliteId").toString();
				qDebug() << array.at(i).toObject().value("stationId").toString();
				qDebug() << array.at(i).toObject().value("commSyncWord").toString();
				if (array.at(i).toObject().value("period").isObject())
				{						
					QJsonObject jsonObject = array.at(i).toObject().value("period").toObject();
					qDebug() << "start is " << jsonObject.value("start").toString().toInt();
					qDebug() << "end is " << jsonObject.value("end").toString().toInt();
				}

			}
		}

	}

	file.close();
	qDebug() << "按任意键退出程序\n";
}

QString QtWorkparameter::time_charge(quint64 sec)
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

	qDebug() << dateTime << endl;
	qDebug() << str;
	str = str.mid(5);
	return str;
}

/*

void QtWorkparameter::date_time_set()
{
	QDateTimeEdit* dateTimeEdit = ui.dateTimeEdit;

	//dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");

	ui.dateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

	dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-1));

}

void QtWorkparameter::date_time_get()
{
	QDateTimeEdit* dateTimeEdit_start = ui.dateTimeEdit;
	int dateTime_start = dateTimeEdit_start->dateTime().toTime_t();

	qDebug() << dateTime_start;

}

*/

qint64 QtWorkparameter::get_second_2010_to_1970()
{
	QDateTime origin_time = QDateTime::fromString("1970-01-01 08:00:00", "yyyy-MM-dd hh:mm:ss");
	QString begin = "2000-01-01 12:00:00";
	QDateTime begin_time = QDateTime::fromString(begin, "yyyy-MM-dd hh:mm:ss");
	qint64 nSeconds = origin_time.secsTo(begin_time);
	return nSeconds;
}

void QtWorkparameter::tcp_to_yunkong_as_client()
{
	tcp_to_ground_ctl = new QTcpSocket(this);
	tcp_to_ground_ctl->connectToHost("192.168.1.254", 6666);

	connect(tcp_to_ground_ctl, SIGNAL(connected()), this, SLOT(tcp_connected_yunkong_success()));
	connect(tcp_to_ground_ctl, SIGNAL(readyRead()), this, SLOT(ground_ctl_tcp_readserver_data()));
}

void QtWorkparameter::tcp_connected_yunkong_success()
{
	ui.plainTextEdit->appendPlainText("连接服务器成功");


	connect(ui.btn_to_Identification, SIGNAL(clicked()), this, SLOT(tcp_send_identity_data_to_ground_ctl()));

}

static qint8 send_data_to_station_switch = 0;
void QtWorkparameter::ground_ctl_tcp_readserver_data()
{
	QByteArray temp = m_tcpClient->readAll();

	QString rev_data = temp;

	static int hsok_num = 0;

	if (rev_data.compare("auth_ok"))
	{		
		ui.plainTextEdit->appendPlainText("身份合法！");
	}
	else if(rev_data.compare("auth_fail"))
	{
		ui.plainTextEdit->appendPlainText("数据错误！");
	}
	else if(rev_data.compare("connect_ok"))
	{
		ui.plainTextEdit->appendPlainText("绑定地面站完成");
		send_data_to_station_switch = 1;
	}
	else if (rev_data.compare("connect_fail"))
	{
		ui.plainTextEdit->appendPlainText("绑定地面站失败");
	}
	else
	{
		ui.plainTextEdit->appendPlainText("返回错误数据");
	}
}

void QtWorkparameter::tcp_send_identity_data_to_ground_ctl()
{
	quint8* data_to_send;
	data_to_send = new quint8(16);

	QString msg_type;
	msg_type = "hshs";
	memcpy(data_to_send, "hshs", 4);
	memcpy(data_to_send+4, "0000", 4);
	memcpy(data_to_send + 8, "0023", 4);
	memcpy(data_to_send + 12, "a0a0", 4);
	tcp_to_ground_ctl->write((char*)data_to_send, 16);
}

void QtWorkparameter::tcp_send_connect_data_to_ground_ctl()
{
	quint8* data_to_send;

	data_to_send = new quint8(8 + stationId.size());

	memcpy(data_to_send, "connect_", 8);
	memcpy(data_to_send + 8, &stationId, stationId.size());
	tcp_to_ground_ctl->write((char*)data_to_send, 8 + stationId.size());
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
