#ifndef NOTATIONPANEL_H
#define NOTATIONPANEL_H

#include "ui_notationpanel.h"

namespace Ui {
class NotationPanel;
}

namespace Ms {

class NotationPanel : public QWidget, private Ui::NotationPanel {
	Q_OBJECT

      virtual void closeEvent(QCloseEvent*);
      
    public:
       NotationPanel(QWidget *parent = 0);
       ~NotationPanel();

    private:
    	 Ui::NotationPanel *ui;

    signals:
       void closed(bool);

    private slots:
      void loadNotation();
      void removeNotation();
      void changeNotation(QListWidgetItem* item);
};

}

#endif // NOTATIONPANEL_H
