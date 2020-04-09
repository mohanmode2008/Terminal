#pragma once

#include <QWidget>
#include "ui_Qttimedimage.h"

class Qttimedimage : public QWidget
{
	Q_OBJECT

public:
	Qttimedimage(QWidget *parent = Q_NULLPTR);
	~Qttimedimage();

private:
	Ui::Qttimedimage ui;
};
