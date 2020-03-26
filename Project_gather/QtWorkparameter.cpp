#include "QtWorkparameter.h"

#include "Project_gather.h"

#include <QDateEdit>

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
}

QtWorkparameter::~QtWorkparameter()
{
}


void QtWorkparameter::on_btn_return_to_state_clicked()
{
	Project_gather* Project_login = new Project_gather;
	Project_login->show();
	this->hide();
}


void QtWorkparameter::on_btn_taskinput_clicked()
{

	QString longitude = ui.lineEdit_longitude->text();
	QString latitude  = ui.lineEdit_latitude->text();

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

	ui.plainTextEdit->appendPlainText(camerastring);

}


void QtWorkparameter::on_btn_applyauthorizationcode_clicked()
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

}

void QtWorkparameter::btn_quit()
{
	this->close();
}

