#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qwlanmanager/manager.h"
#include "qwlanmanager/permissions.h"

#include <QButtonGroup>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void uiInit();
    void uiInitTitle();
    void uiInitMenu();

    void uiLoadIcons();
    void uiLoadManagerOptions();

    void uiDisplayBoxesInterfaces();
    void uiDisplayBoxesNetworks();

    void uiSetInfosBtnNetwork(QAbstractButton *btn, int index, const qwm::Network &network, bool isCurrent);

    void uiDisplayInterfaceSelected(const qwm::Interface &interface);
    void uiDisplayInterfaceCurrentNet(const qwm::Network &network);

    void uiDisplayMsgGeneric(const QString &title, const QString &msg, const QColor &color);
    void uiDisplayMsgInfos(const QString &infos);
    void uiDisplayMsgWarning(const QString &warning);
    void uiDisplayMsgErrors(const QString &errors);

    QString uiAskNetworkPassword(const QString &ssid);

    void uiUpdateSelectedNetwork(const QString &ssid, int idBtn);
    void uiUpdatePermission(qwm::WlanPerm idPerm);
    void uiRefreshPermission();

    void uiDisplayPopupError(const QString &err);

private:
    void onManagerInterfaceAdded(qwm::Interface interface);
    void onManagerInterfaceRemoved(qwm::Interface interface);

    void onManagerScanStarted(const QUuid &idInterface);
    void onManagerScanSucceed(const QUuid &idInterface, qwm::ListNetworks listNets);
    void onManagerScanFailed(const QUuid &idInterface, qwm::WlanError idErr);

    void onManagerConnectionStarted(const QUuid &idInterface, const QString &ssid);
    void onManagerConnectionSucceed(const QUuid &idInterface, const QString &ssid);
    void onManagerConnectionFailed(const QUuid &idInterface, const QString &ssid, qwm::WlanError idErr);

    void onManagerDisconnectionStarted(const QUuid &idInterface);
    void onManagerDisconnectionSucceed(const QUuid &idInterface);
    void onManagerDisconnectionFailed(const QUuid &idInterface, qwm::WlanError idErr);

    void onManagerForgetStarted(const QUuid &idInterface, const QString &ssid);
    void onManagerForgetSucceed(const QUuid &idInterface, const QString &ssid);
    void onManagerForgetFailed(const QUuid &idInterface, const QString &ssid, qwm::WlanError idErr);

    void onManagerSignalQualityChanged(const QUuid &idInterface, uint percent);

private slots:
    void on_checkBox_managerOptAllowAdminReqs_checkStateChanged(const Qt::CheckState &arg1);

    void on_btn_permStatusRefresh_clicked();
    void on_btn_permAsk_clicked();

    void on_btn_interfaceDoScan_clicked();
    void on_btn_interfaceOptCacheEdit_clicked();
    void on_checkBox_interfaceOptRequestQueue_checkStateChanged(const Qt::CheckState &arg1);

    void on_btn_connectToNetwork_clicked();
    void on_btn_disconnectFromNetwork_clicked();
    void on_btn_forgetNetwork_clicked();

    void on_btn_logsClear_clicked();

private:
    Ui::MainWindow *ui;

    qwm::Manager *m_wlanManager = nullptr;
    qwm::Permissions *m_wlanPerms = nullptr;

    qwm::Interface m_selectIface;
    QString m_selectNet;
    int m_selectNetBtn;

    QHash<int, QUuid> m_mapBtnIdIface;
    QHash<int, QString> m_mapBtnIdNet;
    QButtonGroup *m_btnGroupNet = nullptr;
};
#endif // MAINWINDOW_H
