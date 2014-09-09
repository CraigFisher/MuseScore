#include "notationpanel.h"
#include "ui_notationpanel.h"

NotationPanel::NotationPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotationPanel)
{
    ui->setupUi(this);
}

NotationPanel::~NotationPanel()
{
    delete ui;
}
