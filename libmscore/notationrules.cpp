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

#include "notationrules.h"
#include "mscore/preferences.h"
#include "note.h"
#include "chord.h"
#include "clef.h"

//cc
namespace Ms {

//---------------------------------------------------------
//   NotationRules
//---------------------------------------------------------

std::map<int, int> NotationRules::_notePositions;
std::map<int, NoteHead::Group> NotationRules::_noteHeads;
std::map<ClefType, int> NotationRules::_clefOffsets;
int NotationRules::_octaveDistance;
std::vector<int> NotationRules::_innerLedgers;
std::vector<bool> NotationRules::_staffLines;
int NotationRules::_staffLinesHeight;

bool NotationRules::alternateNotePositions;
bool NotationRules::alternateNoteheads;
bool NotationRules::alternateCleffOffsets;
bool NotationRules::alternateStaffLines;
bool NotationRules::useInnerLedgers;
bool NotationRules::noAccidentals;

void NotationRules::load(QFile* f) {
      XmlReader e(f);

      alterNotePositions = false;
      alterNoteheads = false;
      alterstaffLines = false;
      useInnerLedgers = false;
      noAccidentals = false;

      while (e.readNextStartElement()) {
            if (e.name() == "Notation") {
                  while (e.readNextStartElement()) {
                        QString tag = e.name().toString();
                        if (tag == "NotePositions") {
                              readNotePositions(e);
                              }
                        else if (tag == "NoteHeads") {
                              readNoteHeads(e);                 
                              }
                        else if (tag == "StaffLines") {
                              readStaffLines(e);
                              }
                        else if (tag == "UseAccidentals") {
                              noAccidentals = e.readBool();
                              }

                        }   
                  }
            }

      if (!_notePositions.empty())
            alternateNotePositions = true;
      if (!_noteHeads.empty())
            alternateNoteheads = true;                              
      if (!_staffLines.empty())
            alternateStaffLines = true;
      if (!_innerLedgers.empty())
            useInnterLedgers = true;       	
      }

void NotationRules::readNotePositions(XmlReader& e) {
      while (e.readNextElement()) {
            QString tagOuter = e.name().toString();
            if (tagOuter == "OctaveDistance") {
                  _octaveDistance = e.readInt();
                  }
            else if (tagOuter == "Mappings") {
                  while (e.readNextElement()) {
                        QString tag = e.name().remove(0, 1); //removes initial '_' from tag
                        int noteNumber = tag.toInt();
                        notePositions[noteNumber] = e.readInt();
                        }
                  }
            else if (tagOuter == "CleffOffsets") {
                  while (e.readNextElement()) {
                        QString tag = e.name().toString();
                        ClefType ct;
                        if (tag == "G")
                              ct = ClefType::G;
                        else if (tag == "G1");
                              ct = ClefType::G1;
                        else if (tag == "G2");
                              ct = ClefType::G2;
                        else if (tag == "G3");
                              ct = ClefType::G3;
                        else if (tag == "F");
                              ct = ClefType::F;
                        else if (tag == "F8");
                              ct = ClefType::F8;
                        else if (tag == "F15");
                              ct = ClefType::F15;
                        else if (tag == "F_B");
                              ct = ClefType::F_B;
                        else if (tag == "F_C");
                              ct = ClefType::F_C;
                        else if (tag == "C1");
                              ct = ClefType::C1;
                        else if (tag == "C2");
                              ct = ClefType::C2;
                        else if (tag == "C3");
                              ct = ClefType::C3;
                        else if (tag == "C4");
                              ct = ClefType::C4;
                        else if (tag == "C5");
                              ct = ClefType::C5;
                        else if (tag == "G4");
                              ct = ClefType::G4;
                        else if (tag == "F_8VA");
                              ct = ClefType::F_8VA;
                        else if (tag == "F_15MA");
                              ct = ClefType::F_15MA;
                        else if (tag == "INVALID");
                              ct = ClefType::INVALID;

                        _clefOffsets[ct] = e.readInt();;
                        }
                  }
            }
      }

void NotationRules::readNoteHeads(XmlReader& e) {
      while (e.readNextElement()) {
            QString tag(e.name().remove(0, 1)); //removes initial '_' from tag
            int noteNumber = tag.toInt();
            QString head = readElementText();

            NoteHead::Group notehead;
            if (head == "NORMAL")
                  notehead = NoteHead::Group::HEAD_NORMAL;
            if (head == "CROSS") 
                  notehead = NoteHead::Group::HEAD_CROSS;
            if (head == "DIAMOND") 
                  notehead = NoteHead::Group::HEAD_DIAMOND;
            if (head == "TRIANGLE") 
                  notehead = NoteHead::Group::HEAD_TRIANGLE;
            if (head == "MI") 
                  notehead = NoteHead::Group::HEAD_MI;
            if (head == "SLASH") 
                  notehead = NoteHead::Group::HEAD_SLASH;
            if (head == "XCIRCLE") 
                  notehead = NoteHead::Group::HEAD_XCIRCLE;
            if (head == "DO") 
                  notehead = NoteHead::Group::HEAD_DO;
            if (head == "RE") 
                  notehead = NoteHead::Group::HEAD_RE;
            if (head == "FA") 
                  notehead = NoteHead::Group::HEAD_FA;
            if (head == "LA") 
                  notehead = NoteHead::Group::HEAD_LA;
            if (head == "TI") 
                  notehead = NoteHead::Group::HEAD_TI;
            if (head == "SOL") 
                  notehead = NoteHead::Group::HEAD_SOL;
            if (head == "BREVIS_ALT") 
                  notehead = NoteHead::Group::HEAD_BREVIS_ALT;
            if (head == "GROUPS") 
                  notehead =  NoteHead::Group::HEAD_GROUPS;
            if (head == "INVALID") 
                  notehead = NoteHead::Group::HEAD_INVALID;

            _noteHeads[noteNumber] = noteHead;
            }
      }

void NotationRules::readStaffLines(XmlReader& e) {
      _staffLines->clear();
      _innerLedgers->clear();
      QString lineType;

      int lineIndex = 0;
      while (e.readNextElement()) {
            lineType = e.name().toString();
            if (lineType == "solid")
                  _staffLines->push_back(true);      
            else if (lineType == "hidden")
                  _staffLines->push_back(false);
            else if (lineType == "ledger")
                  _innerLedgers->push_back(lineIndex);
            lineIndex++;
            }

      //altStaffLinesHeight represents the height of the staff in units of traditional lines.
      //It converts the height of an alternative StaffLine system (which may use
      //use arbitrary spacing between lines) back to a height in traditional units
      //of lines and spacing.
      //    This value is passed to StaffLines.size.
      _staffLinesHeight = (staffLines.size() / 2) + 1; //round up
      }

// void NotationRules::setNotePositions() {

//           //------------------------TRADITIONAL
//           // Note::altOctaveDistance = 7;
//           // Note::altNotePositions[0] = 10;    //Cbb
//           // Note::altNotePositions[7] = 10;    //Cb
//           // Note::altNotePositions[14] = 10;    //C
//           // Note::altNotePositions[21] = 10;   //C#
//           // Note::altNotePositions[28] = 10;   //C##
          
//           // Note::altNotePositions[2] = 9;    //Dbb
//           // Note::altNotePositions[9] = 9;    //Db
//           // Note::altNotePositions[16] = 9;   //D
//           // Note::altNotePositions[23] = 9;   //D#
//           // Note::altNotePositions[30] = 9;   //D##

//           // Note::altNotePositions[4] = 8;    //Ebb
//           // Note::altNotePositions[11] = 8;   //Eb
//           // Note::altNotePositions[18] = 8;   //E
//           // Note::altNotePositions[25] = 8;   //E#
//           // Note::altNotePositions[32] = 8;   //E##

//           // Note::altNotePositions[-1] = 7;   //Fbb
//           // Note::altNotePositions[6] = 7;    //Fb
//           // Note::altNotePositions[13] = 7;   //F
//           // Note::altNotePositions[20] = 7;   //F#
//           // Note::altNotePositions[27] = 7;   //F##

//           // Note::altNotePositions[1] = 6;    //Gbb
//           // Note::altNotePositions[8] = 6;    //Gb
//           // Note::altNotePositions[15] = 6;   //G
//           // Note::altNotePositions[22] = 6;   //G#
//           // Note::altNotePositions[29] = 6;   //G##

//           // Note::altNotePositions[3] = 5;    //Abb
//           // Note::altNotePositions[10] = 5;   //Ab
//           // Note::altNotePositions[17] = 5;   //A
//           // Note::altNotePositions[24] = 5;   //A#
//           // Note::altNotePositions[31] = 5;   //A##

//           // Note::altNotePositions[5] = 4;    //Bbb
//           // Note::altNotePositions[12] = 4;   //Bb
//           // Note::altNotePositions[19] = 4;   //B
//           // Note::altNotePositions[26] = 4;   //B#
//           // Note::altNotePositions[33] = 4;   //B##

//           //------------------------------------------
//           Note::altOctaveDistance = 12;
//           Note::altNotePositions[0] = 14;    //Cbb
//           Note::altNotePositions[7] = 13;    //Cb
//           Note::altNotePositions[14] = 12;    //C
//           Note::altNotePositions[21] = 11;   //C#
//           Note::altNotePositions[28] = 10;   //C##
          
//           Note::altNotePositions[2] = 12;    //Dbb
//           Note::altNotePositions[9] = 11;    //Db
//           Note::altNotePositions[16] = 10;   //D
//           Note::altNotePositions[23] = 9;   //D#
//           Note::altNotePositions[30] = 8;   //D##

//           Note::altNotePositions[4] = 10;    //Ebb
//           Note::altNotePositions[11] = 9;   //Eb
//           Note::altNotePositions[18] = 8;   //E
//           Note::altNotePositions[25] = 7;   //E#
//           Note::altNotePositions[32] = 6;   //E##

//           Note::altNotePositions[-1] = 9;   //Fbb
//           Note::altNotePositions[6] = 8;    //Fb
//           Note::altNotePositions[13] = 7;   //F
//           Note::altNotePositions[20] = 6;   //F#
//           Note::altNotePositions[27] = 5;   //F##

//           Note::altNotePositions[1] = 7;    //Gbb
//           Note::altNotePositions[8] = 6;    //Gb
//           Note::altNotePositions[15] = 5;   //G
//           Note::altNotePositions[22] = 4;   //G#
//           Note::altNotePositions[29] = 3;   //G##

//           Note::altNotePositions[3] = 5;    //Abb
//           Note::altNotePositions[10] = 4;   //Ab
//           Note::altNotePositions[17] = 3;   //A
//           Note::altNotePositions[24] = 2;   //A#
//           Note::altNotePositions[31] = 1;   //A##

//           Note::altNotePositions[5] = 3;    //Bbb
//           Note::altNotePositions[12] = 2;   //Bb
//           Note::altNotePositions[19] = 1;   //B
//           Note::altNotePositions[26] = 0;   //B#
//           Note::altNotePositions[33] = -1;   //B##

//           //------------------------------
//           // Note::altOctaveDistance = 7;
//           // Note::altNotePositions[0] = 4;    //Cbb
//           // Note::altNotePositions[7] = 4;    //Cb
//           // Note::altNotePositions[14] = 10;    //C
//           // Note::altNotePositions[21] = 10;   //C#
//           // Note::altNotePositions[28] = 9;   //C##
          
//           // Note::altNotePositions[2] = 10;    //Dbb
//           // Note::altNotePositions[9] = 9;    //Db
//           // Note::altNotePositions[16] = 9;   //D
//           // Note::altNotePositions[23] = 9;   //D#
//           // Note::altNotePositions[30] = 8;   //D##

//           // Note::altNotePositions[4] = 9;    //Ebb
//           // Note::altNotePositions[11] = 8;   //Eb
//           // Note::altNotePositions[18] = 8;   //E
//           // Note::altNotePositions[25] = 7;   //E#
//           // Note::altNotePositions[32] = 7;   //E##

//           // Note::altNotePositions[-1] = 8;   //Fbb
//           // Note::altNotePositions[6] = 8;    //Fb
//           // Note::altNotePositions[13] = 7;   //F
//           // Note::altNotePositions[20] = 7;   //F#
//           // Note::altNotePositions[27] = 6;   //F##

//           // Note::altNotePositions[1] = 7;    //Gbb
//           // Note::altNotePositions[8] = 6;    //Gb
//           // Note::altNotePositions[15] = 6;   //G
//           // Note::altNotePositions[22] = 6;   //G#
//           // Note::altNotePositions[29] = 5;   //G##

//           // Note::altNotePositions[3] = 6;    //Abb
//           // Note::altNotePositions[10] = 5;   //Ab
//           // Note::altNotePositions[17] = 5;   //A
//           // Note::altNotePositions[24] = 5;   //A#
//           // Note::altNotePositions[31] = 4;   //A##

//           // Note::altNotePositions[5] = 5;    //Bbb
//           // Note::altNotePositions[12] = 4;   //Bb
//           // Note::altNotePositions[19] = 4;   //B
//           // Note::altNotePositions[26] = 10;   //B#
//           // Note::altNotePositions[33] = 10;   //B##

//           //initialize ClefOffsets

//           //traditional
//           Note::altClefOffsets[ClefType::INVALID] = 0;
//           Note::altClefOffsets[ClefType::G] = 0;
//           Note::altClefOffsets[ClefType::G1] = 7;
//           Note::altClefOffsets[ClefType::G2] = 14;
//           Note::altClefOffsets[ClefType::G3] = -7;
//           Note::altClefOffsets[ClefType::F] = -12;
//           Note::altClefOffsets[ClefType::F8] = -19;
//           Note::altClefOffsets[ClefType::F15] = -26;
//           Note::altClefOffsets[ClefType::F_B] = -10;
//           Note::altClefOffsets[ClefType::F_C] = -14;
//           Note::altClefOffsets[ClefType::C1] = -2;
//           Note::altClefOffsets[ClefType::C2] = -4;
//           Note::altClefOffsets[ClefType::C3] = -6;
//           Note::altClefOffsets[ClefType::C4] = -8;
//           Note::altClefOffsets[ClefType::C5] = -10;
//           Note::altClefOffsets[ClefType::G4] = 2;     //probably incorrect.  cleftype and clefinfo appear out of sync
//           Note::altClefOffsets[ClefType::F_8VA] = -5;
//           Note::altClefOffsets[ClefType::F_15MA] = 2;          
//           // Note::altClefOffsets[ClefType::PERC2] = 0; //these staffs should never be called
//           // Note::altClefOffsets[ClefType::TAB2] = 0;
//           // Note::altClefOffsets[ClefType::MAX] = 0;

//           // //initialize ClefOffsets
//           // Note::altClefOffsets[ClefType::INVALID] = 0;
//           // Note::altClefOffsets[ClefType::G] = 0;
//           // Note::altClefOffsets[ClefType::G1] = 0;
//           // Note::altClefOffsets[ClefType::G2] = 0;
//           // Note::altClefOffsets[ClefType::G3] = 0;
//           // Note::altClefOffsets[ClefType::F] = -12;
//           // Note::altClefOffsets[ClefType::F8] = 0;
//           // Note::altClefOffsets[ClefType::F15] = 0;
//           // Note::altClefOffsets[ClefType::F_B] = 0;
//           // Note::altClefOffsets[ClefType::F_C] = 0;
//           // Note::altClefOffsets[ClefType::C1] = 0;
//           // Note::altClefOffsets[ClefType::C2] = 0;
//           // Note::altClefOffsets[ClefType::C3] = 0;
//           // Note::altClefOffsets[ClefType::C4] = 0;
//           // Note::altClefOffsets[ClefType::TAB] = 0;
//           // Note::altClefOffsets[ClefType::PERC] = 0;
//           // Note::altClefOffsets[ClefType::C5] = 0;
//           // Note::altClefOffsets[ClefType::G4] = 0;
//           // Note::altClefOffsets[ClefType::F_8VA] = 0;
//           // Note::altClefOffsets[ClefType::F_15MA] = 0;
//           // Note::altClefOffsets[ClefType::PERC2] = 0;
//           // Note::altClefOffsets[ClefType::TAB2] = 0;
//           // Note::altClefOffsets[ClefType::MAX] = 0;

// }

// void NotationRules::setNoteHeadGroups() {
//             // HEAD_NORMAL = 0,
//             // HEAD_CROSS,
//             // HEAD_DIAMOND,
//             // HEAD_TRIANGLE,
//             // HEAD_MI,
//             // HEAD_SLASH,
//             // HEAD_XCIRCLE,
//             // HEAD_DO,
//             // HEAD_RE,
//             // HEAD_FA,
//             // HEAD_LA,
//             // HEAD_TI,
//             // HEAD_SOL,
//             // HEAD_BREVIS_ALT,
//             // HEAD_GROUPS,
//             // HEAD_INVALID = -1

//           //traditional
//           // Note::altNoteHeadGroups[0] =  NoteHead::Group::HEAD_NORMAL;   //Cbb
//           // Note::altNoteHeadGroups[7] =  NoteHead::Group::HEAD_NORMAL;   //Cb
//           // Note::altNoteHeadGroups[14] = NoteHead::Group::HEAD_NORMAL; //C
//           // Note::altNoteHeadGroups[21] = NoteHead::Group::HEAD_NORMAL;   //C#
//           // Note::altNoteHeadGroups[28] = NoteHead::Group::HEAD_NORMAL;   //C##
          
//           // Note::altNoteHeadGroups[2] =  NoteHead::Group::HEAD_NORMAL;   //Dbb
//           // Note::altNoteHeadGroups[9] =  NoteHead::Group::HEAD_NORMAL;   //Db
//           // Note::altNoteHeadGroups[16] = NoteHead::Group::HEAD_NORMAL;   //D
//           // Note::altNoteHeadGroups[23] = NoteHead::Group::HEAD_NORMAL;   //D#
//           // Note::altNoteHeadGroups[30] = NoteHead::Group::HEAD_NORMAL;   //D##

//           // Note::altNoteHeadGroups[4] =  NoteHead::Group::HEAD_NORMAL;   //Ebb
//           // Note::altNoteHeadGroups[11] = NoteHead::Group::HEAD_NORMAL;   //Eb
//           // Note::altNoteHeadGroups[18] = NoteHead::Group::HEAD_NORMAL;   //E
//           // Note::altNoteHeadGroups[25] = NoteHead::Group::HEAD_NORMAL;   //E#
//           // Note::altNoteHeadGroups[32] = NoteHead::Group::HEAD_NORMAL;   //E##

//           // Note::altNoteHeadGroups[-1] = NoteHead::Group::HEAD_NORMAL;   //Fbb
//           // Note::altNoteHeadGroups[6] =  NoteHead::Group::HEAD_NORMAL;   //Fb
//           // Note::altNoteHeadGroups[13] = NoteHead::Group::HEAD_NORMAL;   //F
//           // Note::altNoteHeadGroups[20] = NoteHead::Group::HEAD_NORMAL;   //F#
//           // Note::altNoteHeadGroups[27] = NoteHead::Group::HEAD_NORMAL;   //F##

//           // Note::altNoteHeadGroups[1] =  NoteHead::Group::HEAD_NORMAL;   //Gbb
//           // Note::altNoteHeadGroups[8] =  NoteHead::Group::HEAD_NORMAL;   //Gb
//           // Note::altNoteHeadGroups[15] = NoteHead::Group::HEAD_NORMAL;   //G
//           // Note::altNoteHeadGroups[22] = NoteHead::Group::HEAD_NORMAL;   //G#
//           // Note::altNoteHeadGroups[29] = NoteHead::Group::HEAD_NORMAL;   //G##

//           // Note::altNoteHeadGroups[3] =  NoteHead::Group::HEAD_NORMAL;   //Abb
//           // Note::altNoteHeadGroups[10] = NoteHead::Group::HEAD_NORMAL;   //Ab
//           // Note::altNoteHeadGroups[17] = NoteHead::Group::HEAD_NORMAL;   //A
//           // Note::altNoteHeadGroups[24] = NoteHead::Group::HEAD_NORMAL;   //A#
//           // Note::altNoteHeadGroups[31] = NoteHead::Group::HEAD_NORMAL;   //A##

//           // Note::altNoteHeadGroups[5] = NoteHead::Group::HEAD_NORMAL;    //Bbb
//           // Note::altNoteHeadGroups[12] = NoteHead::Group::HEAD_NORMAL;   //Bb
//           // Note::altNoteHeadGroups[19] = NoteHead::Group::HEAD_NORMAL;   //B
//           // Note::altNoteHeadGroups[26] = NoteHead::Group::HEAD_NORMAL;   //B#
//           // Note::altNoteHeadGroups[33] = NoteHead::Group::HEAD_NORMAL;   //B##

//             // HEAD_NORMAL = 0,
//             // HEAD_CROSS,
//             // HEAD_DIAMOND,
//             // HEAD_TRIANGLE,
//             // HEAD_MI,
//             // HEAD_SLASH,
//             // HEAD_XCIRCLE,
//             // HEAD_DO,
//             // HEAD_RE,
//             // HEAD_FA,
//             // HEAD_LA,
//             // HEAD_TI,
//             // HEAD_SOL,
//             // HEAD_BREVIS_ALT,
//             // HEAD_GROUPS,
//             // HEAD_INVALID = -1

//           //-----------------------

// NoteHead::Group SHARP = NoteHead::Group::HEAD_TRIANGLE;
// NoteHead::Group FLAT = NoteHead::Group::HEAD_CROSS;

//           Note::altNoteHeadGroups[0] =  FLAT;   //Cbb
//           Note::altNoteHeadGroups[7] =  NoteHead::Group::HEAD_NORMAL;   //Cb
//           Note::altNoteHeadGroups[14] = NoteHead::Group::HEAD_NORMAL; //C
//           Note::altNoteHeadGroups[21] = SHARP;   //C#
//           Note::altNoteHeadGroups[28] = NoteHead::Group::HEAD_NORMAL;   //C##
          
//           Note::altNoteHeadGroups[2] =  NoteHead::Group::HEAD_NORMAL;   //Dbb
//           Note::altNoteHeadGroups[9] =  FLAT;   //Db
//           Note::altNoteHeadGroups[16] = NoteHead::Group::HEAD_NORMAL;   //D
//           Note::altNoteHeadGroups[23] = SHARP;   //D#
//           Note::altNoteHeadGroups[30] = NoteHead::Group::HEAD_NORMAL;   //D##

//           Note::altNoteHeadGroups[4] =  NoteHead::Group::HEAD_NORMAL;   //Ebb
//           Note::altNoteHeadGroups[11] = FLAT;   //Eb
//           Note::altNoteHeadGroups[18] = NoteHead::Group::HEAD_NORMAL;   //E
//           Note::altNoteHeadGroups[25] = NoteHead::Group::HEAD_NORMAL;   //E#
//           Note::altNoteHeadGroups[32] = SHARP;   //E##

//           Note::altNoteHeadGroups[-1] = FLAT;   //Fbb
//           Note::altNoteHeadGroups[6] =  NoteHead::Group::HEAD_NORMAL;   //Fb
//           Note::altNoteHeadGroups[13] = NoteHead::Group::HEAD_NORMAL;   //F
//           Note::altNoteHeadGroups[20] = SHARP;   //F#
//           Note::altNoteHeadGroups[27] = NoteHead::Group::HEAD_NORMAL;   //F##

//           Note::altNoteHeadGroups[1] =  NoteHead::Group::HEAD_NORMAL;   //Gbb
//           Note::altNoteHeadGroups[8] =  FLAT;   //Gb
//           Note::altNoteHeadGroups[15] = NoteHead::Group::HEAD_NORMAL;   //G
//           Note::altNoteHeadGroups[22] = SHARP;   //G#
//           Note::altNoteHeadGroups[29] = NoteHead::Group::HEAD_NORMAL;   //G##

//           Note::altNoteHeadGroups[3] =  NoteHead::Group::HEAD_NORMAL;   //Abb
//           Note::altNoteHeadGroups[10] = FLAT;   //Ab
//           Note::altNoteHeadGroups[17] = NoteHead::Group::HEAD_NORMAL;   //A
//           Note::altNoteHeadGroups[24] = SHARP;   //A#
//           Note::altNoteHeadGroups[31] = NoteHead::Group::HEAD_NORMAL;   //A##

//           Note::altNoteHeadGroups[5] = NoteHead::Group::HEAD_NORMAL;    //Bbb
//           Note::altNoteHeadGroups[12] = FLAT;   //Bb
//           Note::altNoteHeadGroups[19] = NoteHead::Group::HEAD_NORMAL;   //B
//           Note::altNoteHeadGroups[26] = NoteHead::Group::HEAD_NORMAL;   //B#
//           Note::altNoteHeadGroups[33] = SHARP;   //B##
// }

// void NotationRules::setAccidentals() {
//     //STUB: FUNCTIONALITY NOT YET DECIDED ON
//     //      (currently, setting preferences.altNoAccidentals will eliminate accidentals)
// }

// void NotationRules::setStaffLines() {
//     std::vector<bool>* visList = &StaffLines::altLineVisibility;
//     visList->clear();

//     //-----------------------TRADITIONAL
//     // visList->push_back(true);
//     // visList->push_back(false);
//     // visList->push_back(true);
//     // visList->push_back(false);
//     // visList->push_back(true);
//     // visList->push_back(false);
//     // visList->push_back(true);
//     // visList->push_back(false);
//     // visList->push_back(true);


//     //-----------------------
//     // visList->push_back(true);
//     // visList->push_back(true);
//     // visList->push_back(true);
//     // visList->push_back(true);
//     // visList->push_back(true);
//     // visList->push_back(true);
//     // visList->push_back(true);
//     // visList->push_back(true);
//     // visList->push_back(true);


//     //-----------------------
//     visList->push_back(true);
//     visList->push_back(false);
//     visList->push_back(true);
//     visList->push_back(false);
//     visList->push_back(true);

//     visList->push_back(false);
//     visList->push_back(false);
//     visList->push_back(false);
    
//     visList->push_back(true);
//     visList->push_back(false);
//     visList->push_back(true);
//     visList->push_back(false);
//     visList->push_back(true);


//     //altStaffLinesHeight represents the height of the staff in units of traditional lines.
//     //It converts the height of an alternative StaffLine system (which may use
//     //use arbitrary spacing between lines) back to a height in traditional units
//     //of lines and spacing.
//     //    This value is passed to StaffLines.size.
//     StaffLines::altStaffLinesHeight = (StaffLines::altLineVisibility.size() / 2) + 1; //round up
// }

// void NotationRules::setInnerLedgers() {
//     std::vector<int>* innerLedgers = &Chord::altInnerLedgers;
//     innerLedgers->clear();

//     //traditional
//     innerLedgers->push_back(6);

//     //-------------------
//     // innerLedgers->push_back(6);
// }

// }
