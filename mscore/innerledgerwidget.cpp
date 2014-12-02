//=============================================================================
//  MuseScore
//  Linux Music Score Editor
//  $Id: instrdialog.cpp 5580 2012-04-27 15:36:57Z wschweer $
//
//  Copyright (C) 2002-2009 Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================

#include "innerledgerwidget.h"

//cc
namespace Ms {

//---------------------------------------------------------
//   InnerLedgerWidget
//---------------------------------------------------------

InnerLedgerWidget::InnerLedgerWidget(QWidget *parent) :
      QWidget(parent), _table(0), _addButton(0), _deleteButton(0), _parent(parent)
{
      _table = new QTableView(parent);
      _table->verticalHeader()->hide();
      _table->setSelectionBehavior(QAbstractItemView::SelectRows);
      _table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      _addButton = new QPushButton("Add", parent);
      _deleteButton = new QPushButton("Delete", parent);
      
      LedgerItemDelegate* itemDelegate = new LedgerItemDelegate(_table);
      _table->setItemDelegate(itemDelegate);
      _table->setModel(&_model);

      connect(_addButton, SIGNAL(clicked()), this, SLOT(addLedgerMapping()));
      connect(_deleteButton, SIGNAL(clicked()), this, SLOT(deleteLedgerMappings()));
      connect(&_model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(updateInnerLedgers())); //TODO: MAKE SURE THIS WORKS
 
      QGridLayout* mainLayout = new QGridLayout(parent);
      mainLayout->addWidget(_table, 0, 0, 1, 2);
      mainLayout->addWidget(_addButton, 1, 0, 1, 1);
      mainLayout->addWidget(_deleteButton, 1, 1, 1, 1);
}

//---------------------------------------------------------
//   setColumnParameters
//---------------------------------------------------------

void InnerLedgerWidget::setColumnParameters()
      {
      _model.setHeaderData(0, Qt::Horizontal, "Position");
      _model.setHeaderData(1, Qt::Horizontal, "Ledgers");
      _table->setColumnWidth(0, _parent->width() * 0.473); //HACK
      _table->setColumnWidth(1, _parent->width() * 0.473);
      }

//---------------------------------------------------------
//   addLedgerMapping
//---------------------------------------------------------

void InnerLedgerWidget::addLedgerMapping()
      {
      QStandardItem* item1 = new QStandardItem; //TODO: MAKE SURE THESE ARE DESTROYED
      QStandardItem* item2 = new QStandardItem;

      //do not initialize any real data, it would conflict
      item1->setData("", Qt::EditRole);
      item2->setData("", Qt::EditRole);
      QList<QStandardItem*> row;
      row.append(item1);
      row.append(item2);
      _model.appendRow(row);
      setColumnParameters();
      }

//---------------------------------------------------------
//   deleteLedgerMappings
//---------------------------------------------------------

void InnerLedgerWidget::deleteLedgerMappings()
      {
      QModelIndexList rows = _table->selectionModel()->selectedRows(0);
      for (int i = rows.size() - 1; i >= 0; i--) {
            //remove row from model
            int row = rows.at(i).row();
            QList<QStandardItem*> colsToDelete = _model.takeRow(row);
            
            //delete its children items
            while (colsToDelete.isEmpty())
                  delete colsToDelete.takeFirst();
            emit updateInnerLedgers();
            }
      }

//---------------------------------------------------------
//   setData
//---------------------------------------------------------

void InnerLedgerWidget::setData(const std::map<qreal, std::vector<qreal>>& ledgerMap)
      {
      _model.clear();
      std::map<qreal, std::vector<qreal>>::const_iterator posItr = ledgerMap.begin();
      while (posItr != ledgerMap.end()) {
            qreal pos = posItr->first;
            QString ledgerString;

            std::vector<qreal>::const_iterator ledgerItr = posItr->second.begin();
            while (ledgerItr != posItr->second.end()) {
                  ledgerString.append(QString::number(*ledgerItr,'f', 2)).append(','); //TODO: UPDATE ALL THESE RELATED PRECISIONS TO '1' DECIMAL PRECISION
                  ledgerItr++;
                  }
            
            QStandardItem* item1 = new QStandardItem; //TODO: MAKE SURE THESE ARE DESTROYED
            QStandardItem* item2 = new QStandardItem;
            item1->setData(pos, Qt::EditRole);
            item2->setData(ledgerString, Qt::EditRole);
            QList<QStandardItem*> row;
            row.append(item1);
            row.append(item2);
            _model.appendRow(row);
            
            posItr++;
            }
      setColumnParameters();
      }

//---------------------------------------------------------
//   updateInnerLedgers
//---------------------------------------------------------

void InnerLedgerWidget::updateInnerLedgers()
      {
      std::map<qreal, std::vector<qreal>> ledgerMap;
      for (int i = 0; i < _model.rowCount(); i++) {
            QString originalStr = _model.item(i, 1)->data(Qt::EditRole).value<QString>();
            QString correctedStr;
            std::vector<qreal> ledgers = parseLedgers(&originalStr, &correctedStr);

            //if originalStr needs to be corrected, correct it
            if (originalStr != correctedStr) {
                  disconnect(&_model, SIGNAL(itemChanged(QStandardItem*)), 0, 0);
                  _model.setData(_model.item(i, 1)->index(), correctedStr, Qt::EditRole);
                  connect(&_model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(updateInnerLedgers()));
                  }
            
            if (_model.item(i, 0)->data(Qt::EditRole).value<QString>() != "" && !ledgers.empty()) {
                  qreal position = _model.item(i, 0)->data(Qt::EditRole).value<qreal>();
                  ledgerMap[position] = ledgers;
                  }
            }
      emit innerLedgersChanged(ledgerMap);
      }

//---------------------------------------------------------
//   parseLedgers
//---------------------------------------------------------

std::vector<qreal> InnerLedgerWidget::parseLedgers(const QString* originalStr, QString* correctedStr)
      {
      std::vector<qreal> ledgers;
      QStringList numberList = originalStr->split(",", QString::SkipEmptyParts);
      foreach (const QString& s, numberList) {
            bool ok;
            qreal next = s.toDouble(&ok);
            if (ok) {
                  qreal nearest = round(next * 100) / 100;
                  *correctedStr = correctedStr->append(QString::number(nearest,'f', 2)).append(',');
                  ledgers.push_back(nearest);
                  }
            else {
                  ledgers.clear();
                  *correctedStr = QString();
                  break;
                  }
            }
      return ledgers;
      }

//---------------------------------------------------------
//   LedgerItemDelegate
//---------------------------------------------------------

LedgerItemDelegate::LedgerItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    _state =  QStyle::State_Enabled; //TODO: STILL NECESSARY?
}

