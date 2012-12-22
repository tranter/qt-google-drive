#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QString>
#include <QUrl>

namespace Ui {
    class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

public:
    void setLoginUrl(const QUrl &url);

signals:
    void signalCodeObtained(const QString &code);

private slots:
    void urlChanged(const QUrl& url);

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
