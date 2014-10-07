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

#ifndef __STAFF_H__
#define __STAFF_H__

/**
 \file
 Definition of class Staff.
*/

#include "mscore.h"
#include "velo.h"
#include "pitch.h"
#include "cleflist.h"
#include "keylist.h"
#include "stafftype.h"
#include "groups.h"
#include "notationrules.h"

namespace Ms {

class Instrument;
class InstrumentTemplate;
class Xml;
class Part;
class Score;
class KeyList;
class StaffType;
class Staff;
class StringData;
struct ClefTypeList;
class Segment;
class Clef;
class TimeSig;
class Ottava;

enum class Key;

//---------------------------------------------------------
//   LinkedStaves
//---------------------------------------------------------

class LinkedStaves {
      QList<Staff*> _staves;

   public:
      LinkedStaves() {}
      QList<Staff*>& staves()             { return _staves; }
      const QList<Staff*>& staves() const { return _staves; }
      void add(Staff*);
      void remove(Staff*);
      bool isEmpty() const { return _staves.isEmpty(); }
      };

//---------------------------------------------------------
//   BracketItem
//---------------------------------------------------------

struct BracketItem {
      BracketType _bracket;
      int _bracketSpan;

      BracketItem() {
            _bracket = BracketType::NO_BRACKET;
            _bracketSpan = 0;
            }
      BracketItem(BracketType a, int b) {
            _bracket = a;
            _bracketSpan = b;
            }
      };

//---------------------------------------------------------
//   SwingParameters
//---------------------------------------------------------

struct SwingParameters {
      int swingUnit;
      int swingRatio;
      };

//---------------------------------------------------------
//    Staff
///    Global staff data not directly related to drawing.
//---------------------------------------------------------

class Staff : public QObject {
      Q_OBJECT

      Score* _score ;
      Part* _part       { 0 };

      ClefList clefs;
      ClefTypeList _defaultClefType;

      KeyList _keys;
      std::map<int,TimeSig*> timesigs;

      QList <BracketItem> _brackets;
      int _barLineSpan   { 1     };    ///< 0 - no bar line, 1 - span this staff, ...
      int _barLineFrom   { 0     };    ///< line of start staff to draw the barline from (0 = staff top line, ...)
      int _barLineTo;                  ///< line of end staff to draw the bar line to (0= staff top line, ...)
      bool _small        { false };
      bool _invisible    { false };
      bool _neverHide    { false };    ///< always show this staff, even if empty and hideEmptyStaves is true
      bool _showIfEmpty  { false };    ///< show this staff if system is empty and hideEmptyStaves is true
      QColor _color      { MScore::defaultColor };
      qreal _userDist    { 0.0   };        ///< user edited extra distance
      qreal _userMag     { 1.0   };             // allowed 0.1 - 10.0

      StaffType _staffType;
      LinkedStaves* _linkedStaves { nullptr };
      QMap<int,int> _channelList[VOICES];
      QMap<int,SwingParameters> _swingList;

      VeloList _velocities;         ///< cached value
      PitchList _pitchOffsets;      ///< cached value
      
      NotationRules* _notation; //cc

   public:
      Staff(Score* = 0);
      ~Staff();
      void init(const InstrumentTemplate*, const StaffType *staffType, int);
      void initFromStaffType(const StaffType* staffType);

      bool isTop() const;
      QString partName() const;
      int rstaff() const;
      int idx() const;
      void read(XmlReader&);
      void read114(XmlReader&);
      void write(Xml& xml) const;
      Part* part() const             { return _part;        }
      void setPart(Part* p)          { _part = p;           }

      BracketType bracket(int idx) const;
      int bracketSpan(int idx) const;
      void setBracket(int idx, BracketType val);
      void setBracketSpan(int idx, int val);
      int bracketLevels() const      { return _brackets.size(); }
      void addBracket(BracketItem);
      QList <BracketItem> brackets() const { return _brackets; }
      void cleanupBrackets();

