//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2010-2014 Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#ifndef STAFFTYPETEMPLATES_H
#define STAFFTYPETEMPLATES_H

#include "ui_stafftypetemplates.h"
#include "libmscore/stafftype.h"

//cc
namespace Ui {
class StaffTypeTemplates;
}

namespace Ms {

class StaffTypeTemplates : public QWidget, private Ui::StaffTypeTemplates {
	Q_OBJECT

      virtual void closeEvent(QCloseEvent*);
      
    public:
       StaffTypeTemplates(QWidget *parent = 0);
       ~StaffTypeTemplates();

    private:
    	 Ui::StaffTypeTemplates *ui;
       const StaffType* curStaffType;

    signals:
       void closed(bool);

    private slots:
      void loadStaffType();
      void newStaffType();
      void removeStaffType();
      void duplicateStaffType();
      void saveAll();
};

}

#endif // STAFFTYPETEMPLATES_H
