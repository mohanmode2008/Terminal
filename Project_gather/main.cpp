#include "Project_gather.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Project_gather w;
	w.show();
	return a.exec();
}
