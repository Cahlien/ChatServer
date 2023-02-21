#include "src/app/main.h"
#include "src/dtos/DataMessage.h"
#include "src/server/ChatServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale: uiLanguages)
    {
        const QString baseName = "ChatServer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            app.installTranslator(&translator);
            break;
        }
    }

    dc::ChatServer server{&app};
    server.start(1234);

    return app.exec();
}
