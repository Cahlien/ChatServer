//
// Created by Matthew Crowell on 2/12/2023.
//

#include "src/server/ChatServer.h"
#include <QtConcurrent>
#include "src/client/ChatClient.h"
#include "src/worker/SocketReader.h"

/*!
 * \class dc::ChatServer
 * \inmodule ChatServer
 * \brief The ChatServer manages TCP connections for the application.
 */

namespace dc
{
    ChatServer::ChatServer(QObject *parent)
            : QObject{parent}
    {
        connect(m_server.get(), &QTcpServer::pendingConnectionAvailable, this, &ChatServer::onPendingConnectionAvailable);
    }

    void ChatServer::start(quint16 port)
    {
        if (m_server->listen(QHostAddress::Any, port))
        {
            qInfo() << "Server started on port" << port;
        }
        else
        {
            qCritical() << "Server failed to start on port" << port;
        }
    }

    void ChatServer::stop()
    {
        m_server->close();

        qInfo() << "Server stopped";
    }

    void ChatServer::onPendingConnectionAvailable()
    {
        m_clients.emplace_back(nullptr, m_server.get());
        connect(&m_clients.back(), &ChatClient::readyRead, this, &ChatServer::onReadyRead);
    }

    void ChatServer::onReadyRead()
    {
        auto result2{QtConcurrent::run(&SocketReader::run, new SocketReader{nullptr, qobject_cast<ChatClient*>(sender())->socket()})};
    }
}