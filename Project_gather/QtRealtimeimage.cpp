#include "QtRealtimeimage.h"
#include "QtWorkparameter.h"
#include "QtCommandtest.h"
#include "QtWorkparameter.h"
#include "Project_gather.h"
#include <QFileDialog> 
#include <QMessageBox> 

#pragma execution_character_set("utf-8")

QtRealtimeimage::QtRealtimeimage(QWidget *parent)
	: QWidget(parent)
{
	float_longitude = 0;
	float_latitude = 0;
	ui.setupUi(this);

	connect(ui.btn_to_east, SIGNAL(clicked()), this, SLOT(btn_to_east()));
	connect(ui.btn_to_west, SIGNAL(clicked()), this, SLOT(btn_to_west()));
	connect(ui.btn_to_south, SIGNAL(clicked()), this, SLOT(btn_to_south()));
	connect(ui.btn_to_north, SIGNAL(clicked()), this, SLOT(btn_to_north()));
	connect(ui.btn_to_gather, SIGNAL(clicked()), this, SLOT(btn_gather()));

	connect(ui.btn_to_data_tran, SIGNAL(clicked()), this, SLOT(btn_connect_data_server()));

	connect(ui.btn_to_show, SIGNAL(clicked()), this, SLOT(btn_show_picture()));

	connect(ui.btn_to_connect_udp, SIGNAL(clicked()), this, SLOT(btn_udp_connect_to_sky()));

	
}

QtRealtimeimage::~QtRealtimeimage()
{
}

void QtRealtimeimage::btn_to_east()
{
	QtWorkparameter* QtWorkparameter1;
	QtWorkparameter1 = new QtWorkparameter;
	uchar* initial_data_to_sate = QtWorkparameter1->valid_data_to_send_data();
	QString initial_data_of_str;
	QString initial_longitude, initial_latitude;
	for (int i = 0; i < 100; i++)
	{
		initial_data_of_str += QString("%1").arg(initial_data_to_sate[i] & 0xFF, 2, 16, QLatin1Char('0'));
	}
	ui.plainTextEdit->insertPlainText(initial_data_of_str);
	initial_longitude = initial_data_of_str.mid(22, 8);
	initial_latitude = initial_data_of_str.mid(30, 8);
	initial_longitude = initial_longitude.mid(6, 2) + initial_longitude.mid(4, 2) + initial_longitude.mid(2, 2) + initial_longitude.mid(0, 2);

	int hex_longitude = initial_longitude.toInt(0, 16);
	if (fabs(float_longitude) < 1e-6)
	{
		float_longitude = *(float*)&hex_longitude;
	}
	float_longitude += 0.1;
	unsigned char* longitude_add_east = (unsigned char*)&float_longitude;
	QString str_longitude;
	for (int i = 0; i < 4; i++)
	{
		str_longitude += QString("%1").arg(longitude_add_east[i] & 0xFF, 2, 16, QLatin1Char('0'));
	}
	ui.textEdit_2->insertPlainText(str_longitude);
	initial_data_of_str.replace(22, 8, str_longitude);
	ui.textEdit_3->insertPlainText(initial_data_of_str);

	udp_send_data((quint8*)&initial_data_of_str, 100);
}
	
void QtRealtimeimage::btn_to_west()
{
	QtWorkparameter* QtWorkparameter1;
	QtWorkparameter1 = new QtWorkparameter;
	uchar* initial_data_to_sate = QtWorkparameter1->valid_data_to_send_data();
	QString initial_data_of_str;
	QString initial_longitude;
	for (int i = 0; i < 100; i++)
	{
		initial_data_of_str += QString("%1").arg(initial_data_to_sate[i] & 0xFF, 2, 16, QLatin1Char('0'));
	}
	ui.plainTextEdit->insertPlainText(initial_data_of_str);
	initial_longitude = initial_data_of_str.mid(22, 8);
	initial_longitude = initial_longitude.mid(6, 2) + initial_longitude.mid(4, 2) + initial_longitude.mid(2, 2) + initial_longitude.mid(0, 2);

	int hex_longitude = initial_longitude.toInt(0, 16);
	if (fabs(float_longitude) < 1e-6)
	{
		float_longitude = *(float*)&hex_longitude;
	}
	float_longitude -= 0.1;
	unsigned char* longitude_add_west = (unsigned char*)&float_longitude;
	QString str_longitude;
	for (int i = 0; i < 4; i++)
	{
		str_longitude += QString("%1").arg(longitude_add_west[i] & 0xFF, 2, 16, QLatin1Char('0'));
	}
	ui.textEdit_2->insertPlainText(str_longitude);
	initial_data_of_str.replace(22, 8, str_longitude);
	ui.textEdit_3->insertPlainText(initial_data_of_str);
}

