#include "logindialog.h"
#include "ui_logindialog.h"
#include "share/debug.h"
#include <QWebView>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(urlChanged(QUrl)));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::urlChanged(const QUrl &url)
{
    QString code(url.queryItemValue("code"));

    if(!code.isEmpty())
    {
        emit signalCodeObtained(code);
        QDialog::accept();
    }
}

void LoginDialog::setLoginUrl(const QUrl& url)
{
    ui->webView->setUrl(url);
}
