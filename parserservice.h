// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef PARSERSERVICE_H
#define PARSERSERVICE_H

#include "parser.pb.h"
#include "parser.grpc.pb.h"

#include <QObject>

class ParserService : public Parser::Service
{
public:
    explicit ParserService(QObject *parent);
    ::grpc::Status Ping(::grpc::ServerContext* context, const ::PingReq* request, ::PingRes* response) override;
protected:
    QObject *proxy = nullptr;
};

#endif // PARSERSERVICE_H