//---------------------------------------------------------
//   createEditor
//---------------------------------------------------------

QWidget* LedgerItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
      {
      if(index.column() % 2 == 0) {
            QDoubleSpinBox *spinner = new QDoubleSpinBox(parent);
            spinner->setDecimals(2);
            spinner->setSingleStep(0.5);
            spinner->setRange(-50.0, 50.0);
            spinner->installEventFilter(const_cast<LedgerItemDelegate*>(this));
            return spinner;
            }
      else {
            QLineEdit *editor = new QLineEdit(parent);
            editor->installEventFilter(const_cast<LedgerItemDelegate*>(this));
            return editor;
            }
      }

//---------------------------------------------------------
//   setEditorData
//---------------------------------------------------------

void LedgerItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
      {
      if(index.column() % 2 == 0) {
            QDoubleSpinBox* spinner = static_cast<QDoubleSpinBox*>(editor);
            spinner->setValue(index.data(Qt::EditRole).value<qreal>());
            }
      else {
            QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
            lineEdit->setText(index.data(Qt::EditRole).value<QString>());
            }
      }

//---------------------------------------------------------
//   setModelData
//---------------------------------------------------------

void LedgerItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
      {
      if(index.column() % 2 == 0) {
            QDoubleSpinBox* spinner = static_cast<QDoubleSpinBox*>(editor);
            QStandardItemModel* _model = static_cast<QStandardItemModel*>(model);
            qreal spinnerVal = spinner->value();

            //check if position is already mapped to ledgers
            int row = index.row();
            for (int i = 0; i < _model->rowCount(); i++) {
                  if (i == row)
                        continue;
                  qreal val = _model->item(i, 0)->data(Qt::EditRole).value<qreal>();
                  if (spinnerVal == val) {
                        QMessageBox::warning(0, QObject::tr("MuseScore: InnerLedgers Error"), "Line position already mapped.");
                        return;
                        }
                  }
            
            spinner->interpretText();
            model->setData(index, spinnerVal, Qt::EditRole);                              //save the new data
            }
      else {
            QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
            model->setData(index, lineEdit->text(), Qt::EditRole);
            }
      }

//---------------------------------------------------------
//   updateEditorGeometry
//---------------------------------------------------------

void LedgerItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
      {
      editor->setGeometry(option.rect);
      }

}
