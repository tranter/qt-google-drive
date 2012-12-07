#include "sharedialog.h"
#include "ui_sharedialog.h"
#include <QUrl>
#include <QFile>

ShareDialog::ShareDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShareDialog)
{
    ui->setupUi(this);

    QFile file;
    file.setFileName(":/web/share");
    file.open(QIODevice::ReadOnly);
    QString html = file.readAll();
    file.close();

    ui->shareWebView->setHtml(html);
}

ShareDialog::~ShareDialog()
{
    delete ui;
}
