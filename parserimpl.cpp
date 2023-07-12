// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "parserimpl.h"


#include <QDebug>

void ParserImpl::initAiModule()
{
    Py_SetPythonHome((const wchar_t *)(L"/home/toberyan/anaconda3/envs/AImodule"));
    Py_Initialize();
    if(!Py_IsInitialized())
    {
        qDebug()<<"python module init error";
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    PyObject* pModule = PyImport_ImportModule("AImodule");

    if (!pModule)
         qDebug()<<"pModule create error!";

    pResultFunc= PyObject_GetAttrString(pModule,"get_result");


    if(!pResultFunc)
        qDebug()<<"get pModule get_result error!";

    pPara = PyTuple_New(1);

}

ParserImpl::ParserImpl(QObject *parent) : QObject(parent)
{
    initAiModule();
}

QString ParserImpl::ping(const QString &in)
{
//    qInfo() << "ping" << in;
//    return "ok";
    qDebug()<<"input str:"<<in;
    char *result = NULL;

    PyTuple_SetItem(pPara, 0, Py_BuildValue("s",in.toStdString().c_str()));

    if(initflag)
    {
        resultFuncBack = PyObject_CallObject(pResultFunc,pPara);
    }

    if(resultFuncBack)
    {
        PyObject* repr = PyObject_Repr(resultFuncBack);
        PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "strict");
        result = PyBytes_AsString(str);
        qDebug()<<"get_result: "<<result;
    }

    QString entityStr = QString::fromUtf8(result);
    entityStr.remove(0,1);
    entityStr.remove(entityStr.size() - 1, 1);

//    Py_Finalize();
    return entityStr;
}
