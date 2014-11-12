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
        int _notePositions[35];
        NoteHead::Group _noteHeads[35];
        std::map<ClefType, int> _clefOffsets;
        int _octaveDistance = 7;
        bool _showAccidentals = true;
    
        static const NoteHead::Group defaultHg = NoteHead::Group::HEAD_NORMAL;
        void setTraditionalClefOffsets();

        void writeMappings(Xml&) const;
        void readMappings(XmlReader& e);
        void writeClefOffsets(Xml&) const;
        void readClefOffsets(XmlReader& e);

    public:
        NoteMappings();
        explicit NoteMappings(QFile*);
        bool operator==(const NoteMappings& st) const;
      
        void write(Xml&) const;
        void read(XmlReader&);
      
        void setNotePosition(int tpc, int pos)                { _notePositions[tpc + 1] = pos;   }
        void setNoteHeadGroup(int tpc, NoteHead::Group group) { _noteHeads    [tpc + 1] = group; }
        void setClefOffset(ClefType ct, int offset)           { _clefOffsets[ct] = offset;       }
        void setShowAccidentals(bool val)                     { _showAccidentals = val;          }
        void setOctaveDistance(int val)                       { _octaveDistance = val;           }
      
        int tpc2Position(int tpc) const              { return _notePositions[tpc + 1]; }
        NoteHead::Group tpc2HeadGroup(int tpc) const { return     _noteHeads[tpc + 1]; }
        int clefOffset(ClefType ct) const            { return   _clefOffsets.at(ct);   }
        int octaveDistance() const { return _octaveDistance; }
        bool showAccidentals() const { return _showAccidentals; }

//        friend class MuseScore; //TODO: still necessary?
      };
}
#endif
