#ifndef CREATEFOLDERDIALOG_H
#define CREATEFOLDERDIALOG_H

#include <QDialog>

namespace Ui {
class CreateFolderDialog;
}

class CreateFolderDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CreateFolderDialog(QWidget *parent = 0);
    ~CreateFolderDialog();

public slots:
    void slotAccept();
    void slotReject();

signals:
    void signalAccept(const QString& name);
    void signalReject();

private:
    Ui::CreateFolderDialog *ui;
};

#endif // CREATEFOLDERDIALOG_H
