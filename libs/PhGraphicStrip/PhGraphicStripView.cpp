/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QTime>


#include "SDL/SDL.h"

#include "PhGraphicStripView.h"




PhGraphicStripView::PhGraphicStripView(QWidget *parent)
    : PhGraphicView( parent, "StripTest")
{


    // This is used to make some time-based test
    _test = new QTime();
    _test->start();

    // This is a preload of default fonts
#if defined(Q_OS_MAC)
    _fonts.push_back( new PhFont(QCoreApplication::applicationDirPath() + "/../Resources/fonts/LTE50198.ttf", 150));

#elif defined(Q_OS_UNIX)
  _fonts.push_back( new PhFont(QCoreApplication::applicationDirPath() + "/data/fonts/Bedizen.ttf", 150));
#endif


    // Set current font
    _currentFont = _fonts.first();
    _xmove = 0;
    _xMoveStrip = 0;

    // The strip shouldn't move until a file is loaded
    _shouldmove = false;
}


void PhGraphicStripView::initializeGL()
{
    //Set the background color to white
    glClearColor(1,1,1,0);

    //Store the OpenGL context height in a variable in order to
    //limit functions call
    int h = this->height();

    //Set the ratio between the strip's height and the OpenGL height
    int hstrip = h/3;

    //This clear the data stored
    clearData();

    _xmove = - _controller->getDoc().getLastPosition();
    int max = _controller->getDoc().getTexts().count();
    QProgressDialog barTest("Création des textures","Ok", 0, max, this);

    barTest.move(400,400);
    // barTest.show();

//    int i = 0;
    //Load the all text
    foreach(PhStripText * it, _controller->getDoc().getTexts())
    {
        //barTest.setValue(i);

        //h is the window height, hstrip is the strip height
        //hstrip/16 correspond to the upper alpha line of the strip
        //hstrip/8 correspond to the two alpha lines of the strip (up & down)
        //it->getTrack() is the position on the strip (0 is the upper on)
        //we split   in 3 because we are using 3 tracks on the strip
        int y = h - (hstrip - hstrip/16) + ((hstrip - hstrip/4)/3)*it->getTrack() + 30;

        //Display the name only if the setence is standalone
        if (it->isSimple()){
            int nameWidth = (it->getPeople().getName().length() + 1) * 10;
            _texts.push_back(new PhGraphicText(it->getPeople().getName(),
                                               (it->getTimeIn() - _controller->getDoc().getLastPosition()) * 20 - nameWidth - 10, y, -1,
                                               nameWidth, 30, _fonts.first(), it->getPeople().getColor()));
        }
        _texts.push_back(new PhGraphicText(it->getContent(),
                                           (it->getTimeIn() - _controller->getDoc().getLastPosition()) * 20, y , -1,
                                           (it->getTimeOut() - it->getTimeIn()) * 20, hstrip / 5 , _currentFont, it->getPeople().getColor()));
//        if (i % (max / 20) == 0){
//            QApplication::processEvents();
//        }
 //       i++;
    }

    //Set a default number of strip repetition
    int nbRythmo = this->width()/60;

    //Load the strip
#if defined(Q_OS_MAC)
        PhString filePath = "/../Resources/img/motif-240.png";
#elif defined(Q_OS_UNIX)
        PhString filePath = "/data/img/motif-240.png";
#endif
        _strips.push_back(new PhGraphicImage(QCoreApplication::applicationDirPath() + filePath,
                                           -1000, h - hstrip, -3,
                                           240, hstrip, PhColor("white"),
                                           nbRythmo, 1));

    //Load the cuts
    foreach(PhStripCut * it, _controller->getDoc().getCuts())
    {
        _cuts.push_back(new PhGraphicTexturedRect((it->getTimeIn() - _controller->getDoc().getLastPosition()) * 20, 0, -2,
                                                  2, hstrip,
                                                  PhColor("black")));
    }

}

void PhGraphicStripView::paintGL()
{

    //Time-based test
 //  qDebug() << _test->elapsed(); //<< " : " << _xmove;


    //qDebug() << "PaintGL";

    //Clear the buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Set the deplacement size of the strip
    if (_shouldmove){
        _xmove -= 8;
        _xMoveStrip -= 8;
        // if the strip moved of more than 1 X strip's width it came back
        if(_xMoveStrip <= -240 || _xMoveStrip >= 240)
            _xMoveStrip = 0;
    }

    //Draw Objects
    foreach(PhGraphicImage * it, _strips)
    {
        it->draw(_xMoveStrip);
    }
    foreach(PhGraphicText * it, _texts)
    {
        it->draw(_xmove);
    }
    foreach(PhGraphicTexturedRect * it, _cuts)
    {
        it->draw(_xmove);
    }
}


void PhGraphicStripView::stopScroll()
{
    qDebug() << "Stop Scrolling";
    setScroll(false);
}

void PhGraphicStripView::clearData()
{
    foreach(PhGraphicTexturedRect * it, _cuts)
    {
        delete it;
    }
    foreach(PhGraphicText * it, _texts)
    {
        delete it;
    }
    _cuts.clear();
    _texts.clear();
}


QList<PhFont *> PhGraphicStripView::getFonts()
{
    return _fonts;
}


void PhGraphicStripView::setCurrentFont(QString fontfile){
    _currentFont = new PhFont(fontfile, 150);
    initializeGL();
}


PhFont * PhGraphicStripView::getCurrentFont(){
    return _currentFont;
}


void PhGraphicStripView::changeScroll()
{
    _shouldmove = ! _shouldmove;
}


void PhGraphicStripView::setScroll(bool shouldScroll)
{
    if(shouldScroll)
        qDebug() << "The strip should scroll";
    else
        qDebug() << "The strip should NOT scroll";

    _shouldmove = shouldScroll;
}

void PhGraphicStripView::setNaturalScroll(bool naturalScroll)
{
    _naturalScroll = naturalScroll;
}

void PhGraphicStripView::toggleNaturalScrolling()
{
    _naturalScroll = ! _naturalScroll;
}

void PhGraphicStripView::setController(PhGraphicStripController * controller)
{
    _controller = controller;
}

void PhGraphicStripView::connectSlots(){
    connect(_controller, SIGNAL(docChanged()), this, SLOT(initializeGL()));
    connect(_controller, SIGNAL(docChanged()), this, SLOT(stopScroll()));
}

void PhGraphicStripView::setXmove(int n)
{
    if (_naturalScroll)
    {
        _xmove -= n;
        _xMoveStrip -= n;
        if(_xMoveStrip <= -240 || _xMoveStrip >= 240)
            _xMoveStrip = 0;
    }
    else
    {
        _xmove += n;
        _xMoveStrip += n;
        if(_xMoveStrip <= -240 || _xMoveStrip >= 240)
            _xMoveStrip = 0;
    }
}


PhStripDoc *PhGraphicStripView::getDoc()
{
    return _doc;
}