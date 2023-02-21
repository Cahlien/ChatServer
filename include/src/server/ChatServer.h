//
// Created by Matthew Crowell on 2/12/2023.
//

#ifndef CHATSERVER_CHATSERVER_H
#define CHATSERVER_CHATSERVER_H

#include <QObject>
#include <QTcpServer>
#include <vector>

#include "src/client/ChatClient.h"

namespace dc
{
    class ChatServer : public QObject
    {
    Q_OBJECT
    public:
        explicit ChatServer(QObject *parent = nullptr);
        ~ChatServer() override = default;

        void start(quint16 port);
        void stop();

    public slots:
        void onPendingConnectionAvailable();
        void onReadyRead();

    private:
        QScopedPointer<QTcpServer> m_server{new QTcpServer{nullptr}};
        std::vector<ChatClient> m_clients;
    };
}
#endif //CHATSERVER_CHATSERVER_H
