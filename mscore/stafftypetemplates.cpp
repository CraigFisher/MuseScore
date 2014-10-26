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
//TODO: EXPLAIN SOMEWHERE THE USE OF ABOLUSTEFILEPATHS AS KEYS
//          FOR DSX'S: STAFFTYPESELECTOR, LOCALTEMPLATES, & StaffTypeTemplate::USERTEMPLATES
//
//          AND NAMES AS SECONDARY KEYS IF PATH IS EMPTY

namespace Ms {

//---------------------------------------------------------
//   StaffTypeTemplates
//---------------------------------------------------------

StaffTypeTemplates::StaffTypeTemplates(QWidget *parent) :
      QDialog(parent),
      ui(new Ui::StaffTypeTemplates)
      {
      setupUi(this);
      
      for (const StaffTypeTemplate& st : StaffTypeTemplate::userTemplates()) {
            localTemplates.emplace_front(st); //hasFile == true, dirty == false
            
            QListWidgetItem* item = new QListWidgetItem; //TODO: ensure this is properly deconstructed
            item->setText(st.name());
            item->setData(Qt::UserRole, st.fileInfo()->absoluteFilePath());
            staffTypeSelector->addItem(item);
            }

      //set curStaffType
      if (staffTypeSelector->count() != 0) {
            QListWidgetItem* item = staffTypeSelector->item(0);
            staffTypeSelector->setCurrentItem(item);
            curTemplate = templateByItem(item);
            }
      else
            curTemplate = NULL;
            
      connect(staffTypeSelector, SIGNAL(itemPressed(QListWidgetItem*)), SLOT(handleTemplateSwitch(QListWidgetItem*)));
      connect(exitButton, SIGNAL(clicked()), SLOT(handleExitButton()));
      connect(loadStaffTypeButton, SIGNAL(clicked()), SLOT(load()));
      connect(newStaffTypeButton, SIGNAL(clicked()), SLOT(create()));
      connect(removeStaffTypeButton, SIGNAL(clicked()), SLOT(remove()));
      connect(duplicateStaffTypeButton, SIGNAL(clicked()), SLOT(duplicate()));
      connect(saveStaffTypesButton, SIGNAL(clicked()), SLOT(save()));
	}
      
//---------------------------------------------------------
//   ~StaffTypeTemplates
//---------------------------------------------------------

StaffTypeTemplates::~StaffTypeTemplates()
      {
      delete ui;
	}
      
//---------------------------------------------------------
//   loadStaffType
//---------------------------------------------------------
      
void StaffTypeTemplates::load()
      {
      StaffTypeTemplate st;
      try {
            if (StaffTypeTemplate::userTemplates().size() >= StaffTypeTemplate::STAFFTYPE_TEMPLATE_LIST_SIZE)
                  throw QObject::tr("StaffType Limit Reached.");

            QString path = mscore->getStaffTypeFilename(true);
            if (path.isEmpty())
                  return;
            
            st.setFileName(path);
            QString absPath = st.fileInfo()->absoluteFilePath();
            for (const StaffTypeTemplate& cur : StaffTypeTemplate::userTemplates()) {
                  if (absPath == cur.fileInfo()->absoluteFilePath())
                        throw QObject::tr("StaffType file already loaded.");
                  }

            QFile f(path);
            if (!f.open(QIODevice::ReadOnly))
                  throw QObject::tr("Cannot read file: failed to open.");
            
            XmlReader xml(&f);
            st.read(xml);
            }
      catch (QString error) {
            QMessageBox::warning(0, QObject::tr("MuseScore: Load Error"), error);
            return;
            }

      //if successful
      st.setName(st.xmlName());   //StaffType._name is not currently written to file, so _xmlName will be used instead
                                  //TODO: possibly write _name to file (at least for templates if stafftype class is extended)
      //push to userTemplates
      st.setHasFile(true);
      st.setDirty(false);
      StaffTypeTemplate::addTemplate(st);
      StaffTypeTemplate::updateSettings();
      
      //push to localTemplates
      localTemplates.emplace_back(st);
      curTemplate = &localTemplates.back();

      //push name and path to staffTypeSelector
      QListWidgetItem* item = new QListWidgetItem; //TODO: make sure of no later memory leaks
      item->setText(st.name());
      item->setData(Qt::UserRole, st.fileInfo()->absoluteFilePath());
      staffTypeSelector->addItem(item);
      staffTypeSelector->setCurrentItem(item);
      }
      
//---------------------------------------------------------
//   newStaffType
//---------------------------------------------------------
      
void StaffTypeTemplates::create() //TODO: WHEN SAVING A NEW STAFFTYPE,
                                        //      PERHAPS RENAME THE STAFFTYPE
                                        //      WITH THE NEW FILE NAME
      {
      // create a default stafftype
      StaffTypeTemplate st;
      
      newTemplateNameIndex++;
      QString defaultName =  QString("New Template %1").arg(newTemplateNameIndex);
      st.setXmlName(defaultName);
      st.setName(defaultName);

      localTemplates.push_back(st);
      curTemplate = &localTemplates.back();

      QListWidgetItem* item = new QListWidgetItem;
      item->setText(defaultName);
      item->setData(Qt::UserRole, "");
      staffTypeSelector->addItem(item);
      staffTypeSelector->setCurrentItem(item);
      }
      
//---------------------------------------------------------
//   removeStaffType
//---------------------------------------------------------

void StaffTypeTemplates::remove()
      {

      }

//---------------------------------------------------------
//   duplicateStaffType
//---------------------------------------------------------

void StaffTypeTemplates::duplicate()
      {

	}
      
//---------------------------------------------------------
//   save
//    overloaded
//    return true on successful save
//---------------------------------------------------------
      
bool StaffTypeTemplates::save() {
      return save(curTemplate);
      }
      
//---------------------------------------------------------
//   save
//    return true on successful save
//---------------------------------------------------------
      
bool StaffTypeTemplates::save(StaffTypeTemplate* stt)
      {
      if (stt == NULL)
            return false;
      
      if (stt->dirty() || !stt->hasFile()) {
            if (!stt->hasFile()) {
                  //assign a path to localStafftype
                  QString path = mscore->getStaffTypeFilename(false);
                  if (path.isEmpty())
                        return false;

                  stt->setFileName(path);
                  QString ext(".stt");
                  if (stt->fileInfo()->suffix().isEmpty())
                        stt->setFileName(path + ext);
                  
                  QListWidgetItem* item = itemByTemplate(stt);
                  item->setData(Qt::UserRole, path);
                  }
            
            if (stt->dirty())
                  getValues();
            
            //open and write to file
            QFile f(stt->fileInfo()->filePath()); //TODO: make sure during testing/debugging that file paths are right
            if (!f.open(QIODevice::WriteOnly)) {
                  QMessageBox::critical(this,
                        tr("MuseScore: Save Staff Type Template"), tr("Open Style File\n%1\nfailed: %2").arg(f.fileName()).arg(QString(strerror(errno))));
                  return false;
                  }
            Xml xml(&f);
            xml.header();              //TODO: possibly prepend MSC_VERSION if extended stafftype class
            stt->write(xml);
            if (f.error() != QFile::NoError) {
                  QMessageBox::critical(this,
                        tr("MuseScore: Save Staff Type Template"), tr("Write Style failed: %1").arg(f.errorString()));
                  return false;
                  }
            stt->setDirty(false);
            
            //update StaffType::userTemplates
            if (!stt->hasFile()) {
                  stt->setHasFile(true);
                  StaffTypeTemplate::addTemplate(*stt);
                  StaffTypeTemplate::updateSettings();
                  }
            else {
                  StaffTypeTemplate::updateTemplate(*stt);
                  }
            markSelectorItemDirty(stt->name(), false);
            }
      return true;
      }
      
//---------------------------------------------------------
//   handleCancelButton
//---------------------------------------------------------
      
void StaffTypeTemplates::handleExitButton() {
      for (StaffTypeTemplate& stt : localTemplates) {
            if (stt.dirty() || !stt.hasFile()) {
                  QMessageBox::StandardButton n = QMessageBox::warning(this, tr("MuseScore"),
                     tr("Save changes to the StaffType Template: \"%1\"?").arg(stt.name()),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                     QMessageBox::Save); //TODO: make this dialog not jump around with multiple uses
                  if (n == QMessageBox::Cancel) {
                        return;       //don't exit
                        }
                  else if (n == QMessageBox::Save) {
                        if(!save(&stt))
                              return; //template not resolved
                        }
                  }
            }
            
      close(); //all dirty and unsaved templates resolved
      }

//---------------------------------------------------------
//   handleStaffTypeSwitch
//---------------------------------------------------------

void StaffTypeTemplates::handleTemplateSwitch(QListWidgetItem* item) {
      curTemplate = templateByItem(item);
      }
      
//---------------------------------------------------------
//   getValues
//---------------------------------------------------------
      
void StaffTypeTemplates::getValues() {
      
      }
      
//---------------------------------------------------------
//   markSelectorItemDirty
//---------------------------------------------------------
      
void StaffTypeTemplates::markSelectorItemDirty(QString name, bool dirty) {
//      STUB
      }
      
//---------------------------------------------------------
//   templateByItem
//---------------------------------------------------------
      
StaffTypeTemplate* StaffTypeTemplates::templateByItem(QListWidgetItem* item) {
      QString itemKey;
      QString templateKey;
      itemKey = item->data(Qt::UserRole).toString();
      bool useFilePath = !itemKey.isEmpty(); //whether to use name or filePath as key
      itemKey = useFilePath ? itemKey : item->text();
      
      for (StaffTypeTemplate& stt : localTemplates) {
            if (useFilePath)
                  templateKey = stt.fileInfo()->absoluteFilePath();
            else { //use name
                  //if using stt's name as key, make sure stt's filePath is empty
                  if (!stt.fileInfo()->absoluteFilePath().isEmpty())
                        continue;
                  templateKey = stt.name();
                  }
            if (itemKey == templateKey)
                  return &stt;
            }
      
      return NULL;
      }
      
//---------------------------------------------------------
//   itemByTemplate
//---------------------------------------------------------

QListWidgetItem* StaffTypeTemplates::itemByTemplate(StaffTypeTemplate* stt) {
      QString itemKey;
      QString templateKey;
      bool useFilePath = stt->hasFile(); //whether to use name or filePath as key
      templateKey = useFilePath ? stt->fileInfo()->absoluteFilePath() : stt->name();
      
      int count = staffTypeSelector->count();
      for (int i = 0; i < count; i++) {
            QListWidgetItem* item = staffTypeSelector->item(i);
            if (useFilePath)
                  itemKey = item->data(Qt::UserRole).toString();
            else {
                  //if using item's name as key, make sure item's filePath is empty
                  if (!item->data(Qt::UserRole).toString().isEmpty())
                        continue;
                  itemKey = item->text();
                  }
            if (itemKey == templateKey)
                  return item;
            }
      
      return NULL;
      }

//---------------------------------------------------------
//   closeEvent
//---------------------------------------------------------
      
//void StaffTypeTemplates::closeEvent(QCloseEvent* ev)
//      {
//      emit closed(false);
//      QWidget::closeEvent(ev);
//      }
      
//---------------------------------------------------------
//   showStaffTypeTemplates
//---------------------------------------------------------

//void MuseScore::showStaffTypeTemplates(bool visible)
//      {
//      if (staffTypeTemplates == 0) {
//            if (!visible)
//                  return;
//            staffTypeTemplates = new StaffTypeTemplates(this);
//            connect(staffTypeTemplates, SIGNAL(closed(bool)), notationId, SLOT(setChecked(bool)));
//            }
//      staffTypeTemplates->setVisible(visible);
//      notationId->setChecked(visible);
//      }
      
}
