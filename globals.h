//
// Created by Rache on 2021/10/2.
//

#ifndef RACHEFILE_GLOBALS_H
#define RACHEFILE_GLOBALS_H

// #define CMAKE

#ifdef CMAKE
#define RESOURCE_PATH(name) "D://C++Project//RacheFile//resource//"#name
#else
#define RESOURCE_PATH(name) ".//resource//"#name
#endif

#define CSTR_to_QSTR(s) QString::fromUtf8(s.c_str())
#define CSTRtoUTF8(s) QString::fromUtf8(s.c_str())

#include <QString>
#include <QIcon>

extern QString NormalStyleSheet;
extern QString DoubtStyleSheet;
extern QString AttentionStyleSheet;
extern QString WarningStyleSheet;

extern QIcon EditIcon;
extern QIcon CopyIcon;
extern QIcon PasteIcon;
extern QIcon UpIcon;
extern QIcon DownIcon;
extern QIcon DeleteIcon;
extern QIcon AddIcon;
extern QIcon ImageIcon;
extern QIcon TextIcon;
extern QIcon OtherIcon;
extern QIcon TreeIcon;
extern QIcon FindIcon;
extern QIcon AudioIcon;
extern QIcon VideoIcon;
extern QIcon PlayIcon;
extern QIcon PauseIcon;
extern QIcon ListIcon;
extern QIcon AppIcon;
extern QIcon ResourceIcon;

extern QString ItemTypeNames[4];
extern QString FindTypeNames[5];
extern QStringList FindTableHeader;
extern QStringList ResourceTypeTexts;

extern bool ShowToolButton;
extern bool AutoFromClipBoardSubFileName;
extern bool AutoFromClipBoardItemText;
extern QString LanguageName;

extern int SinglePageItemCount;

#endif //RACHEFILE_GLOBALS_H
