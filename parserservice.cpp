// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "parserservice.h"

#include <QMetaObject>

using namespace grpc;

ParserService::ParserService(QObject *parent) : proxy(parent)
{
    Q_ASSERT(parent);
}

Status ParserService::Ping(grpc::ServerContext *context, const PingReq *request, PingRes *response)
{
    QString ret;
    QMetaObject::invokeMethod(proxy, "ping", Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QString, ret), Q_ARG(QString, QString::fromStdString(request->text())));
    response->set_text(ret.toStdString());
    return Status();
}
