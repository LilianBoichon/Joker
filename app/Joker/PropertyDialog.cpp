#include "PropertyDialog.h"
#include "ui_PropertyDialog.h"

PropertyDialog::PropertyDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PropertyDialog),
	_doc(NULL)
{
	ui->setupUi(this);
}

PropertyDialog::~PropertyDialog()
{
	delete ui;
}

void PropertyDialog::setDoc(PhStripDoc *doc)
{
	_doc = doc;
}

void PropertyDialog::setVideoEngine(PhVideoEngine *videoEngine)
{
	_videoEngine = videoEngine;
}

void PropertyDialog::showEvent(QShowEvent *)
{
	ui->titleLabel->setText("-");
	ui->tcInLabel->setText("-");
	ui->tcOutLabel->setText("-");
	ui->authorLabel->setText("-");
	ui->peopleNumberLabel->setText("-");
	ui->charNumberLabel->setText("-");
	ui->videoFileLabel->setText("-");
	ui->timestampLabel->setText("-");

	if(_doc)
	{
		ui->titleLabel->setText(_doc->getTitle());

		if(_doc->getAuthorName().length())
			ui->authorLabel->setText(_doc->getAuthorName());

		PhFrame frameIn = _doc->getFrameIn();
		if(frameIn > 0)
			ui->tcInLabel->setText(PhTimeCode::stringFromFrame(frameIn, _doc->getTCType()));

		PhFrame frameOut = _doc->getFrameOut();
		if(frameOut > 0)
			ui->tcOutLabel->setText(PhTimeCode::stringFromFrame(frameOut, _doc->getTCType()));

		int peopleNumber = _doc->getPeoples().count();
		ui->peopleNumberLabel->setText(QString::number(peopleNumber));

		int charNumber = 0;
		foreach(PhStripText * text, _doc->getTexts())
			charNumber += text->getContent().length();
		ui->charNumberLabel->setText(QString::number(charNumber));
	}

	if(_videoEngine)
	{
		if(_videoEngine->fileName().length())
			ui->videoFileLabel->setText(_videoEngine->fileName());

		if(_videoEngine->frameStamp())
			ui->timestampLabel->setText(PhTimeCode::stringFromFrame(_videoEngine->frameStamp(), _videoEngine->clock()->timeCodeType()));
	}
}