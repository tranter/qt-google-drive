#ifndef OAUTH2_H
#define OAUTH2_H

#include <QString>
#include <QObject>

class LoginDialog;

class OAuth2 : public QObject
{
    Q_OBJECT

public:
    OAuth2(QWidget* parent = 0);
    QString getAccessToken() const;
    bool isAuthorized();
    void startLogin(bool bForce);

    //Functions to set application's details.
    void setScope(const QString& scopeStr);
    void setClientID(const QString& clientIDStr);
    void setRedirectURI(const QString& redirectURIStr);
    void setCompanyName(const QString& companyNameStr);
    void setAppName(const QString& appNameStr);

    QString loginUrl();

signals:
    //Signal that is emitted when login is ended OK.
    void loginDone();

private slots:
    void accessTokenObtained();

private:
    QString accessToken;

    QString endPoint;
    QString scope;
    QString clientID;
    QString redirectURI;
    QString responseType;

    QString companyName;
    QString appName;

    LoginDialog* loginDialog;
    QWidget* parent;
};

#endif // OAUTH2_H
