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
        void readNotePositions(XmlReader& e);
        void readNoteHeads(XmlReader& e);
        void readInnerLedgers(XmlReader& e);
        void readStaffLines(XmlReader& e);
//        void reset();

        std::map<int, int> _notePositions;
        std::map<int, NoteHead::Group> _noteHeads;
        std::map<ClefType, int> _clefOffsets;
        std::map<int, std::vector<int>*> _innerLedgers;
        std::vector<bool> _staffLines;
        int _octaveDistance;
        int _staffLinesHeight;
        bool _noAccidentals;
      
        static const int NOTATION_FILE_VERSION = 2;

    public:
        NotationRules(QFile* f);
        ~NotationRules();

        const std::map<int, int>* notePositions() { return &_notePositions; }
        const std::map<int, NoteHead::Group>* noteHeads() { return &_noteHeads; }
        const std::map<ClefType, int>* clefOffsets() { return &_clefOffsets; }
        const std::map<int, std::vector<int>*>* innerLedgers() { return &_innerLedgers; }
        const std::vector<bool>* staffLines() { return &_staffLines; }
        int octaveDistance() { return _octaveDistance; }
        int staffLinesHeight() { return _staffLinesHeight; }
        bool noAccidentals() { return _noAccidentals; }
      
//        bool alternateNotePositions; //TODO: make these consts as well?
//        bool alternateNoteheads;
//        bool alternateStaffLines;
//        bool useInnerLedgers;
      
      friend class MuseScore;
      };
}
#endif
