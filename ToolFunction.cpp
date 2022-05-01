//
// Created by Rache on 2021/10/3.
//

#include "ToolFunction.h"

#include <QImageReader>
#include <QDebug>
#include <QBuffer>

#define BUFFER_SIZE 32768

bool is_file_exist(const QString& path)
{
    QFileInfo fileInfo(path);
    return fileInfo.isFile();
}

QString size_to_string(RBytesSize size) {
    if (size < 1024) {
        return QString::number(size) + "B";
    }
    auto s = (double) size;
    s = s / 1024;
    if (s < 1024) {
        return QString::number(s, 'f', 2) + "KB";
    }
    s = s / 1024;
    if (s < 1024) {
        return QString::number(s, 'f', 2) + "MB";
    }
    s = s / 1024;
    return QString::number(s, 'f', 2) + "GB";
}

QByteArray to_qByteArray(const RLib::RBytes& rBytes) {
    char cb[BUFFER_SIZE];
    RBytesSize seek = 0;
    RBytesSize l = BUFFER_SIZE;
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    while (l == BUFFER_SIZE) {
        l = rBytes.read(cb, seek, BUFFER_SIZE);
        seek += BUFFER_SIZE;
        buffer.write(cb,l);
    }
    buffer.close();
    return byteArray;
}

QString time_to_string(int s) {
    int _s = s % 60;
    if (_s < 10) return QString::number(s / 60) + ":0" + QString::number(_s);
    else return QString::number(s / 60) + ":" + QString::number(_s);
}
