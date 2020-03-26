#pragma once

#include <QWidget>
#include "ui_QtWorkparameter.h"

class QtWorkparameter : public QWidget
{
	Q_OBJECT

public:
	QtWorkparameter(QWidget *parent = Q_NULLPTR);
	~QtWorkparameter();

private:
	Ui::QtWorkparameter ui;

private slots:
	void on_btn_return_to_state_clicked();
	void on_btn_taskinput_clicked();
	void on_btn_applyauthorizationcode_clicked();
	void btn_quit();
};
