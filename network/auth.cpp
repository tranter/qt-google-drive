#include "auth.h"

Auth::Auth(QObject *parent) :
    NetworkManager(parent)
{
}

QUrl Auth::getOAuth2CodeUrl(const QString &scope, const QString &redirectUri, const QString &clientId, bool accessType, bool approvalPrompt, const QString &state)
{
    QString urlStr(QString("https://accounts.google.com/o/oauth2/auth?scope=%1&redirect_uri=%2&response_type=code&client_id=%3&access_type=%4&approval_prompt=%5&state=%6")
                   .arg(scope)
                   .arg(redirectUri)
                   .arg(clientId)
                   .arg(accessType ? QString("online") : QString("offline"))
                   .arg(approvalPrompt ? QString("force") : QString("auto"))
                   .arg(state));

    QUrl url;
    QList<QPair<QString, QString> > query;

    //url.setHost();

    return QUrl(urlStr);
}
