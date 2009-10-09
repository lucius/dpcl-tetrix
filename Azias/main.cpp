#include <QApplication>
#include "AziaMaster.h"

int main(int argc, char* argv[])
{
	QApplication app(argc,argv);
	AziaMaster* w = new AziaMaster();

	return app.exec();
}
