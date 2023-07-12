// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QObject>
#include <QThread>


class ServerThread : public QThread
{
    Q_OBJECT
public:
    explicit ServerThread(QObject *proxy);
protected:
    void run() override;
protected:
    QObject *proxy = nullptr;
};

#endif // SERVERTHREAD_H
