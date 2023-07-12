// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "serverthread.h"
#include "parserservice.h"

#include <grpcpp/grpcpp.h>

#include <QDebug>

using namespace grpc;

ServerThread::ServerThread(QObject *p) : proxy(p)
{

}

void ServerThread::run()
{
    std::string server_address("0.0.0.0:50001");

    ParserService service(proxy);

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    qInfo() << "Server listening on " << server_address.c_str();

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}
