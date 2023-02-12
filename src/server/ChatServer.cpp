//
// Created by Matthew Crowell on 2/12/2023.
//

#include "src/server/ChatServer.h"
#include <QtConcurrent>
#include <QTcpSocket>

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
        connect(m_server.get(), &QTcpServer::newConnection, this, [this]() -> void
        {
            while (m_server->hasPendingConnections())
            {
                QTcpSocket *socket{m_server->nextPendingConnection()};

                auto results{QtConcurrent::run([socket]()
                                               {
                                                   if (socket->waitForReadyRead())
                                                   {
                                                       qDebug() << "Received:" << socket->readAll();
                                                   }
                                               })};
            }
        });
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
}