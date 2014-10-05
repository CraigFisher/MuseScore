#include "notationpanel.h"
#include "musescore.h"

namespace Ms {

NotationPanel::NotationPanel(QWidget *parent) :
    QWidget(parent, Qt::Dialog),
    ui(new Ui::NotationPanel) {
    setupUi(this);
    connect(loadNotationButton, SIGNAL(clicked()), SLOT(loadNotation()));
//    connect(removeNotationButton, SIGNAL(clicked()), SLOT(removeNotation()));
//    connect(notationList, SIGNAL(itemClicked(QListWidgetItem* item)), SLOT(changeNotation(QListWidgetItem* item)));
	}

NotationPanel::~NotationPanel() {
      delete ui;
	}

void NotationPanel::loadNotation() {
      qDebug("loadNotation");
      QString fn = mscore->getNotationFilename(true);
      if (fn.isEmpty())
            return;


	}

void NotationPanel::removeNotation() {

	qDebug("removeNotation");

	}

void NotationPanel::changeNotation(QListWidgetItem* item) {

	qDebug("changeNotation");

	}
      
void NotationPanel::closeEvent(QCloseEvent* ev) {
      emit closed(false);
      QWidget::closeEvent(ev);
      }

void MuseScore::showNotationPanel(bool visible) {
      if (notationPanel == 0) {
            if (!visible)
                  return;
            notationPanel = new NotationPanel(this);
            
            // THIS STRATEGY CAN BE USED TO SET THINGS OUTSIDE NotationPanel OBJECT
//            connect(notationPanel, SIGNAL(somethingChanged(float)), otherNotationThing, SLOT(setSomething(float)));
            connect(notationPanel, SIGNAL(closed(bool)), notationId, SLOT(setChecked(bool)));
            }
      notationPanel->setVisible(visible);
      notationId->setChecked(visible);
      }
      
}
