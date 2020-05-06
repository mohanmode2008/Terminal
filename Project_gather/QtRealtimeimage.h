#pragma once

#include <QWidget>
#include "ui_QtRealtimeimage.h"

class QtRealtimeimage : public QWidget
{
	Q_OBJECT

public:
	QtRealtimeimage(QWidget *parent = Q_NULLPTR);
	~QtRealtimeimage();

public slots:
	void btn_to_east();
	void btn_to_west();
	void btn_to_south();
	void btn_to_north();
//	void btn_to_ffpmeg();

private:
	Ui::QtRealtimeimage ui;

	float float_longitude;
	float float_latitude;
};
