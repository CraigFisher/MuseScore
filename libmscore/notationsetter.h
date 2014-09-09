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
 Definition of class NotationSetter
*/

namespace Ms {

class NotationSetter {      
   private:
   	  NotationSetter() {};

      static void setNotePositions(); 
      static void setNoteHeadGroups();
      static void setAccidentals();
      static void setStaffLines();
      static void setInnerLedgers();
      static void loadNotationState();

   public:
      static void loadNotation();
    
      };
}
#endif
