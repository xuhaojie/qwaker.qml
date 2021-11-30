#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookieJar>
#include <QNetworkCookie>

class Waker : public QObject {
    Q_OBJECT
public:
    explicit Waker();
    virtual ~Waker();

    Q_INVOKABLE void login();
    Q_INVOKABLE void executeCommand(const QString& cmd);
    Q_INVOKABLE void wakeUp(const QString& target);
    Q_INVOKABLE void logout();
    Q_INVOKABLE void setup(const QString& url,const QString& userName, const QString& password);

protected:
    void dumpCookies();
    QSslConfiguration ssl_config;
    QNetworkAccessManager* manager;
    QString base_url;
    QString auth;
    QString token;

signals:
    void signalLoginResult(bool result);
    void signalExecuteCommandResult(bool result);
    void signalLogoutResult(bool result);

private slots:
    void loginReplyFinished();
    void executeCommandReplyFinished();
    void logoutReplyFinished();
};