      ClefTypeList clefType(int tick) const;
      ClefTypeList defaultClefType() const           { return _defaultClefType; }
      void setDefaultClefType(const ClefTypeList& l) { _defaultClefType = l; }
      ClefType clef(int tick) const;

      void setClef(Clef*);
      void removeClef(Clef*);

      void addTimeSig(TimeSig*);
      void removeTimeSig(TimeSig*);
      void clearTimeSig()            { timesigs.clear(); }
      Fraction timeStretch(int tick) const;
      TimeSig* timeSig(int tick) const;
      const Groups& group(int tick) const;

      KeyList* keyList()               { return &_keys;      }
      Key key(int tick) const;
      int nextKeyTick(int tick) const;
      int currentKeyTick(int tick) const;
      Key prevKey(int tick) const;
      void setKey(int tick, Key);
      void removeKey(int tick);

      bool show() const;
      bool slashStyle() const;
      bool small() const             { return _small;       }
      void setSmall(bool val)        { _small = val;        }
      bool invisible() const         { return _invisible;   }
      void setInvisible(bool val)    { _invisible = val;    }
      bool neverHide() const         { return _neverHide;   }
      void setNeverHide(bool val)    { _neverHide = val;    }
      bool showIfEmpty() const       { return _showIfEmpty; }
      void setShowIfEmpty(bool val)  { _showIfEmpty = val;  }

      void setSlashStyle(bool val);
      int lines() const;
      void setLines(int);
      qreal lineDistance() const;
      int barLineSpan() const        { return _barLineSpan; }
      int barLineFrom() const        { return _barLineFrom; }
      int barLineTo() const          { return _barLineTo;   }
      void setBarLineSpan(int val)   { _barLineSpan = val;  }
      void setBarLineFrom(int val)   { _barLineFrom = val;  }
      void setBarLineTo(int val);
      Score* score() const           { return _score;       }
      qreal mag() const;
      qreal height() const;
      qreal spatium() const;
      int channel(int tick, int voice) const;
      QMap<int,int>* channelList(int voice) { return  &_channelList[voice]; }
      SwingParameters swing(int tick)  const;
      QMap<int,SwingParameters>* swingList() { return &_swingList; }

      const StaffType* staffType() const { return &_staffType;      }
      StaffType* staffType()             { return &_staffType;      }

      void setStaffType(const StaffType* st);
      StaffGroup staffGroup() const    { return _staffType.group(); }
      bool isPitchedStaff() const      { return staffGroup() == StaffGroup::STANDARD; }
      bool isTabStaff() const          { return staffGroup() == StaffGroup::TAB; }
      bool isDrumStaff() const         { return staffGroup() == StaffGroup::PERCUSSION; }

      VeloList& velocities()           { return _velocities;     }
      int pitchOffset(int tick)        { return _pitchOffsets.pitchOffset(tick);   }
      void updateOttava();

      LinkedStaves* linkedStaves() const    { return _linkedStaves; }
      void setLinkedStaves(LinkedStaves* l) { _linkedStaves = l;    }
      QList<Staff*> staffList() const;
      void linkTo(Staff* staff);
      bool isLinked(Staff* staff);
      void unlink(Staff* staff);
      bool primaryStaff() const;

      qreal userDist() const        { return _userDist;  }
      void setUserDist(qreal val)   { _userDist = val;   }
      qreal userMag() const         { return _userMag;   }
      void setUserMag(qreal m)      { _userMag = m;      }

      void spatiumChanged(qreal /*oldValue*/, qreal /*newValue*/);
      bool genKeySig();
      bool showLedgerLines();


      QColor color() const                { return _color; }
      void setColor(const QColor& val)    { _color = val;    }
      void undoSetColor(const QColor& val);
      void insertTime(int tick, int len);
      
      //cc
      void setNotation(NotationRules*); //TODO: guard access, possibly make friend
      NotationRules* notationRules() const { return _notation; }
      };

}     // namespace Ms
#endif

