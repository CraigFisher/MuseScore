#ifndef NOTATIONPANEL_H
#define NOTATIONPANEL_H

#include <QDialog>

namespace Ui {
class NotationPanel;
}

class NotationPanel : public QDialog
{
    Q_OBJECT

public:
    explicit NotationPanel(QWidget *parent = 0);
    ~NotationPanel();

private:
    Ui::NotationPanel *ui;
};

#endif // NOTATIONPANEL_H
