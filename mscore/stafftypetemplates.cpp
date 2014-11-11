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

const int StaffTypeTemplates::tpcLookup[7][5] = { { 0,  7,  14, 21, 28},
                                                  { 2,  9,  16, 23, 30},
                                                  { 4,  11, 18, 25, 32},
                                                  { -1, 6,  13, 20, 27},
                                                  { 1,  8,  15, 22, 29},
                                                  { 3,  10, 17, 24, 31},
                                                  { 5,  12, 19, 26, 33} };
      
const NoteHead::Group StaffTypeTemplates::noteheadLookup[14] = {
            NoteHead::Group::HEAD_NORMAL,
            NoteHead::Group::HEAD_CROSS,
            NoteHead::Group::HEAD_DIAMOND,
            NoteHead::Group::HEAD_TRIANGLE,
            NoteHead::Group::HEAD_SLASH,
            NoteHead::Group::HEAD_XCIRCLE,
            NoteHead::Group::HEAD_DO,
            NoteHead::Group::HEAD_RE,
            NoteHead::Group::HEAD_MI,
            NoteHead::Group::HEAD_FA,
            NoteHead::Group::HEAD_SOL,
            NoteHead::Group::HEAD_LA,
            NoteHead::Group::HEAD_TI,
            NoteHead::Group::HEAD_BREVIS_ALT
            };
      
const ClefType StaffTypeTemplates::clefLookup[17] = {
      ClefType::G,
      ClefType::G1,
      ClefType::G2,
      ClefType::G3,
      ClefType::F,
      ClefType::F8,
      ClefType::F15,
      ClefType::F_B,
      ClefType::F_C,
      ClefType::C1,
      ClefType::C2,
      ClefType::C3,
      ClefType::C4,
      ClefType::C5,
      ClefType::G4,
      ClefType::F_8VA,
      ClefType::F_15MA
};


//---------------------------------------------------------
//   StaffTypeTemplates
//---------------------------------------------------------

