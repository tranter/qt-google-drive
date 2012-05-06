#include "resmanager.h"
#include <QDebug>

ResManager::ResManager()
{
    setResources();
}

QString ResManager::getResPath(const QString& type)
{
  QString resPath(UNKNOWN_TYPE_STR);

  if(resources.contains(type))
      resPath = resources.find(type).value();

  return resPath;
}

void ResManager::setResources()
{
    // You can add your type here to associate resourse name with resourse path
    resources[FOLDER_TYPE_STR] = ":ico/folder";
}
