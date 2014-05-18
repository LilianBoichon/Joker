/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>

#include "PhTools/PhDebug.h"
#include "PhGraphic/PhGraphicDisc.h"
#include "PhGraphic/PhGraphicDashedLine.h"
#include "PhGraphic/PhGraphicArrow.h"
#include "PhGraphicStrip.h"

PhGraphicStrip::PhGraphicStrip(PhGraphicStripSettings *settings) :
	_settings(settings),
	_clock(_doc.timeCodeType()),
	_trackNumber(4),
	_maxDrawElapsed(0)
{
	// update the  content when the doc changes :
	this->connect(&_doc, SIGNAL(changed()), this, SLOT(onDocChanged()));

	_stripBackgroundImage.setFilename(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/motif-240.png");
	_stripBackgroundImageInverted.setFilename(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/motif-240_black.png");

	PHDEBUG << "Load the font file";
	QString fontFile = "";
	if(_settings != NULL)
		fontFile = _settings->textFontFile();
	else
		PHDEBUG << "no settings...";

	if(!QFile(fontFile).exists()) {
		PHDEBUG << "File not found:" << fontFile;
		fontFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/" + "SWENSON.TTF";
		if(_settings != NULL)
			_settings->textFontFile();
		else
			PHDEBUG << "no settings...";
	}
	_textFont.setFontFile(fontFile);

	if(_settings != NULL)
		_textFont.setBoldness(_settings->textBoldness());

	_hudFont.setFontFile(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/" + "HelveticaCYPlain.ttf");

	// This is used to make some time-based test
	_testTimer.start();

}

PhStripDoc *PhGraphicStrip::doc()
{
	return &_doc;
}

PhClock *PhGraphicStrip::clock()
{
	return &_clock;
}

void PhGraphicStrip::setFontFile(QString fontFile)
{
#warning /// @todo remove and using settings instead
	_textFont.setFontFile(fontFile);
}

void PhGraphicStrip::onDocChanged()
{
	_trackNumber = 4;
	foreach(PhStripText *text, _doc.texts()) {
		if(text->track() >= _trackNumber)
			_trackNumber = text->track() + 1;
	}

	foreach(PhStripDetect *detect, _doc.detects()) {
		if(detect->track() >= _trackNumber)
			_trackNumber = detect->track() + 1;
	}
}

PhFont *PhGraphicStrip::getTextFont()
{
	return &_textFont;
}

PhFont *PhGraphicStrip::getHUDFont()
{
	return &_hudFont;
}

QColor PhGraphicStrip::computeColor(PhPeople * people, QList<PhPeople*> selectedPeoples, bool invertColor)
{
	if(!invertColor) {
		if(people) {
			if(selectedPeoples.size() && !selectedPeoples.contains(people)) {
				return QColor(100, 100, 100);
			}
			else {
				return people->color();
			}
		}
		else if(selectedPeoples.size())
			return QColor(100, 100, 100);
		else
			return Qt::black;
	}
	else {
		if(people) {
			if(selectedPeoples.size() && !selectedPeoples.contains(people)) {
				return QColor(155, 155, 155);
			}
			else {
				QColor color(people->color());
				return QColor(255 - color.red(), 255 - color.green(), 255 - color.blue());
			}
		}
		else if(selectedPeoples.size())
			return QColor(155, 155, 155);
		else
			return Qt::white;
	}
}

void PhGraphicStrip::draw(int x, int y, int width, int height, int tcOffset, QList<PhPeople *> selectedPeoples)
{
	// Just to preload the font in order to avoid font loading during playback
	_textFont.select();
	_hudFont.select();

	_infos.clear();

	int counter = 0;
	bool invertedColor = _settings->invertColor();

	int lastDrawElapsed = _testTimer.elapsed();
	//PHDEBUG << "time " << _clock.time() << " \trate " << _clock.rate();

	if(height > 0) {
		int timePerPixel = _settings->horizontalTimePerPixel();
		_textFont.setBoldness(_settings->textBoldness());
		_textFont.setFontFile(_settings->textFontFile());

		int loopCounter = 0;
		int offCounter = 0;
		int cutCounter = 0;

		long syncBar_X_FromLeft = width / 6;
		long delay = (int)(24 * _settings->screenDelay() *  _clock.rate());
		PhTime clockTime = _clock.time() + delay;
		long offset = clockTime / timePerPixel - syncBar_X_FromLeft;

		//Compute the visible duration of the strip
		PhTime stripDuration = width * timePerPixel;


		if(_settings->stripTestMode()) {
			foreach(PhStripCut * cut, _doc.cuts())
			{
				counter++;
				if(cut->timeIn() == clockTime) {
					PhGraphicSolidRect white(x, y, width, height);
					white.setColor(Qt::white);
					white.draw();

					//This is useless to continue the foreach if the cut is displayed.
					break;
				}
			}
			return;
		}

		PhTime timeIn = clockTime - syncBar_X_FromLeft * timePerPixel;
		PhTime timeOut = timeIn + stripDuration;

		//Draw backgroung picture
		int n = width / height + 2; // compute how much background repetition do we need
		long leftBG = 0;
		if(offset >= 0)
			leftBG -= offset % height;
		else
			leftBG -= height - ((-offset) % height);

		PhGraphicTexturedRect* backgroundImage = &_stripBackgroundImage;
		if(invertedColor)
			backgroundImage = &_stripBackgroundImageInverted;

		backgroundImage->setX(x + leftBG);
		backgroundImage->setY(y);
		backgroundImage->setSize(height * n, height);
		backgroundImage->setZ(-2);
		backgroundImage->setTextureCoordinate(n, 1);
		backgroundImage->draw();

		PhGraphicSolidRect syncBarRect;
		syncBarRect.setColor(QColor(225, 86, 108));
		syncBarRect.setSize(4, height);
		syncBarRect.setPosition(x + width/6, y, 0);

		syncBarRect.draw();

		if(_settings->displayRuler()) {
			PhTime rulerTimeIn = _settings->rulerTimeIn();
			PhTime timeBetweenRuler = _settings->timeBetweenRuler();
			int rulerNumber = (timeIn - rulerTimeIn) / timeBetweenRuler;
			if (rulerNumber < 0)
				rulerNumber = 0;

			PhTime rulerTime = rulerTimeIn + rulerNumber * timeBetweenRuler;
			PhGraphicSolidRect rulerRect;
			PhGraphicDisc rulerDisc;
			PhGraphicText rulerText(&_hudFont);
			QColor rulerColor(80, 80, 80);
			if(invertedColor)
				rulerColor = Qt::white;

			int width = 1000 / timePerPixel;

			rulerRect.setColor(rulerColor);
			rulerRect.setWidth(width);
			rulerRect.setHeight(height / 2);
			rulerRect.setZ(0);
			rulerRect.setY(y);

			rulerDisc.setColor(rulerColor);
			rulerDisc.setRadius(2 * width);
			rulerDisc.setY(y + height / 2 + 3 * width);
			rulerDisc.setZ(0);

			rulerText.setColor(rulerColor);
			rulerText.setY(y + height / 2);
			rulerText.setHeight(height / 2);
			rulerText.setZ(0);


			while (rulerTime < timeOut + timeBetweenRuler) {
				counter++;
				int x = rulerTime / timePerPixel - offset;

				rulerRect.setX(x - rulerRect.width() / 2);
				rulerRect.draw();

				QString text = QString::number(rulerNumber);
				rulerText.setContent(text);
				int textWidth = _hudFont.getNominalWidth(text);
				rulerText.setWidth(textWidth);
				rulerText.setX(x - textWidth / 2);
				rulerText.draw();

				x += timeBetweenRuler / timePerPixel / 2;

				rulerRect.setX(x - rulerRect.width() / 2);
				rulerRect.draw();

				rulerDisc.setX(x);
				rulerDisc.draw();

				rulerNumber++;
				rulerTime += timeBetweenRuler;
			}
		}

		int minTimeBetweenPeople = 48000;
		int timeBetweenPeopleAndText = 4000;
		PhStripText ** lastTextList = new PhStripText*[_trackNumber];
		for(int i = 0; i < _trackNumber; i++)
			lastTextList[i] = NULL;

		int trackHeight = height / _trackNumber;

		int verticalTimePerPixel = _settings->verticalTimePerPixel();
		bool displayNextText = _settings->displayNextText();
		PhTime maxTimeIn = timeOut;
		if(displayNextText)
			maxTimeIn += y * verticalTimePerPixel;

		foreach(PhStripText * text, _doc.texts())
		{
			int track = text->track();

			if( !((text->timeOut() < timeIn) || (text->timeIn() > timeOut)) ) {
				counter++;
				PhGraphicText gText(&_textFont, text->content());
				gText.setZ(-1);

				gText.setX(x + text->timeIn() / timePerPixel - offset);
				gText.setWidth((text->timeOut() - text->timeIn()) / timePerPixel);
				gText.setY(y + track * trackHeight);
				gText.setHeight(trackHeight);
				gText.setZ(-1);
				gText.setColor(computeColor(text->people(), selectedPeoples, invertedColor));

				gText.draw();
			}

			PhPeople * people = text->people();
			QString name = people ? people->name() : "???";
			PhGraphicText gPeople(&_hudFont, name);
			gPeople.setWidth(name.length() * 12);

			PhStripText * lastText = lastTextList[track];
			// Display the people name only if one of the following condition is true:
			// - it is the first text
			// - it is a different people
			// - the distance between the latest text and the current is superior to a limit
			if((
			       (lastText == NULL)
			       || (lastText->people() != text->people())
			       || (text->timeIn() - lastText->timeOut() > minTimeBetweenPeople))
			   ) {

				gPeople.setX(x + (text->timeIn() - timeBetweenPeopleAndText) / timePerPixel - offset - gPeople.width());
				gPeople.setY(y + track * trackHeight);
				gPeople.setZ(-1);
				gPeople.setHeight(trackHeight / 2);

				gPeople.setColor(computeColor(people, selectedPeoples, invertedColor));

				gPeople.draw();
			}

			PhTime timePerPeopleHeight = gPeople.height() * verticalTimePerPixel;

			if(displayNextText && (timeIn < text->timeIn() + timePerPeopleHeight) && ((lastText == NULL) || (text->timeIn() - lastText->timeOut() > minTimeBetweenPeople))) {
				PhPeople * people = text->people();

				int howFarIsText = (text->timeIn() - clockTime) / verticalTimePerPixel;
				//This line is used to see which text's name will be displayed
				gPeople.setX(width - gPeople.width());
				gPeople.setY(y - howFarIsText - gPeople.height());

				gPeople.setZ(-3);
				gPeople.setHeight(trackHeight / 2);

				gPeople.setColor(computeColor(people, selectedPeoples, invertedColor));

				PhGraphicSolidRect background(gPeople.x(), gPeople.y(), gPeople.width(), gPeople.height() + 2);
				if(selectedPeoples.size() && !selectedPeoples.contains(people))
					background.setColor(QColor(90, 90, 90));
				else
					background.setColor(QColor(180, 180, 180));

				background.setZ(gPeople.z() - 1);

				if(gPeople.y() > tcOffset) {
					if(!invertedColor)
						background.draw();

					gPeople.draw();
				}
			}

			lastTextList[track] = text;

			if(text->timeIn() > maxTimeIn)
				break;
		}

		delete lastTextList;

		foreach(PhStripCut * cut, _doc.cuts())
		{
			//_counter++;
			if( (timeIn < cut->timeIn()) && (cut->timeIn() < timeOut)) {
				PhGraphicSolidRect gCut;
				gCut.setZ(-1);
				gCut.setWidth(2);

				if(invertedColor)
					gCut.setColor(QColor(255, 255, 255));
				else
					gCut.setColor(QColor(0, 0, 0));
				gCut.setHeight(height);
				gCut.setX(x + cut->timeIn() / timePerPixel - offset);
				gCut.setY(y);

				gCut.draw();
				cutCounter++;
			}
			//Doesn't need to process undisplayed content
			if(cut->timeIn() > timeOut)
				break;
		}

		foreach(PhStripLoop * loop, _doc.loops())
		{
			//_counter++;
			// This calcul allow the cross to come smoothly on the screen (height * timePerPixel / 8)
			if( ((loop->timeIn() + height * timePerPixel / 8) > timeIn) && ((loop->timeIn() - height * timePerPixel / 8 ) < timeOut)) {
				PhGraphicLoop gLoop;
				if(!invertedColor)
					gLoop.setColor(Qt::black);
				else
					gLoop.setColor(Qt::white);

				gLoop.setX(x + loop->timeIn() / timePerPixel - offset);
				gLoop.setY(y);
				gLoop.setZ(-1);
				gLoop.setHThick(height / 40);
				gLoop.setHeight(height);
				gLoop.setCrossHeight(height / 4);
				gLoop.setWidth(height / 4);

				gLoop.draw();
				loopCounter++;
			}

			if(displayNextText && ((loop->timeIn() + height * timePerPixel / 8) > timeIn)) {
				PhGraphicLoop gLoopPred;

				int howFarIsLoop = (loop->timeIn() - clockTime) / verticalTimePerPixel;
				gLoopPred.setColor(Qt::white);

				gLoopPred.setHorizontalLoop(true);
				gLoopPred.setZ(-3);

				gLoopPred.setX(width - width / 10);
				gLoopPred.setY(y - howFarIsLoop);
				gLoopPred.setHeight(30);

				gLoopPred.setHThick(3);
				gLoopPred.setCrossHeight(20);
				gLoopPred.setWidth(width / 10);

				gLoopPred.draw();
			}
			if((loop->timeIn() - height * timePerPixel / 8) > timeOut + 25 * 30)
				break;
		}

		foreach(PhStripDetect * detect, _doc.detects())
		{
			//_counter++;

			if((timeIn < detect->timeOut()) && (detect->timeIn() < timeOut) ) {
				PhGraphicRect *gDetect = NULL;
				switch (detect->type()) {
				case PhStripDetect::Off:
					gDetect = new PhGraphicSolidRect();
					gDetect->setY(y + detect->track() * trackHeight + trackHeight * 0.9);
					gDetect->setHeight(trackHeight / 10);
					break;
				case PhStripDetect::SemiOff:
					gDetect = new PhGraphicDashedLine((detect->timeOut() - detect->timeIn()) / 1200);
					gDetect->setY(y + detect->track() * trackHeight + trackHeight * 0.9);
					gDetect->setHeight(trackHeight / 10);
					break;
				case PhStripDetect::ArrowUp:
					gDetect = new PhGraphicArrow(PhGraphicArrow::DownLeftToUpRight);
					gDetect->setY(y + detect->track() * trackHeight);
					gDetect->setHeight(trackHeight);
					break;
				case PhStripDetect::ArrowDown:
					gDetect = new PhGraphicArrow(PhGraphicArrow::UpLefToDownRight);
					gDetect->setY(y + detect->track() * trackHeight);
					gDetect->setHeight(trackHeight);
					break;
				default:
					break;
				}

				if(gDetect) {
					gDetect->setColor(computeColor(detect->people(), selectedPeoples, invertedColor));

					gDetect->setX(x + detect->timeIn() / timePerPixel - offset);
					gDetect->setZ(-1);
					gDetect->setWidth((detect->timeOut() - detect->timeIn()) / timePerPixel);
					gDetect->draw();
					offCounter++;
					delete gDetect;
				}
			}
			//Doesn't need to process undisplayed content
			if(detect->timeIn() > timeOut)
				break;
		}
	}

	//	PHDEBUG << "off counter : " << offCounter << "cut counter : " << cutCounter << "loop counter : " << loopCounter;

	int currentDrawElapsed = _testTimer.elapsed() - lastDrawElapsed;
	if(currentDrawElapsed > _maxDrawElapsed)
		_maxDrawElapsed = currentDrawElapsed;
	_testTimer.restart();

	_infos.append(QString("Max strip draw: %1").arg(_maxDrawElapsed));
	_infos.append(QString("Count: %1").arg(counter));

	if(_settings->resetInfo())
		_maxDrawElapsed = 0;
}
