/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPOFF_H
#define PHSTRIPOFF_H

#include "PhPeople.h"
#include "PhStrip/PhStripPeopleObject.h"

/**
 * Sentence or part of a sentence of a PhPeople with synchronization information.
 */
class PhStripOff : public PhStripPeopleObject {

public:
	/*!
	 * \brief PhStripOff constructor
	 * \param timeIn the beggining of the PhStripOff
	 * \param people the corresponding PhPeople
	 * \param timeOut the end of the PhStripOff
	 * \param track the track of the PhStripOff
	 */
	PhStripOff( PhTime timeIn, PhPeople * people, PhTime timeOut, int track);

};

#endif // PHSTRIPOFF_H
