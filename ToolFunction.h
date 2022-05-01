//
// Created by Rache on 2021/10/3.
//

#ifndef RACHEFILE_TOOLFUNCTION_H
#define RACHEFILE_TOOLFUNCTION_H

#include <QFileInfo>
#include <QByteArray>

#include "RacheFile/FileDocument.h"
#include "RacheFile/ResourceSet.h"

bool is_file_exist(const QString& path);

QByteArray to_qByteArray(const RLib::RBytes& rBytes);

QString size_to_string(RBytesSize size);

QString time_to_string(int sec);

#endif //RACHEFILE_TOOLFUNCTION_H
