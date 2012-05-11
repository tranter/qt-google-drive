#include "commontools.h"
#include <QDebug>

QList<int> CommonTools::indexes;

void CommonTools::setHeader(QNetworkRequest& request)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    QString accessToken = settings.value("access_token").toString();

    request.setRawHeader("User-Agent", APP_NAME);
    request.setRawHeader("GData-Version", "3.0");
    request.setRawHeader("Authorization",(QString("OAuth %1").arg(accessToken)).toLatin1());
}

void CommonTools::msg(const QString& str)
{
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();
}

const QList<int> CommonTools::getTreeViewOpenedItem(void)
{
    return CommonTools::indexes;
}

void CommonTools::treeViewOpenedItemClear(void)
{
    CommonTools::indexes.clear();
}

void CommonTools::addTreeViewOpenedItem(int index)
{
    CommonTools::indexes.push_back(index);
    qDebug() << "addTreeViewOpenedItem ->" << QString::number(CommonTools::indexes.count());
}

void CommonTools::removeTreeViewOpenedItem(int index)
{
    CommonTools::indexes.removeAt(CommonTools::indexes.indexOf(index));
    qDebug() << "removeTreeViewOpenedItem ->" << QString::number(CommonTools::indexes.count());
}

QString CommonTools::convertDate(const QString& dt)
{
    QDateTime fileDateTime = QDateTime::fromString(dt, "yyyy-MM-ddThh:mm:ss.zzzZ");

    return fileDateTime.toString(/*"ddd, MMM dd, yyyy    hh:mm::ss"*/);
}
