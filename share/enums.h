#ifndef ENUMS_H
#define ENUMS_H

enum EPanels
{
    EUnknown = -1,
    ELeft,
    ERight,
    EPanelsCount
};

enum EOperations
{
    ECopy,
    EMove,
    EDelete,
    ECreate,
    ERename,
    EDownload,
    EUpload,
    EChildren
};

enum EPanelState
{
    EWeb,
    EComputer,
    EPanelStateCount
};

#endif // ENUMS_H
