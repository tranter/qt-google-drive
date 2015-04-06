#include "authdialog.h"
#include "ui_authdialog.h"
#include "share/debug.h"
#include "share/defs.h"
#include "share/registration.h"
#include "settings/settingsmanager.h"

AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    init();
}

AuthDialog::~AuthDialog()
{  
    delete ui;
}

void AuthDialog::init(void)
{
    ui->setupUi(this);

    auth.reset(new Auth);

    SettingsManager settingsManager;

    QString id, secret, uri;
    id =  settingsManager.getValueFromGroup(COMMON_GROUP, CLIENT_ID_KEY, "").toString();
    secret =  settingsManager.getValueFromGroup(COMMON_GROUP, CLIENT_SECRET_KEY, "").toString();
    uri =  settingsManager.getValueFromGroup(COMMON_GROUP, REDIRECT_URI_KEY, "").toString();


    ui->webView->setUrl(auth->getOAuth2CodeUrl(SCOPE, uri, id, false, true));

    connect(ui->webView, SIGNAL(urlChanged(const QUrl&)), this, SLOT(slotUrlChanged(const QUrl&)));
    connect(auth.data(), SIGNAL(signalAuthResponse(const QString&, const QString&)), this, SLOT(slotAuthResponse(const QString&, const QString&)));
}

void AuthDialog::slotUrlChanged(const QUrl &url)
{
    QString code(auth->getOAuth2Code(url));

    SettingsManager settingsManager;

    QString id, secret, uri;
    id =  settingsManager.getValueFromGroup(COMMON_GROUP, CLIENT_ID_KEY, "").toString();
    secret =  settingsManager.getValueFromGroup(COMMON_GROUP, CLIENT_SECRET_KEY, "").toString();
    uri =  settingsManager.getValueFromGroup(COMMON_GROUP, REDIRECT_URI_KEY, "").toString();

    if(!code.isEmpty())
    {
       auth->getTokens(code, id, secret, uri);
    }
}

void AuthDialog::slotAuthResponse(const QString &accessToken, const QString &refreshToken)
{
    emit signalTokens(accessToken, refreshToken);
    QDialog::accept();
}