StaffTypeTemplates::StaffTypeTemplates(QWidget *parent) :
      QDialog(parent),
      ui(new Ui::StaffTypeTemplates)
      {
      setupUi(this);
      
      /* retrieve STT::userTemplates and
         use them to populate localTemplates */
      for (const StaffTypeTemplate& st : StaffTypeTemplate::userTemplates()) {
            Q_ASSERT(st.hasFile() && !st.dirty());
            
            localTemplates.emplace_back(st);
            QListWidgetItem* item = new QListWidgetItem;
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
      else  {
            enableInput(false);
            curTemplate = NULL;
            }

      connect(staffTypeSelector, SIGNAL(currentRowChanged(int)), SLOT(handleTemplateSwitch(int)));
      connect(exitButton, SIGNAL(clicked()), SLOT(handleExitButton()));
      connect(loadStaffTypeButton, SIGNAL(clicked()), SLOT(load()));
      connect(newStaffTypeButton, SIGNAL(clicked()), SLOT(create()));
      connect(removeStaffTypeButton, SIGNAL(clicked()), SLOT(remove()));
      connect(duplicateStaffTypeButton, SIGNAL(clicked()), SLOT(duplicate()));
      connect(saveStaffTypesButton, SIGNAL(clicked()), SLOT(save()));

      doubleFlatOffset->setMinimum(-24); //TODO: determine if there are more logical values here
      flatOffset->setMinimum(-24);
      naturalOffset->setMinimum(-24);
      sharpOffset->setMinimum(-24);
      doubleSharpOffset->setMinimum(-24);
      
      noteLetterIdx = noteComboBox->currentIndex();
      clefIdx = clefComboBox->currentIndex();
      setValues();
      connectInput();
      }
      
//---------------------------------------------------------
//   ~StaffTypeTemplates
//---------------------------------------------------------

StaffTypeTemplates::~StaffTypeTemplates()
      {
      delete ui;
	}
      
//---------------------------------------------------------
//   connectInput
//---------------------------------------------------------

void StaffTypeTemplates::connectInput() const
      {
      connect(noteComboBox, SIGNAL(currentIndexChanged(const QString&)), SLOT(switchNoteLetter(const QString&)));
      connect(clefComboBox, SIGNAL(currentIndexChanged(const QString&)), SLOT(switchClef(const QString&)));
      connect(showAccidentals, SIGNAL(clicked(bool)), SLOT(setShowAccidental(bool)));
      connect(octaveDistance, SIGNAL(valueChanged(int)), SLOT(setOctaveDistance(int)));
      connect(clefOffset, SIGNAL(valueChanged(int)), SLOT(setClefOffset(int)));
      
      connect(doubleFlatOffset, SIGNAL(valueChanged(int)), SLOT(setDoubleFlatOffset(int)));
      connect(flatOffset, SIGNAL(valueChanged(int)), SLOT(setFlatOffset(int)));
      connect(naturalOffset, SIGNAL(valueChanged(int)), SLOT(setNaturalOffset(int)));
      connect(sharpOffset, SIGNAL(valueChanged(int)), SLOT(setSharpOffset(int)));
      connect(doubleSharpOffset, SIGNAL(valueChanged(int)), SLOT(setDoubleSharpOffset(int)));
      
      connect(doubleFlatNotehead, SIGNAL(currentIndexChanged(int)), SLOT(setDoubleFlatNotehead(int)));
      connect(flatNotehead, SIGNAL(currentIndexChanged(int)), SLOT(setFlatNotehead(int)));
      connect(naturalNotehead, SIGNAL(currentIndexChanged(int)), SLOT(setNaturalNotehead(int)));
      connect(sharpNotehead, SIGNAL(currentIndexChanged(int)), SLOT(setSharpNotehead(int)));
      connect(doubleSharpNotehead, SIGNAL(currentIndexChanged(int)), SLOT(setDoubleSharpNotehead(int)));
      }

//---------------------------------------------------------
//   disconnectInput
//---------------------------------------------------------

void StaffTypeTemplates::disconnectInput() const
      {
      disconnect(noteComboBox, SIGNAL(currentIndexChanged(const QString&)), 0, 0);
      disconnect(clefComboBox, SIGNAL(currentIndexChanged(const QString&)), 0, 0);
      disconnect(showAccidentals, SIGNAL(clicked(bool)), 0, 0);
      disconnect(octaveDistance, SIGNAL(valueChanged(int)), 0, 0);
      disconnect(clefOffset, SIGNAL(valueChanged(int)), 0, 0);
      
      disconnect(doubleFlatOffset, SIGNAL(valueChanged(int)), 0, 0);
      disconnect(flatOffset, SIGNAL(valueChanged(int)), 0, 0);
      disconnect(naturalOffset, SIGNAL(valueChanged(int)), 0, 0);
      disconnect(sharpOffset, SIGNAL(valueChanged(int)), 0, 0);
      disconnect(doubleSharpOffset, SIGNAL(valueChanged(int)), 0, 0);
      
      disconnect(doubleFlatNotehead, SIGNAL(currentIndexChanged(int)), 0, 0);
      disconnect(flatNotehead, SIGNAL(currentIndexChanged(int)), 0, 0);
      disconnect(naturalNotehead, SIGNAL(currentIndexChanged(int)), 0, 0);
      disconnect(sharpNotehead, SIGNAL(currentIndexChanged(int)), 0, 0);
      disconnect(doubleSharpNotehead, SIGNAL(currentIndexChanged(int)), 0, 0);
      }
      
//---------------------------------------------------------
//   setValues
//       set all widget values based on curTemplate
//---------------------------------------------------------
      
void StaffTypeTemplates::setValues() const
      {
      if (curTemplate == NULL)
            return;
      
      const NoteMappings* mappings = curTemplate->noteMappings();
      int bbTpc = tpcLookup[noteLetterIdx][0]; //current bb      note
      int bTpc  = tpcLookup[noteLetterIdx][1]; //current b       note
      int nTpc  = tpcLookup[noteLetterIdx][2]; //current natural note
      int sTpc  = tpcLookup[noteLetterIdx][3]; //current #       note
      int ssTpc = tpcLookup[noteLetterIdx][4]; //current ##      note
      
      //SET NOTE OFFSETS (POSITIONS)
      doubleFlatOffset-> setValue(mappings->tpc2Position(bbTpc));
      flatOffset->       setValue(mappings->tpc2Position(bTpc));
      naturalOffset->    setValue(mappings->tpc2Position(nTpc));
      sharpOffset->      setValue(mappings->tpc2Position(sTpc));
      doubleSharpOffset->setValue(mappings->tpc2Position(ssTpc));
      
      //SET NOTEHEADS
      doubleFlatNotehead-> setCurrentIndex(noteheadIndex(mappings->tpc2HeadGroup(bbTpc)));
      flatNotehead->       setCurrentIndex(noteheadIndex(mappings->tpc2HeadGroup(bTpc)));
      naturalNotehead->    setCurrentIndex(noteheadIndex(mappings->tpc2HeadGroup(nTpc)));
      sharpNotehead->      setCurrentIndex(noteheadIndex(mappings->tpc2HeadGroup(sTpc)));
      doubleSharpNotehead->setCurrentIndex(noteheadIndex(mappings->tpc2HeadGroup(ssTpc)));
      
      ClefType curClef = clefLookup[clefIdx];
      clefOffset->setValue(mappings->clefOffset(curClef));
      showAccidentals->setChecked(mappings->showAccidentals());
      octaveDistance->setValue(mappings->octaveDistance());
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
      if (!inputEnabled)
            enableInput(true);
      }
      
//---------------------------------------------------------
//   newStaffType
//---------------------------------------------------------
      
void StaffTypeTemplates::create()
      {
      // create a default STT
      StaffTypeTemplate st;
      newTemplateNameIndex++;
      QString defaultName =  QString("New Template %1").arg(newTemplateNameIndex);
      st.setXmlName(defaultName);
      st.setName(defaultName);
      st.setDirty(false);
      st.setHasFile(false);
      
      // add STT to local list
      localTemplates.push_back(st);
      curTemplate = &localTemplates.back();

      // add STT's name and key to the list widget
      QListWidgetItem* item = new QListWidgetItem;
      item->setText(defaultName);
      item->setData(Qt::UserRole, "");
      staffTypeSelector->addItem(item);
      staffTypeSelector->setCurrentItem(item);
      if (!inputEnabled)
            enableInput(true);
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
      
bool StaffTypeTemplates::save()
      {
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
            
      bool newTemplate = false;
      if (stt->dirty() || !stt->hasFile()) {
            QListWidgetItem* selectorItem = itemByTemplate(stt);
            
            //if localStaffType lacks a filePath, assign a filePath to it
            if (!stt->hasFile()) {
                  newTemplate = true;
                  QString path = mscore->getStaffTypeFilename(false);
                  if (path.isEmpty())
                        return false;
                  stt->setFileName(path); //set file path to template
                  stt->setHasFile(true);
                  QString ext(".stt");
                  if (stt->fileInfo()->suffix().isEmpty())
                        stt->setFileName(path + ext);
                  selectorItem->setData(Qt::UserRole, path); //set file path to selector item
                  }
            
            //open and write to file
            QFile f(stt->fileInfo()->filePath());
            if (!f.open(QIODevice::WriteOnly)) {
                  if (newTemplate) {   //if error occurs with new template, remove file association
                        stt->setFileName("");
                        stt->setHasFile(false);
                        }
                  QMessageBox::critical(this,
                        tr("MuseScore: Save Staff Type Template"), tr("Open Style File\n%1\nfailed: %2").arg(f.fileName()).arg(QString(strerror(errno))));
                  return false;
                  }
            Xml xml(&f);
            xml.header();              //TODO: possibly prepend MSC_VERSION if extended stafftype class
            stt->write(xml);
            
            //check for errors in writing
            if (f.error() != QFile::NoError) {
                  if (newTemplate) {   //if error occurs with new template, remove file association
                        stt->setFileName("");
                        stt->setHasFile(false);
                        }
                  QMessageBox::critical(this,
                        tr("MuseScore: Save Staff Type Template"), tr("Write Style failed: %1").arg(f.errorString()));

                  return false;
                  }
            /* mark local template clean before updating
               STT::userTemplates so that the corresponding
               userTemplate is also marked clean            */
            markTemplateDirty(stt, false);
            
            //update StaffType::userTemplates
            if (newTemplate) {
                  StaffTypeTemplate::addTemplate(*stt);
                  StaffTypeTemplate::updateSettings();
                  }
            else {
                  StaffTypeTemplate::updateTemplate(*stt);
                  }
            }
      return true;
      }
      
//---------------------------------------------------------
//   handleCancelButton
//---------------------------------------------------------
      
void StaffTypeTemplates::handleExitButton()
      {
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
//   handleTemplateSwitch
//---------------------------------------------------------

void StaffTypeTemplates::handleTemplateSwitch(int row)
      {
      curTemplate = templateByItem(staffTypeSelector->item(row));
      disconnectInput();
      setValues();
      connectInput();
      }

//---------------------------------------------------------
//   switchNoteLetter
//---------------------------------------------------------
      
void StaffTypeTemplates::switchNoteLetter(const QString& text)
      {
      doubleFlatLabel->setText(text + "bb");
      flatLabel->setText(text + "b");
      naturalLabel->setText(text);
      sharpLabel->setText(text + "#");
      doubleSharpLabel->setText(text + "##");
      
      noteLetterIdx = noteComboBox->currentIndex();
      disconnectInput();
      setValues();
      connectInput();
      }
      
//---------------------------------------------------------
//   switchClef
//---------------------------------------------------------
      
void StaffTypeTemplates::switchClef(const QString& text)
      {
      clefIdx = clefComboBox->currentIndex();
      ClefType curClef = clefLookup[clefIdx];
      const NoteMappings* mappings = curTemplate->noteMappings();
      
      disconnectInput();
      clefOffset->setValue(mappings->clefOffset(curClef));
      connectInput();
      }
      
//---------------------------------------------------------
//   markSelectorItemDirty
//---------------------------------------------------------
      
void StaffTypeTemplates::markTemplateDirty(StaffTypeTemplate* stt, bool dirty)
      {
      if (stt->dirty() == dirty)
            return;
      QListWidgetItem* item = itemByTemplate(stt);
      stt->setDirty(dirty);
      if (dirty)
            item->setText(QString("%1*").arg(stt->name()));
      else
            item->setText(stt->name());
      }
      
//---------------------------------------------------------
//   enableInput
//---------------------------------------------------------
      
void StaffTypeTemplates::enableInput(bool enable) const //TODO: rename to "enableEditorForms" or something
      {
      inputEnabled = enable;
      
      doubleSharpOffset->setEnabled(enable);
      sharpNotehead->setEnabled(enable);
      doubleFlatOffset->setEnabled(enable);
      naturalOffset->setEnabled(enable);
      flatOffset->setEnabled(enable);
      doubleSharpNotehead->setEnabled(enable);
      doubleFlatNotehead->setEnabled(enable);
      sharpOffset->setEnabled(enable);
      flatNotehead->setEnabled(enable);
      naturalNotehead->setEnabled(enable);
      noteComboBox->setEnabled(enable);
      groupBox_2->setEnabled(enable);
      clefComboBox->setEnabled(enable);
      clefOffset->setEnabled(enable);
      showAccidentals->setEnabled(enable);
      octaveDistance->setEnabled(enable);
      }
      
//---------------------------------------------------------
//   templateByItem
//---------------------------------------------------------
      
StaffTypeTemplate* StaffTypeTemplates::templateByItem(QListWidgetItem* item)
      {
      QString itemKey;
      QString templateKey;
      itemKey = item->data(Qt::UserRole).toString();
      bool useFilePath = !itemKey.isEmpty(); //whether to use name or filePath as key
      if (!useFilePath) {
            itemKey = item->text();
            if (itemKey[itemKey.size() - 1] == '*')
                  itemKey = itemKey.remove(itemKey.size()-1, 1);
            }

      //search local templates until "itemKey == templateKey"
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

QListWidgetItem* StaffTypeTemplates::itemByTemplate(StaffTypeTemplate* stt)
      {
      if (stt == curTemplate)
            return staffTypeSelector->currentItem();
      
      QString itemKey;
      QString templateKey;
      
      //whether to use name or filePath as key
      bool useFilePath = stt->hasFile();
      templateKey = useFilePath ? stt->fileInfo()->absoluteFilePath() : stt->name();

      //search widget items until "itemKey == templateKey"
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
                  if (itemKey[itemKey.size() - 1] == '*')
                        itemKey = itemKey.remove(itemKey.size()-1, 1);
                  }
            if (itemKey == templateKey)
                  return item;
            }
      
      return NULL;
      }
      
//TODO: ADD METHOD HEADERS
      
void StaffTypeTemplates::setOffset(int accidentalIdx, int offset)
      {
      int tpc = tpcLookup[noteLetterIdx][accidentalIdx];
      curTemplate->noteMappings()->setNotePosition(tpc, offset);
      markTemplateDirty(curTemplate, true);
      }
      
void StaffTypeTemplates::setNotehead(int accidentalIdx, int headIdx)
      {
      int tpc = tpcLookup[noteLetterIdx][accidentalIdx];
      curTemplate->noteMappings()->setNoteHeadGroup(tpc, noteheadLookup[headIdx]);
      markTemplateDirty(curTemplate, true);
      }
      
void StaffTypeTemplates::setClefOffset(int clefOffset)
      {
      ClefType curClef = clefLookup[clefIdx];
      curTemplate->noteMappings()->setClefOffset(curClef, clefOffset);
      markTemplateDirty(curTemplate, true);
      }
      
void StaffTypeTemplates::setShowAccidental(bool val)
      {
      curTemplate->noteMappings()->setShowAccidentals(val);
      markTemplateDirty(curTemplate, true);
      }
      
void StaffTypeTemplates::setOctaveDistance(int val)
      {
      curTemplate->noteMappings()->setOctaveDistance(val);
      markTemplateDirty(curTemplate, true);
      }
      
int StaffTypeTemplates::noteheadIndex(NoteHead::Group group) const
      {
      for (int i = 0; i < 14; i++) {
            if (noteheadLookup[i] == group)
                  return i;
            }
      return -1;
      }

int StaffTypeTemplates::clefIndex(ClefType ct) const
      {
      for (int i = 0; i < 17; i++) {
            if (clefLookup[i] == ct)
                  return i;
            }
      return -1;
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
