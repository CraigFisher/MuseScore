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

class StaffTypeTemplates : public QDialog, private Ui::StaffTypeTemplates {
	Q_OBJECT
      
    public:
       StaffTypeTemplates(QWidget *parent = 0);
       ~StaffTypeTemplates();

    private:
       Ui::StaffTypeTemplates *ui;
      
       std::list<StaffTypeTemplate> localTemplates; //local copy of userTemplates
       StaffTypeTemplate* curTemplate;
       int newTemplateNameIndex = 0;
      
       void getValues();
       void markSelectorItemDirty(QString, bool);
       StaffTypeTemplate* templateByItem(QListWidgetItem*);
       QListWidgetItem* itemByTemplate(StaffTypeTemplate*);

    signals:
       void closed(bool);

    private slots:
      void load();
//STUB
      void create();
      void remove();    //STUB
      void duplicate(); //STUB
      bool save();
      bool save(StaffTypeTemplate* stt);
      void handleExitButton();
      void handleTemplateSwitch(QListWidgetItem*);
};

}

#endif // STAFFTYPETEMPLATES_H
