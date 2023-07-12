// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef PARSERIMPL_H
#define PARSERIMPL_H

#include <QObject>
#undef slots
#include <Python.h>
#define slots Q_SLOTS


class ParserImpl : public QObject
{
    Q_OBJECT
private:
    void initAiModule();
public:
    explicit ParserImpl(QObject *parent = nullptr);

signals:

public slots:
    QString ping(const QString &in);

private:
    PyObject *pResultFunc = NULL;
    PyObject *pPara = NULL;
    PyObject *resultFuncBack = NULL;
    bool initflag = true;
};

#endif // PARSERIMPL_H
