//
// Created by Matthew Crowell on 2/18/2023.
//

#ifndef CHATSERVER_CHATCLIENT_H
#define CHATSERVER_CHATCLIENT_H

#include <QObject>
#include <QRunnable>
#include <QScopedPointer>
#include <QTcpServer>
#include <QTcpSocket>

namespace dc
{
    class ChatClient : public QObject
    {
    Q_OBJECT
    public:
        explicit ChatClient(QObject* parent = nullptr, QTcpServer* server = nullptr);
        ChatClient(const ChatClient& other) = delete;
        ChatClient(ChatClient&& other) noexcept;
        ChatClient& operator=(const ChatClient& other) = delete;
        ChatClient& operator=(ChatClient&& other) noexcept;
        ~ChatClient() override = default;

        QTcpSocket* socket() const;

#   pragma clang diagnostic push
#   pragma ide diagnostic ignored "NotImplementedFunctions"

    signals:
        void readyRead();
        void disconnected();

#   pragma clang diagnostic pop

    private:
        std::unique_ptr<QTcpSocket> m_socket;
    };
}

#endif //CHATSERVER_CHATCLIENT_H
