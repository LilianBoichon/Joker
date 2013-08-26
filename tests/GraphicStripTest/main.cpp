/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QApplication>
#include <QDebug>

#include "PhStrip/PhStripDoc.h"

#include "MainView.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainView w;

	//	QString file;
	//    if (argc < 2)
	//		file = "Tunnel Rats.detx";
	//    else
	//        file = argv[1];
	//w.openFile(QString(file));

	w.openFile(QString("test.detx"));

    w.show();

    return a.exec();

}
