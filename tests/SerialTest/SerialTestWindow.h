#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>

#include "PhTools/PhTickCounter.h"

#include "SerialTestSettings.h"

namespace Ui {
class SerialTestWindow;
}

class SerialTestWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit SerialTestWindow(SerialTestSettings *settings);
	~SerialTestWindow();

private slots:
	void sendTextA();
	void readTextA();
	void sendTextB();
	void readTextB();

	void on_checkA_toggled(bool checked);

	void on_checkB_toggled(bool checked);
	void checkCTS();

	void onAvailablePortA_trigger();
	void onAvailablePortB_trigger();

private:
	bool open(QSerialPort * serial, QString suffix);

	SerialTestSettings *_settings;

	Ui::SerialTestWindow *ui;

	QSerialPort _serialA, _serialB;
	QTimer _ctsTimer;
	PhTickCounter _ctsCounter, _timerCounter;
	bool _lastCTS;
};

#endif // MAINWINDOW_H
