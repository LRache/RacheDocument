//
// Created by Rache on 2021/10/30.
//

#include <QCoreApplication>
#include <QIcon>
#include <QFile>
#include <QTranslator>
#include <QSettings>

#include "globals.h"
#include "ToolFunction.h"

#define SettingVersion "2.01"

QString NormalStyleSheet;
QString DoubtStyleSheet;
QString AttentionStyleSheet;
QString WarningStyleSheet;
QIcon EditIcon;
QIcon CopyIcon;
QIcon PasteIcon;
QIcon UpIcon;
QIcon DownIcon;
QIcon DeleteIcon;
QIcon AddIcon;
QIcon ImageIcon;
QIcon TextIcon;
QIcon OtherIcon;
QIcon TreeIcon;
QIcon FindIcon;
QIcon AudioIcon;
QIcon VideoIcon;
QIcon PlayIcon;
QIcon PauseIcon;
QIcon ResourceIcon;
QIcon ListIcon;
QIcon AppIcon;

QString ItemTypeNames[4];
QString FindTypeNames[5];
QStringList FindTableHeader;
QStringList ResourceTypeTexts;

bool ShowToolButton;
bool AutoFromClipBoardSubFileName;
bool AutoFromClipBoardItemText;
QString LanguageName;

int SinglePageItemCount;

void init_stylesheet() {
    QFile file1(RESOURCE_PATH(stylesheet/fileitem_normal.txt));
    QFile file2(RESOURCE_PATH(stylesheet/fileitem_doubt.txt));
    QFile file3(RESOURCE_PATH(stylesheet/fileitem_attention.txt));
    QFile file4(RESOURCE_PATH(stylesheet/fileitem_warning.txt));

    file1.open(QFile::ReadOnly);
    file2.open(QFile::ReadOnly);
    file3.open(QFile::ReadOnly);
    file4.open(QFile::ReadOnly);

    NormalStyleSheet = file1.readAll();
    DoubtStyleSheet = file2.readAll();
    AttentionStyleSheet = file3.readAll();
    WarningStyleSheet = file4.readAll();

    file1.close();
    file2.close();
    file3.close();
    file4.close();
}

void init_icon() {
    EditIcon    = QIcon(RESOURCE_PATH(icon/edit.png));
    CopyIcon    = QIcon(RESOURCE_PATH(icon/copy.png));
    PasteIcon   = QIcon(RESOURCE_PATH(icon/paste.png));
    UpIcon      = QIcon(RESOURCE_PATH(icon/up.png));
    DownIcon    = QIcon(RESOURCE_PATH(icon/down.png));
    DeleteIcon  = QIcon(RESOURCE_PATH(icon/delete.png));
    AddIcon     = QIcon(RESOURCE_PATH(icon/add.png));
    ImageIcon   = QIcon(RESOURCE_PATH(icon/image.png));
    TextIcon    = QIcon(RESOURCE_PATH(icon/text.png));
    OtherIcon   = QIcon(RESOURCE_PATH(icon/other.png));
    TreeIcon    = QIcon(RESOURCE_PATH(icon/tree.png));
    FindIcon    = QIcon(RESOURCE_PATH(icon/find.png));
    AudioIcon   = QIcon(RESOURCE_PATH(icon/audio.png));
    VideoIcon   = QIcon(RESOURCE_PATH(icon/video.png));
    PlayIcon    = QIcon(RESOURCE_PATH(icon/play.png));
    PauseIcon   = QIcon(RESOURCE_PATH(icon/pause.png));
    ResourceIcon= QIcon(RESOURCE_PATH(icon/resource.png));
    ListIcon    = QIcon(RESOURCE_PATH(icon/list.png));
    AppIcon     = QIcon(RESOURCE_PATH(icon/Icon.ico));
}

void init_string() {
    FindTypeNames[0] = QObject::tr("全部");
    FindTypeNames[1] = QObject::tr("包含");
    FindTypeNames[2] = QObject::tr("等于");
    FindTypeNames[3] = QObject::tr("以……开头");
    FindTypeNames[4] = QObject::tr("以……结尾");

    ItemTypeNames[0] = QObject::tr("正常");
    ItemTypeNames[1] = QObject::tr("注意");
    ItemTypeNames[2] = QObject::tr("存疑");
    ItemTypeNames[3] = QObject::tr("警告");

    FindTableHeader.push_back(QObject::tr("子文件"));
    FindTableHeader.push_back(QObject::tr("组"));
    FindTableHeader.push_back(QObject::tr("项"));
    FindTableHeader.push_back(QObject::tr("内容"));
    FindTableHeader.push_back(QObject::tr("来源"));
    FindTableHeader.push_back(QObject::tr("类型"));
    FindTableHeader.push_back(QObject::tr("日期"));
    FindTableHeader.push_back(QObject::tr("资源"));

    ResourceTypeTexts.push_back(QObject::tr("图片"));
    ResourceTypeTexts.push_back(QObject::tr("文本"));
    ResourceTypeTexts.push_back(QObject::tr("音频"));
    ResourceTypeTexts.push_back(QObject::tr("视频"));
    ResourceTypeTexts.push_back(QObject::tr("其它"));
}

void load_translator(const QString& lang){
    static QTranslator* translator;
    if (translator != nullptr)
    {
        qApp->removeTranslator(translator);
        delete translator;
        translator = nullptr;
    }
    translator = new QTranslator;
    translator->load(RESOURCE_PATH(language/RacheFile_)+lang+".qm");
    qApp->installTranslator(translator);
}

void write_settings() {
    QSettings settings("./settings.ini", QSettings::IniFormat);

    settings.setValue("App/SettingVersion", SettingVersion);
    settings.setValue("App/LanguageName", LanguageName);
    settings.setValue("Edit/AutoFromClipBoardSubFileName", AutoFromClipBoardSubFileName);
    settings.setValue("Edit/AutoFromClipBoardItemText", AutoFromClipBoardItemText);
    settings.setValue("Viewer/ShowToolButton", ShowToolButton);
}

void load_settings() {
    if (is_file_exist("./settings.ini")) {
        QSettings settings("./settings.ini", QSettings::IniFormat);
        if (settings.value("App/SettingVersion").toString() == SettingVersion) {
            AutoFromClipBoardSubFileName    = settings.value("Edit/AutoFromClipBoardSubFileName").toBool();
            AutoFromClipBoardItemText       = settings.value("Edit/AutoFromClipBoardItemText").toBool();
            ShowToolButton                  = settings.value("Viewer/ShowToolButton").toBool();
            LanguageName                    = settings.value("App/LanguageName").toString();
            SinglePageItemCount             = 200;

            load_translator(LanguageName);
            return;
        }
     }
    AutoFromClipBoardSubFileName    = false;
    AutoFromClipBoardItemText       = false;
    ShowToolButton                  = true;
    LanguageName                    = "zh_CN";
    SinglePageItemCount             = 200;

    load_translator("zh_CN");

    write_settings();
}

void init_globals() {
    init_icon();
    init_stylesheet();
    init_string();
    load_settings();
}