void QtRealtimeimage::btn_to_south()
{
	QtWorkparameter* QtWorkparameter1;
	QtWorkparameter1 = new QtWorkparameter;
	uchar* initial_data_to_sate = QtWorkparameter1->valid_data_to_send_data();
	QString initial_data_of_str;
	QString initial_latitude;
	for (int i = 0; i < 100; i++)
	{
		initial_data_of_str += QString("%1").arg(initial_data_to_sate[i] & 0xFF, 2, 16, QLatin1Char('0'));
	}
	ui.plainTextEdit->insertPlainText(initial_data_of_str);
	initial_latitude = initial_data_of_str.mid(30, 8);
	initial_latitude = initial_latitude.mid(6, 2) + initial_latitude.mid(4, 2) + initial_latitude.mid(2, 2) + initial_latitude.mid(0, 2);

	int hex_latitude = initial_latitude.toInt(0, 16);
	if (fabs(float_latitude) < 1e-6)
	{
		float_latitude = *(float*)&hex_latitude;
	}
	float_latitude += 0.1;
	unsigned char* longitude_add_south = (unsigned char*)&float_latitude;
	QString str_longitude;
	for (int i = 0; i < 4; i++)
	{
		str_longitude += QString("%1").arg(longitude_add_south[i] & 0xFF, 2, 16, QLatin1Char('0'));
	}
	ui.textEdit_2->insertPlainText(str_longitude);
	initial_data_of_str.replace(30, 8, str_longitude);
	ui.textEdit_3->insertPlainText(initial_data_of_str);
}

void QtRealtimeimage::btn_to_north()
{
	QtWorkparameter* QtWorkparameter1;
	QtWorkparameter1 = new QtWorkparameter;
	uchar* initial_data_to_sate = QtWorkparameter1->valid_data_to_send_data();
	QString initial_data_of_str;
	QString initial_latitude;
	for (int i = 0; i < 100; i++)
	{
		initial_data_of_str += QString("%1").arg(initial_data_to_sate[i] & 0xFF, 2, 16, QLatin1Char('0'));
	}
	ui.plainTextEdit->insertPlainText(initial_data_of_str);
	initial_latitude = initial_data_of_str.mid(30, 8);
	initial_latitude = initial_latitude.mid(6, 2) + initial_latitude.mid(4, 2) + initial_latitude.mid(2, 2) + initial_latitude.mid(0, 2);

	int hex_latitude = initial_latitude.toInt(0, 16);
	if (fabs(float_latitude) < 1e-6)
	{
		float_latitude = *(float*)&hex_latitude;
	}
	float_latitude -= 0.1;
	unsigned char* longitude_add_south = (unsigned char*)&float_latitude;
	QString str_longitude;
	for (int i = 0; i < 4; i++)
	{
		str_longitude += QString("%1").arg(longitude_add_south[i] & 0xFF, 2, 16, QLatin1Char('0'));
	}
	ui.textEdit_2->insertPlainText(str_longitude);
	initial_data_of_str.replace(30, 8, str_longitude);
	ui.textEdit_3->insertPlainText(initial_data_of_str);

}

void QtRealtimeimage::btn_gather()
{
	Project_gather* Project_login = new Project_gather;
	Project_login->show();
	this->hide();
}

void QtRealtimeimage::btn_connect_data_server()
{
	tcpClient = new QTcpSocket(this);

	connect(tcpClient, SIGNAL(connected()), this, SLOT(connect_data_server_success()));

	connect(tcpClient, SIGNAL(readyRead()), this, SLOT(read_data_server()));

	tcpClient->connectToHost("192.168.2.228", 10086);
}

void QtRealtimeimage::connect_data_server_success() {

	ui.plainTextEdit->appendPlainText("连接数传成功");
}

void QtRealtimeimage::read_data_server()
{
	QByteArray temp = tcpClient->readAll();
	QDataStream out(&temp, QIODevice::ReadWrite);    //将字节数组读入
	data_tran_frame_realtime_t data_tran_frame;
	while (!out.atEnd())
	{
		out >> data_tran_frame.frame_head;
		data_tran_frame.frame_head = qFromBigEndian(data_tran_frame.frame_head);
		if (data_tran_frame.frame_head != 0XEB90)
		{
			qDebug() << "帧头错误！";
			return;
		}
		out >> data_tran_frame.frame_type;
		if (data_tran_frame.frame_type != 0Xdd)
		{
			qDebug() << "非数传类型！";
			return;
		}
		out >> data_tran_frame.frame_len;
		data_tran_frame.frame_len = qFromBigEndian(data_tran_frame.frame_len);
		out >> data_tran_frame.frame_aisle;
		if (data_tran_frame.frame_aisle != 0X0d)
		{
			qDebug() << "非数传通道！";
			return;
		}
		out >> data_tran_frame.frame_serial;
		out >> data_tran_frame.frame_time;
		if (data_tran_frame.frame_len - 10 > 0)
		{
			data_tran_frame.frame_data = new quint8[data_tran_frame.frame_len - 10];
		}
		else
		{
			qDebug() << "数据错误";
			return;
		}
		for (quint64 i = 0; i < data_tran_frame.frame_len - 10; i++)
		{
			out >> data_tran_frame.frame_data[i];
		}
		out >> data_tran_frame.frame_sum;
	}
	
	QPixmap imageresult;//

//	temp = temp.remove(0, 15);
//	imageresult.loadFromData(data_tran_frame.frame_data, data_tran_frame.frame_len - 10);
//	bool ret = imageresult.save("d:/1.jpg");

//	QString data((data_tran_frame.frame_data));
//	QByteArray imageData = QByteArray::fromBase64(data.toLatin1());
//	QByteArray imageData = temp.mid(10, data_tran_frame.frame_len - 10);
	QImage image;
	image.loadFromData(data_tran_frame.frame_data, data_tran_frame.frame_len - 10);
	bool ret = image.save("d:/1.jpg");
	if (ret == true)
	{
		qDebug() << "保存成功！";
	}
	else
	{
		qDebug() << "保存失败！";
	}
	
}

