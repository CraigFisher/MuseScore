//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2002-2011 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#include "notationsetter.h"

//TODO: GUARANTEE THIS CLASS IS NEVER CALLED BEFORE A SCORE IS LOADED
//TODO: MOVE FUNCTIONALITY OF THIS CLASS TO A DIALOG
//TODO: INTRODUCE TRADITIONAL ACCIDENTAL BEHAVIOR (NOT JUST APPEARANCE) AS
//      AN ALTNOTAITON OPTION

//cc
namespace Ms {

//---------------------------------------------------------
//   NotationSetter
//---------------------------------------------------------

void NotationSetter::setNotation() {
	
	//TODO: GUARANTEE NO POSSIBLE RACE CONDITION FROM
	//		   NOTATION PREFERENCES BEING CHANGED TWICE
	//		   BEFORE LAYOUT() IS PERFORMED.
	
  if(preferences.altNotePositions) {
		setNotePositions();		
	}
	if(preferences.altNoteHeadGroups) {
		setNoteHeadGroups();
	}
	if(preferences.altNewAccidentals) {  //STUB
		setAccidentals();                   
	}
	if(preferences.altStaffLines) {
		setStaffLines();
	}
	if(preferences.altInnerLedgers) {
		setInnerLedgers();
	}
}

void NotationSetter::setNotePositions() {

          //------------------------TRADITIONAL
          // Note::altOctaveDistance = 7;
          // Note::altNotePositions[0] = 10;    //Cbb
          // Note::altNotePositions[7] = 10;    //Cb
          // Note::altNotePositions[14] = 10;    //C
          // Note::altNotePositions[21] = 10;   //C#
          // Note::altNotePositions[28] = 10;   //C##
          
          // Note::altNotePositions[2] = 9;    //Dbb
          // Note::altNotePositions[9] = 9;    //Db
          // Note::altNotePositions[16] = 9;   //D
          // Note::altNotePositions[23] = 9;   //D#
          // Note::altNotePositions[30] = 9;   //D##

          // Note::altNotePositions[4] = 8;    //Ebb
          // Note::altNotePositions[11] = 8;   //Eb
          // Note::altNotePositions[18] = 8;   //E
          // Note::altNotePositions[25] = 8;   //E#
          // Note::altNotePositions[32] = 8;   //E##

          // Note::altNotePositions[-1] = 7;   //Fbb
          // Note::altNotePositions[6] = 7;    //Fb
          // Note::altNotePositions[13] = 7;   //F
          // Note::altNotePositions[20] = 7;   //F#
          // Note::altNotePositions[27] = 7;   //F##

          // Note::altNotePositions[1] = 6;    //Gbb
          // Note::altNotePositions[8] = 6;    //Gb
          // Note::altNotePositions[15] = 6;   //G
          // Note::altNotePositions[22] = 6;   //G#
          // Note::altNotePositions[29] = 6;   //G##

          // Note::altNotePositions[3] = 5;    //Abb
          // Note::altNotePositions[10] = 5;   //Ab
          // Note::altNotePositions[17] = 5;   //A
          // Note::altNotePositions[24] = 5;   //A#
          // Note::altNotePositions[31] = 5;   //A##

          // Note::altNotePositions[5] = 4;    //Bbb
          // Note::altNotePositions[12] = 4;   //Bb
          // Note::altNotePositions[19] = 4;   //B
          // Note::altNotePositions[26] = 4;   //B#
          // Note::altNotePositions[33] = 4;   //B##

          //------------------------------------------
          Note::altOctaveDistance = 12;
          Note::altNotePositions[0] = 14;    //Cbb
          Note::altNotePositions[7] = 13;    //Cb
          Note::altNotePositions[14] = 12;    //C
          Note::altNotePositions[21] = 11;   //C#
          Note::altNotePositions[28] = 10;   //C##
          
          Note::altNotePositions[2] = 12;    //Dbb
          Note::altNotePositions[9] = 11;    //Db
          Note::altNotePositions[16] = 10;   //D
          Note::altNotePositions[23] = 9;   //D#
          Note::altNotePositions[30] = 8;   //D##

          Note::altNotePositions[4] = 10;    //Ebb
          Note::altNotePositions[11] = 9;   //Eb
          Note::altNotePositions[18] = 8;   //E
          Note::altNotePositions[25] = 7;   //E#
          Note::altNotePositions[32] = 6;   //E##

          Note::altNotePositions[-1] = 9;   //Fbb
          Note::altNotePositions[6] = 8;    //Fb
          Note::altNotePositions[13] = 7;   //F
          Note::altNotePositions[20] = 6;   //F#
          Note::altNotePositions[27] = 5;   //F##

          Note::altNotePositions[1] = 7;    //Gbb
          Note::altNotePositions[8] = 6;    //Gb
          Note::altNotePositions[15] = 5;   //G
          Note::altNotePositions[22] = 4;   //G#
          Note::altNotePositions[29] = 3;   //G##

          Note::altNotePositions[3] = 5;    //Abb
          Note::altNotePositions[10] = 4;   //Ab
          Note::altNotePositions[17] = 3;   //A
          Note::altNotePositions[24] = 2;   //A#
          Note::altNotePositions[31] = 1;   //A##

          Note::altNotePositions[5] = 3;    //Bbb
          Note::altNotePositions[12] = 2;   //Bb
          Note::altNotePositions[19] = 1;   //B
          Note::altNotePositions[26] = 0;   //B#
          Note::altNotePositions[33] = -1;   //B##

}

void NotationSetter::setNoteHeadGroups() {
            // HEAD_NORMAL = 0,
            // HEAD_CROSS,
            // HEAD_DIAMOND,
            // HEAD_TRIANGLE,
            // HEAD_MI,
            // HEAD_SLASH,
            // HEAD_XCIRCLE,
            // HEAD_DO,
            // HEAD_RE,
            // HEAD_FA,
            // HEAD_LA,
            // HEAD_TI,
            // HEAD_SOL,
            // HEAD_BREVIS_ALT,
            // HEAD_GROUPS,
            // HEAD_INVALID = -1


          // Note::altNoteHeadGroups[0] =  NoteHead::Group::HEAD_TRIANGLE;   //Cbb
          // Note::altNoteHeadGroups[7] =  NoteHead::Group::HEAD_TRIANGLE;   //Cb
          // Note::altNoteHeadGroups[14] = NoteHead::Group::HEAD_TRIANGLE; //C
          // Note::altNoteHeadGroups[21] = NoteHead::Group::HEAD_TRIANGLE;   //C#
          // Note::altNoteHeadGroups[28] = NoteHead::Group::HEAD_TRIANGLE;   //C##



          Note::altNoteHeadGroups[0] =  NoteHead::Group::HEAD_NORMAL;   //Cbb
          Note::altNoteHeadGroups[7] =  NoteHead::Group::HEAD_NORMAL;   //Cb
          Note::altNoteHeadGroups[14] = NoteHead::Group::HEAD_NORMAL; //C
          Note::altNoteHeadGroups[21] = NoteHead::Group::HEAD_NORMAL;   //C#
          Note::altNoteHeadGroups[28] = NoteHead::Group::HEAD_NORMAL;   //C##
          
          Note::altNoteHeadGroups[2] =  NoteHead::Group::HEAD_NORMAL;   //Dbb
          Note::altNoteHeadGroups[9] =  NoteHead::Group::HEAD_NORMAL;   //Db
          Note::altNoteHeadGroups[16] = NoteHead::Group::HEAD_NORMAL;   //D
          Note::altNoteHeadGroups[23] = NoteHead::Group::HEAD_NORMAL;   //D#
          Note::altNoteHeadGroups[30] = NoteHead::Group::HEAD_NORMAL;   //D##

          Note::altNoteHeadGroups[4] =  NoteHead::Group::HEAD_NORMAL;   //Ebb
          Note::altNoteHeadGroups[11] = NoteHead::Group::HEAD_NORMAL;   //Eb
          Note::altNoteHeadGroups[18] = NoteHead::Group::HEAD_NORMAL;   //E
          Note::altNoteHeadGroups[25] = NoteHead::Group::HEAD_NORMAL;   //E#
          Note::altNoteHeadGroups[32] = NoteHead::Group::HEAD_NORMAL;   //E##

          Note::altNoteHeadGroups[-1] = NoteHead::Group::HEAD_NORMAL;   //Fbb
          Note::altNoteHeadGroups[6] =  NoteHead::Group::HEAD_NORMAL;   //Fb
          Note::altNoteHeadGroups[13] = NoteHead::Group::HEAD_NORMAL;   //F
          Note::altNoteHeadGroups[20] = NoteHead::Group::HEAD_NORMAL;   //F#
          Note::altNoteHeadGroups[27] = NoteHead::Group::HEAD_NORMAL;   //F##

          Note::altNoteHeadGroups[1] =  NoteHead::Group::HEAD_NORMAL;   //Gbb
          Note::altNoteHeadGroups[8] =  NoteHead::Group::HEAD_NORMAL;   //Gb
          Note::altNoteHeadGroups[15] = NoteHead::Group::HEAD_NORMAL;   //G
          Note::altNoteHeadGroups[22] = NoteHead::Group::HEAD_NORMAL;   //G#
          Note::altNoteHeadGroups[29] = NoteHead::Group::HEAD_NORMAL;   //G##

          Note::altNoteHeadGroups[3] =  NoteHead::Group::HEAD_NORMAL;   //Abb
          Note::altNoteHeadGroups[10] = NoteHead::Group::HEAD_NORMAL;   //Ab
          Note::altNoteHeadGroups[17] = NoteHead::Group::HEAD_NORMAL;   //A
          Note::altNoteHeadGroups[24] = NoteHead::Group::HEAD_NORMAL;   //A#
          Note::altNoteHeadGroups[31] = NoteHead::Group::HEAD_NORMAL;   //A##

          Note::altNoteHeadGroups[5] = NoteHead::Group::HEAD_NORMAL;    //Bbb
          Note::altNoteHeadGroups[12] = NoteHead::Group::HEAD_NORMAL;   //Bb
          Note::altNoteHeadGroups[19] = NoteHead::Group::HEAD_NORMAL;   //B
          Note::altNoteHeadGroups[26] = NoteHead::Group::HEAD_NORMAL;   //B#
          Note::altNoteHeadGroups[33] = NoteHead::Group::HEAD_NORMAL;   //B##
}

void NotationSetter::setAccidentals() {
    //STUB: FUNCTIONALITY NOT YET DECIDED ON
    //      (currently, setting preferences.altNoAccidentals is enough to eliminate accidentals)
}

void NotationSetter::setStaffLines() {
    std::vector<bool>* visList = &StaffLines::altLineVisibility;
    visList->clear();

    //-----------------------TRADITIONAL
    // visList->push_back(true);
    // visList->push_back(false);
    // visList->push_back(true);
    // visList->push_back(false);
    // visList->push_back(true);
    // visList->push_back(false);
    // visList->push_back(true);
    // visList->push_back(false);
    // visList->push_back(true);


    //-----------------------
    // visList->push_back(true);
    // visList->push_back(true);
    // visList->push_back(true);
    // visList->push_back(true);
    // visList->push_back(true);
    // visList->push_back(true);
    // visList->push_back(true);
    // visList->push_back(true);
    // visList->push_back(true);


    //------------------------------------------------

    visList->push_back(true);
    visList->push_back(false);
    visList->push_back(true);
    visList->push_back(false);
    visList->push_back(true);

    visList->push_back(false);
    visList->push_back(false);
    visList->push_back(false);
    
    visList->push_back(true);
    visList->push_back(false);
    visList->push_back(true);
    visList->push_back(false);
    visList->push_back(true);


    //altStaffLinesHeight represents the height of the staff in units of traditional lines.
    //It converts the height of an alternative StaffLine system (which may use
    //use arbitrary spacing between lines) back to a height in traditional units
    //of lines and spacing.
    //    This value is passed to StaffLines.size.
    StaffLines::altStaffLinesHeight = (StaffLines::altLineVisibility.size() / 2) + 1; //round up
}

void NotationSetter::setInnerLedgers() {
    std::vector<int>* innerLedgers = &Chord::altInnerLedgers;
    innerLedgers->clear();

    innerLedgers->push_back(6);
}

}
