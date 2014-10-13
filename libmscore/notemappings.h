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
 #ifndef __NOTEMAPPINGS_H__
 #define __NOTEMAPPINGS_H__

#include "note.h"
#include <map>
#include <vector>
#include <QFile>

/**
 \file
 Definition of class NoteMappings
*/

namespace Ms {

class Xml;
class XmlReader;

class NoteMappings {      
    private:
        std::map<int, int> _notePositions;
        std::map<int, NoteHead::Group> _noteHeads;
        std::map<ClefType, int> _clefOffsets;
        int _octaveDistance = 7;
        bool _noAccidentals = false;
    
        void writeMappings(Xml&) const;
        void readMappings(XmlReader& e);
        void writeClefOffsets(Xml&) const;
        void readClefOffsets(XmlReader& e);

    public:
        NoteMappings() {}
        NoteMappings(QFile* f);
        void write(Xml&) const;
        void read(XmlReader&);

        const std::map<int, int>* notePositions() { return &_notePositions; }
        const std::map<int, NoteHead::Group>* noteHeads() { return &_noteHeads; }
        const std::map<ClefType, int>* clefOffsets() { return &_clefOffsets; }
        int octaveDistance() { return _octaveDistance; }
      
      friend class MuseScore;
      };
}
#endif
