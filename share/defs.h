#ifndef DEF_H
#define DEF_H

#define GET_FULL_ROOT_CONTENT QString("https://docs.google.com/feeds/default/private/full/folder%3Aroot/contents")
#define GET_FILES_IN_FOLDER QString("https://docs.google.com/feeds/default/private/full/")
#define ROOT_TAG QString("https://docs.google.com/feeds/default/private/full/folder%3Aroot")
#define PARENT_TAG QString("http://schemas.google.com/docs/2007#parent")
#define UPLOAD_TAG QString("http://schemas.google.com/g/2005#resumable-create-media")
#define ICON_TAG QString("http://schemas.google.com/docs/2007#icon")
#define DELETE_FILE QString("https://docs.google.com/feeds/default/private/full/file:")
#define CREATE_FOLDER QString("https://www.googleapis.com/drive/v2/files")
#define COPY_FILE QString("https://docs.google.com/feeds/default/private/full/document:")
#define COPY_FILE_FIRST_QUERY_PART QString("https://www.googleapis.com/drive/v2/files/")
#define COPY_FILE_LAST_QUERY_PART QString("/copy")

#define CONTENTS QString("/contents")
#define MAX_RESULTS QString("?max-results=10000")

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

#define FOLDER_TYPE 0
#define FILE_TYPE 1

#define FOLDER_TYPE_STR QString("folder")
#define FILE_TYPE_STR QString("file")
#define CONTENT QString("content")

#define ACCOUNTS_GROUP QString("accounts")
#define COMMON_GROUP QString("common")
#define PANEL_GROUP QString("panel-")
#define ACCESS_TOKEN_KEY QString("access_token")
#define REFRESH_TOKEN_KEY QString("refresh_token")
#define WORK_DIR_KEY QString("working directory")
#define INIT_LOAD_KEY QString("initial load")
#define CURRENT_PANEL_KEY QString("current panel")
#define CURRENT_FOLDER_URL_KEY QString("current folder URL")
#define CURRENT_FOLDER_PATH_KEY QString("current folder path")
#define PATHES_URLS_KEY QString("pathes URLs")
#define LEFT_PANEL_VALUE QString("left")
#define RIGHT_PANEL_VALUE QString("right")

#define PARENT_FOLDER_SIGN QString("[..]")

#endif // DEF_H
