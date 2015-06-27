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
#include "durationtype.h"
#include "clef.h"
#include <map>
#include <array>
#include <vector>
#include <QFile>

/**
 \file
 Definition of class AltNoteMappings
*/

namespace Ms {

class Xml;
class XmlReader;

class AltNoteMappings {      
    public:
        enum class FillType : signed char {
            TRADITIONAL = 0,
            FILLED,
            HOLLOW,
            };

    private:
        bool _activated   = false;
        bool _initialized = false;
    
        std::array<int, 35> _notePositions;
        std::array<NoteHead::Group, 35> _noteHeads;
        std::array<FillType, 35> _fillTypes;
        std::array<QColor, 35> _noteColors;
        std::map<ClefType, int> _clefOffsets;
        int _octaveDistance = 7;
        bool _showAccidentals = true;
    
        static const NoteHead::Group defaultH = NoteHead::Group::HEAD_NORMAL;
        static const FillType defaultF;
        static const QColor defaultC;
        void setTraditionalClefOffsets();

        void writeMappings(Xml&) const;
        void readMappings(XmlReader& e);
        void writeClefOffsets(Xml&) const;
        void readClefOffsets(XmlReader& e);

    public:
        AltNoteMappings(bool setActive);
        explicit AltNoteMappings(const QString&);
        bool operator==(const AltNoteMappings& st) const;
      
        void activate();
        void deActivate()    { _activated = false; }
        bool active() const  {  return _activated; }
      
        void write(Xml&) const;
        void read(XmlReader&);
      
        void setNotePosition(int tpc, int pos)                { _notePositions[tpc + 1] = pos;      }
        void setNoteHeadGroup(int tpc, NoteHead::Group group) { _noteHeads    [tpc + 1] = group;    }
        void setNoteColor(int tpc, QColor color)              { _noteColors   [tpc + 1] = color;    }
        void setNoteFill(int tpc, FillType filltype)          { _fillTypes    [tpc + 1] = filltype; }
        void setClefOffset(ClefType ct, int offset)           { _clefOffsets[ct] = offset;          }
        void setShowAccidentals(bool val)                     { _showAccidentals = val;             }
        void setOctaveDistance(int val)                       { _octaveDistance = val;              }
      
        int tpc2Position(int tpc) const              { return _notePositions[tpc + 1]; }
        FillType tpc2FillType(int tpc) const         { return     _fillTypes[tpc + 1]; }
        NoteHead::Group tpc2HeadGroup(int tpc) const { return     _noteHeads[tpc + 1]; }
        QColor tpc2Color(int tpc) const              { return    _noteColors[tpc + 1]; }
        int clefOffset(ClefType ct) const            { return   _clefOffsets.at(ct);   }
        NoteHead::Type headType (TDuration duration, int tpc) const;
      
        int octaveDistance() const { return _octaveDistance; }
        bool showAccidentals() const { return _showAccidentals; }

        int getPitch(int tpc, int step);
        int getTpc(int position, int accidental);
        int getTpc(int position);
      };
}
#endif
