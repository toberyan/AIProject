// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "serverthread.h"
#include "parserimpl.h"

#include <QString>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ParserImpl impl;

    ServerThread th(&impl);
    th.start();

    return a.exec();
}
