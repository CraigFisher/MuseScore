//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2002-2014 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#ifndef __INNERLEDGERWIDGET_H__
#define __INNERLEDGERWIDGET_H__

#include <QTableView>

//cc
namespace Ms {

//---------------------------------------------------------
//   InnerLedgerWidget
//---------------------------------------------------------

class InnerLedgerWidget : public QWidget {
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
      
     std::vector<qreal> parseLedgers(const QString* originalStr, QString* correctedStr);
     void setColumnParameters();
      
   private slots:
     void addLedgerMapping();
     void deleteLedgerMappings();
     void updateInnerLedgers();
      
   signals:
     void innerLedgersChanged(std::map<qreal, std::vector<qreal>>*);
     };

//---------------------------------------------------------
//   LedgerItemDelegate
//---------------------------------------------------------

class LedgerItemDelegate : public QItemDelegate {
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

}  // namespace Ms

#endif
