//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2004-2011 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================
//cc
 #ifndef __NOTATIONRULES_H__
 #define __NOTATIONRULES_H__

#include "xml.h"
#include "note.h"
#include "clef.h"
#include <map>
#include <vector>
#include <QFile>

/**
 \file
 Definition of class NotationRules
*/

namespace Ms {

class NotationRules {      
    private:
        NotationRules() {};

        static void readNotePositions(XmlReader& e); 
        static void readNoteHeads(XmlReader& e);
        static void readStaffLines(XmlReader& e);

        static std::map<int, int> _notePositions;
        static std::map<int, NoteHead::Group> _noteHeads;
        static std::map<ClefType, int> _clefOffsets;
        static int _octaveDistance;
        static std::vector<int> _innerLedgers;
        static std::vector<bool> _staffLines;
        static int _staffLinesHeight;

    public:
        static bool load(QFile* f);
        static void reset();

        static const std::map<int, int>* notePositions() { return &_notePositions; }
        static const std::map<int, NoteHead::Group>* noteHeads() { return &_noteHeads; }
        static const std::map<ClefType, int>* clefOffsets() { return &_clefOffsets; }
        static const std::vector<int>* innerLedgers() { return &_innerLedgers; }
        static const std::vector<bool>* staffLines() { return &_staffLines; }
        static const int octaveDistance() { return _octaveDistance; }
        static const int staffLinesHeight() { return _staffLinesHeight; }

        static bool alternateNotePositions; //TODO: make these consts as well?
        static bool alternateNoteheads;
        static bool alternateStaffLines;
        static bool useInnerLedgers;
        static bool noAccidentals;
      };
}
#endif
