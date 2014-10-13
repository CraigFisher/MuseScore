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

#include "stafftypetemplates.h"
#include "musescore.h"
#include "libmscore/xml.h"

//cc
namespace Ms {

StaffTypeTemplates::StaffTypeTemplates(QWidget *parent) :
      QWidget(parent, Qt::Dialog),
      ui(new Ui::StaffTypeTemplates)
      {
      setupUi(this);

      //StaffTypeTemplates will correspond to StaffType::_userTemplates
      for (const StaffType& st : StaffType::userTemplates()) {
            QListWidgetItem* item = new QListWidgetItem; //TODO: MAKE SURE OF NO LATER MEMORY LEAKS
            item->setText(st.name());
            staffTypeTemplates->addItem(item);
            }
            
      //set curStaffType
      if (staffTypeTemplates->count() != 0) {
            staffTypeTemplates->setCurrentRow(0);
            curStaffType = &(StaffType::userTemplates().front());
            }

      connect(cancelButton, SIGNAL(clicked()), SLOT(close()));
      connect(loadStaffTypeButton, SIGNAL(clicked()), SLOT(loadStaffType()));
      connect(newStaffTypeButton, SIGNAL(clicked()), SLOT(newStaffType()));
      connect(removeStaffTypeButton, SIGNAL(clicked()), SLOT(removeStaffType()));
      connect(duplicateStaffTypeButton, SIGNAL(clicked()), SLOT(duplicateStaffType()));
      connect(saveStaffTypesButton, SIGNAL(clicked()), SLOT(saveAll()));
	}

StaffTypeTemplates::~StaffTypeTemplates()
      {
      delete ui;
	}
      
void StaffTypeTemplates::loadStaffType()
      {
      StaffType* st = new StaffType();
      try {
            if (StaffType::userTemplates().size() >= StaffType::STAFFTYPE_TEMPLATE_LIST_SIZE)
                  throw QObject::tr("StaffType Limit Reached.");
            
            QString path = mscore->getNotationFilename(true);
            if (path.isEmpty())
                  return;
            
            st->setFileName(path);
            QString absPath = st->fileInfo()->absolutePath();
            for (const StaffType& cur : StaffType::userTemplates()) {
                  if (absPath == cur.fileInfo()->absolutePath())
                        throw QObject::tr("StaffType file already loaded.");
                  }

            QFile f(path);
            if (!f.open(QIODevice::ReadOnly))
                  throw QObject::tr("Cannot read file: failed to open.");
            
            XmlReader xml(&f);
            st->read(xml);
            }
      catch (QString error) {
            delete st;
            QMessageBox::warning(0, QObject::tr("MuseScore: Load Error"), error);
            return;
            }

      //if successful
      QListWidgetItem* item = new QListWidgetItem; //TODO: make sure of no later memory leaks
      item->setText(st->xmlName());      //StaffType._name is not currently written to file, so _xmlName will be used instead
      staffTypeTemplates->addItem(item);
      StaffType::addTemplate(*st);                 //TODO: Possibly convert _userTemplates into a list of
                                                   //      pointers instead (and then just push st).
      }
      
void StaffTypeTemplates::newStaffType()
      {
      
      }

void StaffTypeTemplates::removeStaffType()
      {

	}

void StaffTypeTemplates::duplicateStaffType()
      {

	}
      
void StaffTypeTemplates::saveAll()
      {

      //TODO: WRITE XML, ACTUALLY SAVE FILES

      QSettings settings;
      int i = 0;
      for (const StaffType& st : StaffType::userTemplates()) {
            QString path(st.fileInfo()->absolutePath());
            settings.setValue(QString("user-stafftypes-%1").arg(i), path);
            i++;
            }
      while (i < StaffType::STAFFTYPE_TEMPLATE_LIST_SIZE) {
            settings.setValue(QString("user-stafftypes-%1").arg(i), "");
            }
      }
      
void StaffTypeTemplates::closeEvent(QCloseEvent* ev)
      {
      emit closed(false);
      QWidget::closeEvent(ev);
      }

void MuseScore::showStaffTypeTemplates(bool visible)
      {
      if (staffTypeTemplates == 0) {
            if (!visible)
                  return;
            staffTypeTemplates = new StaffTypeTemplates(this);
            connect(staffTypeTemplates, SIGNAL(closed(bool)), notationId, SLOT(setChecked(bool)));
            }
      staffTypeTemplates->setVisible(visible);
      notationId->setChecked(visible);
      }
      
}
