#pragma once

#include <QWidget>
#include "ui_QtRealtimeimage.h"

class QtRealtimeimage : public QWidget
{
	Q_OBJECT

public:
	QtRealtimeimage(QWidget *parent = Q_NULLPTR);
	~QtRealtimeimage();

private:
	Ui::QtRealtimeimage ui;
};
