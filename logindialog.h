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
    void setLoginUrl(const QString& url);
    QString code();

signals:
    void signalAccessTokenObtained();
    void signalCodeObtained();

private slots:
    void urlChanged(const QUrl& url);

private:
    Ui::LoginDialog *ui;
    QString accessToken;
    QString codeStr;
};

#endif // LOGINDIALOG_H
