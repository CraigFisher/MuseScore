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
#include <QTableView>

//cc
namespace Ui {
class StaffTypeTemplates;
}

namespace Ms {


//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
      //TODO: SEPARATE HEADER FILE
//////////////////////////////////
//////////////////////////////////
//////////////////////////////////

//---------------------------------------------------------
//   InnerLedgerWidget
//---------------------------------------------------------

class InnerLedgerWidget : public QWidget
{
      Q_OBJECT
    public:
      InnerLedgerWidget(QWidget *parent = 0);
      void setData(const std::map<qreal, std::vector<qreal>>*);
 
    private:
      QStandardItemModel _model;
      QTableView*  _table;
      QPushButton* _addButton;
      QPushButton* _deleteButton;
      QWidget* _parent;
      
      std::vector<qreal> parseLedgers(QString);
      void setColumnParameters();
      
    private slots:
      void addLedgerMapping();
      void deleteLedgerMappings();
      void updateInnerLedgers(QStandardItem*);
      
    signals:
      void innerLedgersChanged(qreal, std::vector<qreal>*);
};

//---------------------------------------------------------
//   LedgerItemDelegate
//---------------------------------------------------------

class LedgerItemDelegate : public QItemDelegate
{
      Q_OBJECT
    public:
      explicit LedgerItemDelegate(QObject *parent = 0);

      virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const;
      
      void setEditorData(QWidget *editor, const QModelIndex &index) const;
      void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
      void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
 
    signals:
      void buttonClicked(const QModelIndex &index);

    private:
      QStyle::State  _state;
      QRect oldRect;
};

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
           //TODO: SEPARATE HEADER (THE ABOVE)
//////////////////////////////////
//////////////////////////////////
//////////////////////////////////



//TODO: HANDLE NECESSARY DELETIONS IN A DESTRUCTOR
class StaffTypeTemplates : public QDialog, private Ui::StaffTypeTemplates {
	Q_OBJECT
      
    public:
      StaffTypeTemplates(QWidget *parent = 0);
      ~StaffTypeTemplates();

    private:
      Ui::StaffTypeTemplates *ui;
      InnerLedgerWidget* innerLedgerWidget;
      
      mutable bool inputEnabled = true;
      int newTemplateNameIndex = 0;
      StaffTypeTemplate* curTemplate;
      std::vector<StaffTypeTemplate> localTemplates; //local copy of userTemplates
      
      std::list<QStandardItem*> InnerLedgerWidgetItems;
      
      void setValues() const;
      void enableInput(bool) const;
      void connectInput() const;
      void disconnectInput() const;
      
      void markTemplateDirty(StaffTypeTemplate* stt, bool val);
      StaffTypeTemplate* templateByItem(QListWidgetItem*);
      QListWidgetItem* itemByTemplate(StaffTypeTemplate*);

      const static int tpcLookup[7][5];
      const static NoteHead::Group noteheadLookup[14];
      const static ClefType clefLookup[17];
      
      int noteLetterIdx; //index of a note's letter, used in tpcLookup
      int clefIdx; //index of a clef, used in clefLookup
      int noteheadIndex(NoteHead::Group) const;
      int clefIndex(ClefType) const;
      void debugLocals();

    signals:
      void closed(bool);

    private slots:
      void load();      //STUB
      void create();
      void remove();    //STUB
      void duplicate(); //STUB
      bool save();
      bool save(StaffTypeTemplate* stt);
      void handleExitButton();
      void handleTemplateSwitch(int);
      
      void switchNoteLetter(const QString& text);
      void switchClef(const QString& text);
      
      void setShowAccidental(bool);
      void setOctaveDistance(int);
      void setClefOffset(int clefOffset);
      void setOffset(int idx, int offset);
      void setNotehead(int idx, int headIdx);
      
      void setDoubleFlatOffset(int offset) { setOffset(0, offset); }
      void setFlatOffset(int offset) { setOffset(1, offset); }
      void setNaturalOffset(int offset) { setOffset(2, offset); }
      void setSharpOffset(int offset) { setOffset(3, offset); }
      void setDoubleSharpOffset(int offset) { setOffset(4, offset); }
      
      void setDoubleFlatNotehead(int headIdx) { setNotehead(0, headIdx); }
      void setFlatNotehead(int headIdx) { setNotehead(1, headIdx); }
      void setNaturalNotehead(int headIdx) { setNotehead(2, headIdx); }
      void setSharpNotehead(int headIdx) { setNotehead(3, headIdx); }
      void setDoubleSharpNotehead(int headIdx) { setNotehead(4, headIdx); }
      
      void setInnerLedgers(qreal pos, std::vector<qreal>* ledgers);
      
      //TODO: SET Notehead AND CLEFOFFSET
};

}

#endif // STAFFTYPETEMPLATES_H
