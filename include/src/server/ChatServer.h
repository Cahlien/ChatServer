//
// Created by Matthew Crowell on 2/12/2023.
//

#ifndef CHATSERVER_CHATSERVER_H
#define CHATSERVER_CHATSERVER_H

#include <QObject>
#include <QTcpServer>

namespace dc
{
    class ChatServer : public QObject
    {
    Q_OBJECT
    public:
        explicit ChatServer(QObject *parent = nullptr);

        ~ChatServer() override = default;

    public slots:

        void start(quint16 port);

        void stop();

    private:
        QScopedPointer<QTcpServer> m_server{new QTcpServer{nullptr}};
    };
}
#endif //CHATSERVER_CHATSERVER_H
