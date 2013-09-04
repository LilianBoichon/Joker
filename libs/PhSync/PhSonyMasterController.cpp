#include "PhSonyMasterController.h"

#include <QDebug>

PhSonyMasterController::PhSonyMasterController(QObject *parent)
	: PhSonyController("B", parent )
{
}

void PhSonyMasterController::deviceTypeRequest()
{
	qDebug() << "Device type request";
	sendCommand(0x00, 0x11);
}

void PhSonyMasterController::play()
{
	qDebug() << "Play";
	sendCommand(0x20, 0x01);
}

void PhSonyMasterController::stop()
{
	qDebug() << "Stop";
	sendCommand(0x20, 0x00);
}

void PhSonyMasterController::fastForward()
{
	qDebug() << "Fast forward";
	sendCommand(0x20, 0x10);
}

void PhSonyMasterController::rewind()
{
	qDebug() << "Rewind";
	sendCommand(0x20, 0x20);
}

void PhSonyMasterController::jog(PhRate rate)
{
	qDebug() << "Jog " << rate;
	unsigned char data1;
	if (rate < 0)
	{
		data1 = computeData1(-rate);
		sendCommand(0x21, 0x21, data1);
	}
	else
	{
		data1 = computeData1(rate);
		sendCommand(0x21, 0x11, data1);
	}
}

void PhSonyMasterController::varispeed(PhRate rate)
{
	qDebug() << "Varispeed " << rate;
	unsigned char data1;
	if (rate < 0)
	{
		data1 = computeData1(-rate);
		sendCommand(0x21, 0x22, data1);
	}
	else
	{
		data1 = computeData1(rate);
		sendCommand(0x21, 0x12, data1);
	}
}

void PhSonyMasterController::shuttle(PhRate rate)
{
	qDebug() << "Shuttle " << rate;
	unsigned char data1;
	if (rate < 0)
	{
		data1 = computeData1(-rate);
		sendCommand(0x21, 0x23, data1);
	}
	else
	{
		data1 = computeData1(rate);
		sendCommand(0x21, 0x13, data1);
	}
}

void PhSonyMasterController::timeSense()
{
	qDebug() << "Time sense";
	sendCommand(0x61, 0x0c, 1);
}

void PhSonyMasterController::statusSense()
{
	qDebug() << "Status sense";
	sendCommand(0x61, 0x20, 4);
}

void PhSonyMasterController::processCommand(unsigned char cmd1, unsigned char cmd2, const unsigned char *data)
{
	qDebug() << "PhSonyMasterController::processCommand : " << stringFromCommand(cmd1, cmd2, data);
	switch (cmd1 >> 4)
	{
	case 1:
		switch (cmd2)
		{
		case 0x01:
			qDebug() << " => ACK";
			break;
		case 0x11:
		{
			deviceIdData(data[0], data[1]);
			QString id;
			id.sprintf("%02X %02X", data[0], data[1]);
			qDebug() << " => Device ID answer : " << id;
			break;
		}
		case 0x12:
			qDebug() << " => NAK :" <<  QString::number(data[0], 16);
			break;
		default:
			qDebug() << " => Unknown answer : " << QString("%x %x").arg(cmd1, cmd2);
			break;
		}
		break;
	case 7:
		switch (cmd2)
		{
		case 0x04:
		{
			PhFrame frame = PhTimeCode::frameFromBcd(*(unsigned int *)data, _clock.getTCType());
			qDebug() << " => LTC Time Data : " << PhTimeCode::stringFromFrame(frame, _clock.getTCType());
			_clock.setFrame(frame);
			break;
		}
		case 0x20:
		{
			unsigned char status[4];
			QString statusStr = "";
			for (int i = 0; i < 4; i++)
			{
				status[i] = data[i];
				statusStr += QString::number(data[i], 16) + " ";
			}
			statusData(4, status);
			qDebug() << " => Status data : " << statusStr;
			break;
		}
		default:
			qDebug() << " => Unknown answer : " << QString::number(cmd1, 16) << " " << QString::number(cmd2, 16);
			break;
		}
		break;
	default:
		qDebug() << " => Unknown answer : " << QString::number(cmd1, 16) << " " << QString::number(cmd2, 16);
		break;
	}
	qDebug() << "PhSonyMasterController::processCommand : " << stringFromCommand(cmd1, cmd2, data) << " over";

}
