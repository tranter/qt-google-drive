#ifndef DEF_H
#define DEF_H

#define ROOT_FOLDER "https://docs.google.com/feeds/default/private/full/folder%3Aroot"
#define PARENT_FOLDER "http://schemas.google.com/docs/2007#parent"
#define HIERARCHY_ATTRIBUTE attribs.value("rel")
#define SELF_TAG "self"
#define HIERARCHY_VALUE attribs.value("href")

#define FYLE_TYPE_ATTRIBUTE attribs.value("type")
#define FYLE_TYPE_SRC_ATTRIBUTE attribs.value("src");

#define FOLDER_TITLE_TAG "title"
#define FILE_TITLE_TAG "docs:filename"
#define FILE_SIZE_TAG "docs:size"
#define PUBLISHED_FILE_TAG "published"
#define UPDATED_FILE_TAG "updated"
#define EDITED_FILE_TAG "app:edited"

#define GET_FOLDERS QString("https://docs.google.com/feeds/default/private/full/-/folder?showroot=true")
#define GET_FILES QString("https://docs.google.com/feeds/default/private/full")
#define UPLOAD QString("http://schemas.google.com/g/2005#resumable-create-media")

#define FOLDER_TYPE 0
#define FILE_TYPE 1

#define UNKNOWN_TYPE_STR "unknown"
#define FILE_TYPE_STR "file"
#define FOLDER_TYPE_STR "folder"

#define TREE_VIEW_MAIN_TITLE "My Disc"
#define TREE_VIEW_SIZE_TITLE "Size"
#define TREE_VIEW_PUBLISHED_TITLE "Published"
#define TREE_VIEW_UPDATED_TITLE "Updated"
#define TREE_VIEW_EDITED_TITLE "Edited"

#define ADDITIONAL_INFO_KEY "additional info state"

// to add this token in the beginning of string means that images will not be drawing
#define INFO_TOKEN '|'

#define CONTENT "content"

#define WORK_DIR QString("WorkDir")

#define SET_DIR_REMINDER_MSG QString("Please note: you must set working directory for downloading files")

#endif // DEF_H
