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
    void setLoginUrl(const QString& url);
    QString getAccessToken() const;

signals:
    void accessTokenObtained();

private slots:
    void urlChanged(const QUrl& url);


private:
    Ui::LoginDialog *ui;
    QString accessToken;
};

#endif // LOGINDIALOG_H
