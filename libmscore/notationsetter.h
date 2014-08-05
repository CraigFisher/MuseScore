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

 #ifndef __NOTATIONSETTER_H__
 #define __NOTATIONSETTER_H__

/**
 \file
 Definition of class Accidental
*/

#include "mscore/preferences.h"
#include "note.h"
#include "chord.h"

// class QPainter;

namespace Ms {

//---------------------------------------------------------
//   @@ Accidental
//   @P hasBracket  bool
//   @P small       bool
//   @P acctype     Ms::Accidental::Type  (NONE, SHARP, FLAT, SHARP2, FLAT2, NATURAL, ...) (read only)
//   @P role        Ms::Accidental::Role  (AUTO, USER) (read only)
//---------------------------------------------------------

class NotationSetter {      
   private:
   	  NotationSetter() {};

      static void setNotePositions(); 
      static void setNoteHeadGroups();
      static void setAccidentals();
      static void setStaffLines();
      static void setInnerLedgers();
      static void setNotationState();

   public:
      static void setNotation();
    
      };
}
#endif
