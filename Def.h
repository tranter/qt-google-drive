#ifndef DEF_H
#define DEF_H

#define ROOT_FOLDER "https://docs.google.com/feeds/default/private/full/folder%3Aroot"
#define PARENT_FOLDER "http://schemas.google.com/docs/2007#parent"
#define HIERARCHY_ATTRIBUTE attribs.value("rel")
#define SELF_TAG "self"
#define HIERARCHY_VALUE attribs.value("href")
#define TITLE_TAG "title"
#define GET_FOLDERS QString("https://docs.google.com/feeds/default/private/full/-/folder?showroot=true")
#define GET_FILES QString("https://docs.google.com/feeds/default/private/full")
#define FOLDERS 0
#define FILES 1

#endif // DEF_H
