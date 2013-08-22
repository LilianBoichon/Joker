#include "GraphicTestView.h"

GraphicTestView::GraphicTestView(QWidget *parent, QString name)
	: PhGraphicView( parent , name ) , _image(NULL), _font(NULL), _text(NULL), _rect(NULL)
{

}

bool GraphicTestView::init()
{
	playEnable = false;

#ifdef IMAGE
	qDebug() << "GraphicTestView::init";
	if(_image == NULL)
	{
		qDebug() << "Initialize _image";
		_image = new PhGraphicImage;
		_image->setFilename("look.png");
		_image->setTextureCoordinate(1,1);
		_image->setRect(50,0,250,125);
		if (! _image->init())
			qDebug() << "_image not initialize";
	}
#endif

#ifdef TEXT
	if(_font == NULL)
	{
		qDebug() << "Initialize _font";
		_font = new PhFont("Bedizen.ttf",50);
		if (! _font->init())
			qDebug() << "_font not initialize";
	}

	if(_text == NULL)
	{
		qDebug() << "Initialize _text";
		_text = new PhGraphicText;
		_text->setContent("Test PhGraphicText");
		_text->setFont(_font);
		_text->setRect(50,135,250,125);
		if (! _text->init())
			qDebug() << "_text not initialize";

		_text->setX(280);
	}
#endif

#ifdef RECT
	if(_rect == NULL)
	{
		qDebug() << "Initialize _text";
		_rect = new PhGraphicSolidRect(0, 0, 200, 200, 1, new PhColor());
	}
#endif
}

void GraphicTestView::paint()
{
#ifdef IMAGE
	qDebug() << "GraphicTestView::paint";
	if(_image != NULL)
		_image->draw();
#endif

#ifdef TEXT
	if (playEnable == false)
	{
		if(_text != NULL)
		{
			//_text->setX(_text->getX()+4);
			_text->draw();
			if(_text->getX() > this->width())
				_text->setX(0);

		}
	}
	else
	{
		if(_text != NULL)
		{
			qDebug() <<"text pas null";
			_text->setX(_text->getX()+4);
			_text->draw();
			if(_text->getX() > this->width())
				_text->setX(0);

		}
	}
#endif

#ifdef RECT
	_rect->draw();
#endif
}


void GraphicTestView::play()
{
	if (playEnable == true)
		playEnable = false;
	else
		playEnable = true;
}
