#include "QtRealtimeimage.h"

#include "QtWorkparameter.h"

#include "QtCommandtest.h"
#include "QtWorkparameter.h"
#include "Project_gather.h"
/*

#include <shlobj.h> // ¿ÉÌæ»»Îª windows.h
#include <shellapi.h>

#define __STDC_CONSTANT_MACROS

using namespace std;

extern "C"
{
	#include "libavcodec/avcodec.h "
}
*/

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
	ui.textEdit->insertPlainText(initial_data_of_str);
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
	ui.textEdit->insertPlainText(initial_data_of_str);
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
	ui.textEdit->insertPlainText(initial_data_of_str);
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
	ui.textEdit->insertPlainText(initial_data_of_str);
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