void QtRealtimeimage::tcp_set_param_format_to_data_server()
{
	data_trans_req_frame_realtime_t data_trans_req_frame;
	data_trans_req_frame.frame_head = 0XEB90;
	data_trans_req_frame.frame_type = 0X02;
	data_trans_req_frame.frame_len = 0x9;
	data_trans_req_frame.frame_data = 0x0100;

	data_trans_req_frame.frame_sum = check_sum((quint8*)&data_trans_req_frame.frame_data, data_trans_req_frame.frame_len);

	if (tcpClient)
	{
		tcpClient->write((char*)&data_trans_req_frame, 8);//发送到串口
	}
	else
	{
		ui.plainTextEdit->setPlainText("句柄读取失败");
	}

	ui.plainTextEdit->setPlainText("发送数传接收命令成功");
}

void QtRealtimeimage::tcp_send_data_trans_req_to_data_server()
{
	data_trans_req_frame_realtime_t data_trans_req_frame;
	data_trans_req_frame.frame_head = 0XEB90;
	data_trans_req_frame.frame_type = 0Xdd;
	data_trans_req_frame.frame_len = 0x2;
	data_trans_req_frame.frame_data = 0x0100;

	data_trans_req_frame.frame_sum = check_sum((quint8*)&data_trans_req_frame.frame_data, data_trans_req_frame.frame_len);

	if (tcpClient)
	{
		tcpClient->write((char*)&data_trans_req_frame, 8);//发送到串口
	}
	else
	{
		ui.plainTextEdit->setPlainText("句柄读取失败");
		return;
	}

	ui.plainTextEdit->setPlainText("发送数传接收命令成功");
}

quint8 QtRealtimeimage::check_sum(quint8* a, int len)
{
	unsigned int sum = 0;

	while (len > 1) {
		sum += *a++;
		len -= 1;
	}

	if (len) {
		sum += *(unsigned char*)a;
	}

	while (sum >> 8) {
		sum = (sum >> 8) + (sum & 0xff);
	}

	return (quint8)(~sum);
}

void QtRealtimeimage::btn_show_picture()
{
	QString filename;
	filename = QFileDialog::getOpenFileName(this,
		tr("选择图像"),
		"",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	if (filename.isEmpty())
	{
		return;
	}
	else
	{
		QImage* img = new QImage;

		if (!(img->load(filename))) //加载图像
		{
			QMessageBox::information(this,
				tr("打开图像失败"),
				tr("打开图像失败!"));
			delete img;
			return;
		}
		ui.label_show->setPixmap(QPixmap::fromImage(*img));
	}
}

void QtRealtimeimage::btn_udp_connect_to_sky()
{
	sender1 = new QUdpSocket(this);

	sender1->bind(10086, QUdpSocket::ShareAddress);

	connect(sender1, SIGNAL(readyRead()), this, SLOT(udp_rev_data_from_sky()));

	qDebug() << "have initial UDP connect!";
}

void QtRealtimeimage::udp_rev_data_from_sky() //处理等待的数据报
{
	while (sender1->hasPendingDatagrams()) //拥有等待的数据报
	{
		QByteArray datagram; //拥于存放接收的数据报
		datagram.resize(sender1->pendingDatagramSize());
		sender1->readDatagram(datagram.data(), datagram.size());
		unsigned short size = datagram.size();
		unsigned char msg[10] = { 0 };
		memcpy(msg, datagram.data(), size);
		unsigned short arr = msg[0];
		arr = (arr << 8) + msg[1];
		qDebug() << msg[0] << msg[1] << msg[2] << msg[3];
	}
}

void QtRealtimeimage::udp_send_data(quint8* total_data_to_send, quint16 total_data_len)
{
	quint8* data_to_send;
	data_to_send = new uchar[10 + total_data_len];

	memset(data_to_send, 0, 21);
	quint32 smg_head = 0x7EA58C91;
	quint16 smg_type = 0x0104;
	quint16 smg_len = total_data_len + 1;
	quint8  smg_crc = 0x88;
	memcpy(data_to_send, &smg_head, 4);
	memcpy(data_to_send + 4, &smg_type, 2);
	memcpy(data_to_send + 6, &smg_len, 2);
	memset(data_to_send + 8, 0, 1);
	memcpy(data_to_send + 9, &total_data_to_send, total_data_len);

	memcpy(data_to_send + 10 + total_data_len - 1, &smg_crc, 1);

	sender1->writeDatagram((char*)data_to_send, total_data_len + 10, QHostAddress("192.168.2.228"), 10087);
}