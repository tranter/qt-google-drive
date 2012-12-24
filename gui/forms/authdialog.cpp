#include "authdialog.h"
#include "ui_authdialog.h"
#include "share/debug.h"
#include "share/registration.h"
//#include "settings/settingsmanager.h"

AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    init();
}

AuthDialog::~AuthDialog()
{  
    DEBUG;
    delete ui;
}

void AuthDialog::init(void)
{
    ui->setupUi(this);

    auth.reset(new Auth);

    ui->webView->setUrl(auth->getOAuth2CodeUrl(SCOPE, REDIRECT_URI, CLIENT_ID, false, true));

    connect(ui->webView, SIGNAL(urlChanged(const QUrl&)), this, SLOT(slotUrlChanged(const QUrl&)));
    connect(auth.data(), SIGNAL(signalAuthResponse(const QString&, const QString&)), this, SLOT(slotAuthResponse(const QString&, const QString&)));
}

void AuthDialog::slotUrlChanged(const QUrl &url)
{
    QString code(auth->getOAuth2Code(url));

    if(!code.isEmpty())
    {
       auth->getTokens(code, CLIENT_ID, CLIENT_SECRET, REDIRECT_URI);
    }
}

void AuthDialog::slotAuthResponse(const QString &accessToken, const QString &refreshToken)
{
//    SettingsManager settingsManager;

//    settingsManager.setAccessToken(accessToken);
//    settingsManager.setRefreshToken(refreshToken);

    emit signalTokens(accessToken, refreshToken);

    QDialog::accept();
}
