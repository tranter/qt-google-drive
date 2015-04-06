#include "createfolderdialog.h"
#include "ui_createfolderdialog.h"
#include "share/commontools.h"

CreateFolderDialog::CreateFolderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateFolderDialog)
{
    ui->setupUi(this);
    ui->nameEdit->setFocus();

    //connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotAccept()));
    //connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(slotReject()));
    //connect(this, SIGNAL(finished(int)), this, SLOT(slotFinished(int)));
}

void CreateFolderDialog::accept()
{
    QString name = ui->nameEdit->text();
    if(name == "" || name.contains(QRegExp("[/.<>]")) || name.contains(QRegExp("\\\\")) || name.contains(QRegExp("\"")))
    {
        CommonTools::msg(tr("Please enter a valid name"));
        return;
    }
    QDialog::accept();
}

QString CreateFolderDialog::folderName() const
{
    return ui->nameEdit->text();
}

CreateFolderDialog::~CreateFolderDialog()
{
    delete ui;
}

//void CreateFolderDialog::slotAccept()
//{
//    emit signalAccept(ui->nameEdit->text());
//}

//void CreateFolderDialog::slotReject()
//{
//    emit signalReject();
//}

//void CreateFolderDialog::slotFinished(int result)
//{
//    emit signalFinished(result);
//}
