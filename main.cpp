#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QQuickView>
#include <QStandardPaths>
#include "setting.h"
#include "hostmodel.h"
#include "waker.h"

int main(int argc, char *argv[]) {
    qDebug() << QSslSocket::supportsSsl();
    qDebug() << QSslSocket::sslLibraryBuildVersionString();
    // http://stackoverflow.com/questions/27982443/qnetworkaccessmanager-crash-related-to-ssl
    // qunsetenv("OPENSSL_CONF");
    QString path =  QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    const QString settingFileName = path + "/qwaker.setting";
    const QString hostsFileName = path + "/qwaker.hosts";
    Setting setting;

    if(setting.load(settingFileName)) {
        qDebug() << "config file"<< settingFileName << "loaded.";
    } else {
        qDebug() << "failed load config file" << hostsFileName << "use default.";
        setting.set("protocol","https");
        setting.set("address","192.168.1.1");
        setting.set("port","443");
        setting.set("user","admin");
        setting.set("password","admin");
    }

    HostListModel hostList;
    if(hostList.load(hostsFileName)){
        qDebug() << "host list"<< hostsFileName << "loaded.";
    } else {
        qDebug() << "failed load host list"<< hostsFileName;
    }

    Waker *pLoginNetworkManager = new Waker();
    //qmlRegisterType<LoginNetworkManager>("routerMaster.autopard.com", 1, 0, "RouterMaster");

    QGuiApplication app(argc, argv);
    app.setApplicationName("qwaker");
    app.setOrganizationName("www.autopard.com");
    QQuickView view;
    QQmlContext *ctxt = view.rootContext();
    ctxt->setContextProperty("waker", pLoginNetworkManager);
    ctxt->setContextProperty("hostModel", &hostList);
    ctxt->setContextProperty("appSetting", &setting);
    QObject::connect(ctxt->engine(), SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setMinimumSize(QSize(480,640));
    view.resize(720,1280);
    view.show();
    int result = app.exec();
    if(hostList.save(hostsFileName)){
        qDebug() << "hosts list" << hostsFileName << "saved.";
    } else {
        qDebug() << "save hosts list" << hostsFileName << "failed!";
    }

    if(setting.save(settingFileName)){
        qDebug() << "setting file" << settingFileName << "saved.";
    } else {
        qDebug() << "save setting file" << settingFileName << "failed!";
    }
    pLoginNetworkManager->deleteLater();
    return result;
}
