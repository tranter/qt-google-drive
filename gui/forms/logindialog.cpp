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
    QString str = url.toString();

    DEBUG << "====================================>" << url;

    if(str.indexOf("code=") != -1)
    {
        QStringList parseStrs = str.split("?");
        QStringList exp = parseStrs[1].split("&");

        for (int i = 0; i < exp.count(); ++i)
        {
            QStringList token = exp[i].split("=");

            if (token[0] == "code")
            {
                emit signalCodeObtained(token[1]);
                QDialog::accept();
                break;
            }
        }
    }
}

void LoginDialog::setLoginUrl(const QUrl& url)
{
    ui->webView->setUrl(url);
}
