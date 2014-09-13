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
bool NotationRules::alternateStaffLines;
bool NotationRules::useInnerLedgers;
bool NotationRules::noAccidentals;

bool NotationRules::load(QFile* f) {
      XmlReader e(f);
      reset();

      while (e.readNextStartElement()) {
            if (e.name().toString() == "Notation") {
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
                        else if (tag == "NoAccidentals") {
                              noAccidentals = e.readBool();
                              }
                        else
                              return false;
                        }
                  }
            else {
                  return false;
                  }
            }

      if (!_notePositions.empty() && !_clefOffsets.empty() && (_octaveDistance != -1))
            alternateNotePositions = true;
      if (!_noteHeads.empty())
            alternateNoteheads = true;                              
      if (!_staffLines.empty() && (_staffLinesHeight != -1))
            alternateStaffLines = true;
      if (!_innerLedgers.empty())
            useInnerLedgers = true;
    
      return true;
      }

void NotationRules::reset() {
      _notePositions.clear();
      _noteHeads.clear();
      _clefOffsets.clear();
      _innerLedgers.clear();
      _staffLines.clear();
      _staffLinesHeight = -1;
      _octaveDistance = -1;
    
      alternateNotePositions = false;
      alternateNoteheads = false;
      alternateStaffLines = false;
      useInnerLedgers = false;
      noAccidentals = false;
      }

void NotationRules::readNotePositions(XmlReader& e) {
      while (e.readNextStartElement()) {
            QString tagOuter = e.name().toString();
            if (tagOuter == "OctaveDistance") {
                  _octaveDistance = e.readInt();
                  }
            else if (tagOuter == "Mappings") {
                  while (e.readNextStartElement()) {
                        QString tag = e.name().toString();
                        tag = tag.remove(0, 1);            //removes initial '_' from tag
                        int noteNumber = tag.toInt();
                        _notePositions[noteNumber] = e.readInt();
                        }
                  }
            else if (tagOuter == "ClefOffsets") {
                  while (e.readNextStartElement()) {
                        QString tag = e.name().toString();
                        ClefType ct;
                        if (tag == "G")
                              ct = ClefType::G;
                        else if (tag == "G1")
                              ct = ClefType::G1;
                        else if (tag == "G2")
                              ct = ClefType::G2;
                        else if (tag == "G3")
                              ct = ClefType::G3;
                        else if (tag == "F")
                              ct = ClefType::F;
                        else if (tag == "F8")
                              ct = ClefType::F8;
                        else if (tag == "F15")
                              ct = ClefType::F15;
                        else if (tag == "F_B")
                              ct = ClefType::F_B;
                        else if (tag == "F_C")
                              ct = ClefType::F_C;
                        else if (tag == "C1")
                              ct = ClefType::C1;
                        else if (tag == "C2")
                              ct = ClefType::C2;
                        else if (tag == "C3")
                              ct = ClefType::C3;
                        else if (tag == "C4")
                              ct = ClefType::C4;
                        else if (tag == "C5")
                              ct = ClefType::C5;
                        else if (tag == "G4")
                              ct = ClefType::G4;
                        else if (tag == "F_8VA")
                              ct = ClefType::F_8VA;
                        else if (tag == "F_15MA")
                              ct = ClefType::F_15MA;
                        else if (tag == "INVALID")
                              ct = ClefType::INVALID;

                        _clefOffsets[ct] = e.readInt();;
                        }
                  }
            }
      }

void NotationRules::readNoteHeads(XmlReader& e) {
      while (e.readNextStartElement()) {
            QString tag(e.name().toString().remove(0, 1)); //removes initial '_' from tag
            int noteNumber = tag.toInt();
            QString head = e.readElementText();

            NoteHead::Group noteHead;
            if (head == "NORMAL")
                  noteHead = NoteHead::Group::HEAD_NORMAL;
            if (head == "CROSS") 
                  noteHead = NoteHead::Group::HEAD_CROSS;
            if (head == "DIAMOND") 
                  noteHead = NoteHead::Group::HEAD_DIAMOND;
            if (head == "TRIANGLE") 
                  noteHead = NoteHead::Group::HEAD_TRIANGLE;
            if (head == "MI") 
                  noteHead = NoteHead::Group::HEAD_MI;
            if (head == "SLASH") 
                  noteHead = NoteHead::Group::HEAD_SLASH;
            if (head == "XCIRCLE") 
                  noteHead = NoteHead::Group::HEAD_XCIRCLE;
            if (head == "DO") 
                  noteHead = NoteHead::Group::HEAD_DO;
            if (head == "RE") 
                  noteHead = NoteHead::Group::HEAD_RE;
            if (head == "FA") 
                  noteHead = NoteHead::Group::HEAD_FA;
            if (head == "LA")
                  noteHead = NoteHead::Group::HEAD_LA;
            if (head == "TI") 
                  noteHead = NoteHead::Group::HEAD_TI;
            if (head == "SOL") 
                  noteHead = NoteHead::Group::HEAD_SOL;
            if (head == "BREVIS_ALT") 
                  noteHead = NoteHead::Group::HEAD_BREVIS_ALT;
            if (head == "GROUPS")
                  noteHead =  NoteHead::Group::HEAD_GROUPS;
            if (head == "INVALID") 
                  noteHead = NoteHead::Group::HEAD_INVALID;

            _noteHeads[noteNumber] = noteHead;
            }
      }

void NotationRules::readStaffLines(XmlReader& e) {
      QString lineType;
    
      int lineIndex = 0;
      while (e.readNextStartElement()) {
            lineType = e.readElementText();
            if (lineType == "solid")
                  _staffLines.push_back(true);
            else if (lineType == "hidden")
                  _staffLines.push_back(false);
            else if (lineType == "ledger")
                  _innerLedgers.push_back(lineIndex);
            lineIndex++;
            }

      //altStaffLinesHeight represents the height of the staff in units of traditional lines.
      //It converts the height of an alternative StaffLine system (which may use
      //use arbitrary spacing between lines) back to a height in traditional units
      //of lines and spacing.
      //    This value is passed to StaffLines.size.
      _staffLinesHeight = (_staffLines.size() / 2) + 1; //round up
      }
    
}