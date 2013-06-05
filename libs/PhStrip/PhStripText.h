#ifndef PHSTRIPTEXT_H
#define PHSTRIPTEXT_H

#include "PhTools/PhString.h"

#include "PhPeople.h"
#include "PhStrip/PhStripObject.h"

/**
 * Sentence or part of a sentence of a PhPeople with synchronization information.
 */
class PhStripText : public PhStripObject {

public:

    PhStripText(PhPeople * people, PhString content, PhTime timeIn, PhTime timeOut, int track, bool simpleText);
    PhString getContent();
    PhPeople getPeople();
    int getTrack();
    int getTimeIn();
    int getTimeOut();
    bool isSimple();

private:
/**
 * Indicate the end of the PhText. The text content is stretch between timeIn : PhTime and timeOut : PhTime
 */
    PhTime _timeOut;
/**
 * Text string of Sentence or part of a sentence.
 */
    PhString _content;

    /**
     * Reference to the PhPeople who is affected to the PhStripText
     */
    PhPeople *_people;

    int _track;

    bool _simpleText;
};

#endif // PHSTRIPTEXT_H
