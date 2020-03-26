/*
#include "GenerateVerificationCode.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GenerateVerificationCode w;
	w.show();
	return a.exec();
}

*/

#include "QtLogin.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QtLogin w;
	w.show();
	return a.exec();
}