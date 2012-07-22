#ifndef DEF_H
#define DEF_H

#define GET_FOLDERS_TREE QString("https://docs.google.com/feeds/default/private/full/-/folder?showroot=true")
#define GET_FULL_ROOT_CONTENT QString("https://docs.google.com/feeds/default/private/full/folder%3Aroot/contents")
//#define GET_FOLDERS_ONLY_ROOT_CONTENT  QString("https://docs.google.com/feeds/default/private/full/folder%3Aroot/contents/-/folder")
#define GET_FILES_IN_FOLDER QString("https://docs.google.com/feeds/default/private/full/")
#define GET_ALL_ITEMS QString("https://docs.google.com/feeds/default/private/full")
#define GET_TRASH QString("https://docs.google.com/feeds/default/private/full/-/trashed?showfolders=true")
#define GET_OWNED_BY_ME QString("https://docs.google.com/feeds/default/private/full/-/mine")
#define GET_STARRED QString("https://docs.google.com/feeds/default/private/full/-/starred")
#define GET_USER_DOCUMENTS QString("https://docs.google.com/feeds/default/private/full/-/document")
#define GET_USER_PRESENTATIONS QString("https://docs.google.com/feeds/default/private/full/-/presentation")
#define GET_USER_SPREADSHEETS QString("https://docs.google.com/feeds/default/private/full/-/spreadsheet")

#define ROOT_TAG QString("https://docs.google.com/feeds/default/private/full/folder%3Aroot")
#define PARENT_TAG QString("http://schemas.google.com/docs/2007#parent")
#define UPLOAD_TAG QString("http://schemas.google.com/g/2005#resumable-create-media")
#define ICON_TAG QString("http://schemas.google.com/docs/2007#icon")
#define DELETE_FILE QString("https://docs.google.com/feeds/default/private/full/file:")
#define DELETE_FOLDER QString("https://docs.google.com/feeds/default/private/full/folder%3A")

#define OAUTH2_TOKEN_URL QString("https://accounts.google.com/o/oauth2/token")

#define ACCESS_TOKEN QString("access_token")
#define REFRESH_TOKEN QString("refresh_token")

#define CONTENTS QString("/contents")
#define MAX_RESULTS QString("?max-results=1000")

#define HIERARCHY_ATTRIBUTE_TAG(param) param("rel")
#define FYLE_TYPE_ATTRIBUTE_TAG(param) param("type")
#define FYLE_TYPE_SRC_ATTRIBUTE_TAG(param) param("src");
#define HIERARCHY_VALUE_TAG(param) param("href")
#define SELF_TAG QString("self")
#define FOLDER_TITLE_TAG QString("title")
#define FILE_TITLE_TAG QString("docs:filename")
#define FILE_SIZE_TAG QString("docs:size")
#define PUBLISHED_FILE_TAG QString("published")
#define UPDATED_FILE_TAG QString("updated")
#define EDITED_FILE_TAG QString("app:edited")
#define AUTHOR_TAG QString("author")
#define ENTRY_TAG QString("entry")

#define OWNER_ME QString("me")

#define FOLDER_TYPE 0
#define FILE_TYPE 1

#define UNKNOWN_TYPE_STR QString("unknown")
#define FILE_TYPE_STR QString("file")
#define FOLDER_TYPE_STR QString("folder")
#define CONTENT QString("content")

#define WORK_DIR QString("WorkDir")
#define INIT_LOAD QString("InitLoad")
#define LEFT_PANEL QString("left")
#define RIGHT_PANEL QString("right")

#define MAIN_TITLE QString("My drive")
#define ALL_ITEMS_TITLE QString("All items")
#define OWNED_BY_ME_TITLE QString("Owned by me")
#define TRASH_TITLE QString("Trash")
#define GET_STARRED_TITLE QString("Starred")
#define GET_USER_DOCUMENTS_TITLE QString("My Documents")
#define GET_USER_PRESENTATIONS_TITLE QString("My Presentations")
#define GET_USER_SPREADSHEETS_TITLE QString("My Spreadsheets")

#define SET_DIR_REMINDER_MSG QString("Please note: you must set working directory for downloading files")

#endif // DEF_H
