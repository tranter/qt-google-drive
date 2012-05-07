#include "resmanager.h"
#include <QDebug>

ResManager::ResManager()
{
    setResources();
}

QString ResManager::getResPath(const QString& type)
{
  QString resPath(resources.find(UNKNOWN_TYPE_STR).value());

  if(resources.contains(type))
      resPath = resources.find(type).value();

  return resPath;
}

void ResManager::setResources()
{
    // You can add your type here to associate resourse name with resourse path
    resources[UNKNOWN_TYPE_STR] = ":ico/unknown";
    resources[FOLDER_TYPE_STR] = ":ico/folder";
}
