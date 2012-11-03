#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFileDialog>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(browseDir()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::browseDir()
{
    QString dir = QFileDialog::getExistingDirectory (this, "Set Directory", ui->directoryLineEdit->text());   
    if(dir.isEmpty()) return;
    ui->directoryLineEdit->setText(dir);
}

QString SettingsDialog::directoryPath() const
{
    QString str = ui->directoryLineEdit->text();
    str.replace("\\", "/");   
    return str;
}

void SettingsDialog::setDirectoryPath(const QString& dir)
{
    QString str = dir;
    str.replace("\\", "/");
    ui->directoryLineEdit->setText(str);
}
