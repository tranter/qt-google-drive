#include "logindialog.h"
#include "ui_logindialog.h"
#include "share/debug.h"
#include <QWebView>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    DEBUG;

    ui->setupUi(this);
    connect(ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(urlChanged(QUrl)));
}

LoginDialog::~LoginDialog()
{
    DEBUG;
    delete ui;
}

void LoginDialog::urlChanged(const QUrl &url)
{
    DEBUG;

    QString str = url.toString();

    if(str.indexOf("code=") != -1)
    {
        QStringList parseStrs = str.split("?");
        QStringList exp = parseStrs[1].split("&");

        for (int i = 0; i < exp.count(); ++i)
        {
            QStringList token = exp[i].split("=");

            if (token[0] == "code")
            {
                codeStr = token[1];
                emit signalCodeObtained();
                QDialog::accept();
                break;
            }
        }
    }
}

QString LoginDialog::code()
{
    DEBUG;
    return codeStr;
}

void LoginDialog::setLoginUrl(const QString& url)
{
    DEBUG;
    ui->webView->setUrl(url);
}
