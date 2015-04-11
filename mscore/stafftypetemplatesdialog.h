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

#ifndef STAFFTYPETEMPLATESDIALOG_H
#define STAFFTYPETEMPLATESDIALOG_H

#include "ui_stafftypetemplatesdialog.h"
#include "libmscore/stafftype.h"
#include "innerledgerwidget.h"
#include <QTableView>

//cc
namespace Ui {
class StaffTypeTemplatesDialog;
}

namespace Ms {

//TODO: HANDLE NECESSARY DELETIONS IN A DESTRUCTOR
class StaffTypeTemplatesDialog : public QDialog, private Ui::StaffTypeTemplatesDialog {
	Q_OBJECT
      
    public:
      StaffTypeTemplatesDialog(QWidget *parent = 0);
      ~StaffTypeTemplatesDialog();

    private:
      Ui::StaffTypeTemplatesDialog *ui;
      InnerLedgerWidget* innerLedgerWidget;
      
      mutable bool inputEnabled = true;
      int newTemplateNameIndex = 0;
      StaffTypeTemplate* curTemplate;
      std::vector<StaffTypeTemplate> localTemplates; //local copy of userTemplates
      Score* previewScore;
      
      QPixmap* doubleFlatColorIcon;
      QPixmap* flatColorIcon;
      QPixmap* naturalColorIcon;
      QPixmap* sharpColorIcon;
      QPixmap* doubleSharpColorIcon;
      
      void setValues() const;
      void enableInput(bool) const;
      void connectInput() const;
      void disconnectInput() const;
      void initColorButtons();
      
      void markTemplateDirty(StaffTypeTemplate* stt, bool val);
      StaffTypeTemplate* templateByItem(QListWidgetItem*);
      QListWidgetItem* itemByTemplate(StaffTypeTemplate*);

      static int customColorIdx;
      static QVector<QColor> colorHistory;

      const static int tpcLookup[7][5];
      const static NoteHead::Group noteheadLookup[14];
      const static NoteMappings::FillType filltypeLookup[3];
      const static ClefType clefLookup[17];
      
      int noteLetterIdx; //index of a note's letter, used in tpcLookup
      int clefIdx;       //index of a clef, used in clefLookup
      int noteheadIndex(NoteHead::Group) const;
      int clefIndex(ClefType) const;
      int filltypeIndex(NoteMappings::FillType) const;
      void debugLocals();
      
      void updatePreview() const;
      void updateColorHistory() const;

    signals:
      void closed(bool);

    private slots:
      void load();
      void create();
      void remove();
      void duplicate();
      bool save();
      bool save(StaffTypeTemplate* stt);
      void handleExitButton();
      void handleTemplateSwitch(int);
      
      void switchNoteLetter(const QString&);
      void switchClef(const QString&);
      
      void setShowAccidental(bool);
      void setOctaveDistance(int);
      void setClefOffset(int clefOffset);
      void setOffset(int idx, int offset);
      void setNotehead(int idx, int headIdx);
      void setFilltype(int idx, int fillIdx);
      void pickNoteColor(int idx);
      
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
      
      void setDoubleFlatFilltype(int fillIdx) { setFilltype(0, fillIdx); }
      void setFlatFilltype(int fillIdx) { setFilltype(1, fillIdx); }
      void setNaturalFilltype(int fillIdx) { setFilltype(2, fillIdx); }
      void setSharpFilltype(int fillIdx) { setFilltype(3, fillIdx); }
      void setDoubleSharpFilltype(int fillIdx) { setFilltype(4, fillIdx); }

      void pickDoubleFlatColor() { pickNoteColor(0); }
      void pickFlatColor() { pickNoteColor(1); }
      void pickNaturalColor() { pickNoteColor(2); }
      void pickSharpColor() { pickNoteColor(3); }
      void pickDoubleSharpColor() { pickNoteColor(4); }
      
      void setInnerLedgers(std::map<qreal, std::vector<qreal>>&);
      void updateStaffLines();
      void updateTemplateName(const QString&);
      };

}     // namespace Ms

#endif   // STAFFTYPETEMPLATESDIALOG_H
