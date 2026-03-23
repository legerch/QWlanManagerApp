#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "base/qwlanapp_global.h"
#include "ui/widgets/menu/menubar.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QRadioButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Initialize members */
    m_wlanManager = new qwm::Manager(this);
    m_wlanPerms = new qwm::Permissions(this);

    /* Manage UI */
    uiInit();

    /* Manage connections */
    // Wlan manager
    connect(m_wlanManager, &qwm::Manager::sInterfaceAdded, this, &MainWindow::onManagerInterfaceAdded);
    connect(m_wlanManager, &qwm::Manager::sInterfaceRemoved, this, &MainWindow::onManagerInterfaceRemoved);

    connect(m_wlanManager, &qwm::Manager::sScanStarted, this, &MainWindow::onManagerScanStarted);
    connect(m_wlanManager, &qwm::Manager::sScanSucceed, this, &MainWindow::onManagerScanSucceed);
    connect(m_wlanManager, &qwm::Manager::sScanFailed, this, &MainWindow::onManagerScanFailed);

    connect(m_wlanManager, &qwm::Manager::sConnectionStarted, this, &MainWindow::onManagerConnectionStarted);
    connect(m_wlanManager, &qwm::Manager::sConnectionSucceed, this, &MainWindow::onManagerConnectionSucceed);
    connect(m_wlanManager, &qwm::Manager::sConnectionFailed, this, &MainWindow::onManagerConnectionFailed);

    connect(m_wlanManager, &qwm::Manager::sDisconnectionStarted, this, &MainWindow::onManagerDisconnectionStarted);
    connect(m_wlanManager, &qwm::Manager::sDisconnectionSucceed, this, &MainWindow::onManagerDisconnectionSucceed);
    connect(m_wlanManager, &qwm::Manager::sDisconnectionFailed, this, &MainWindow::onManagerDisconnectionFailed);

    connect(m_wlanManager, &qwm::Manager::sForgetStarted, this, &MainWindow::onManagerForgetStarted);
    connect(m_wlanManager, &qwm::Manager::sForgetSucceed, this, &MainWindow::onManagerForgetSucceed);
    connect(m_wlanManager, &qwm::Manager::sForgetFailed, this, &MainWindow::onManagerForgetFailed);

    connect(m_wlanManager, &qwm::Manager::sSignalQualityChanged, this, &MainWindow::onManagerSignalQualityChanged);

    // Wlan permissions
    connect(m_wlanPerms, &qwm::Permissions::sPermissionsChanged, this, &MainWindow::uiUpdatePermission);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::uiInit()
{
    uiInitTitle();
    uiInitMenu();

    uiLoadIcons();
    uiLoadManagerOptions();

    uiRefreshPermission();
    uiDisplayBoxesInterfaces();
}

void MainWindow::uiInitTitle()
{
    const QString title = QString("%1 - v%2 - Qt %3").arg(
        qApp->applicationName(),
        qApp->applicationVersion(),
        qVersion()
    );

    setWindowTitle(title);
}

void MainWindow::uiInitMenu()
{
    MenuBar *menu = new MenuBar(this);
    setMenuBar(menu);
}

void MainWindow::uiLoadIcons()
{
    const QIcon edit = QIcon::fromTheme("edit");

    ui->btn_permStatusRefresh->setIcon(QIcon::fromTheme("refresh"));
    ui->btn_permAsk->setIcon(edit);

    ui->btn_interfaceOptCacheEdit->setIcon(edit);
}

void MainWindow::uiLoadManagerOptions()
{
    const qwm::WlanOptions opts = m_wlanManager->getOptions();
    ui->checkBox_managerOptAllowAdminReqs->setChecked(opts & qwm::WOPT_ALLOW_ADMIN_REQUESTS);
}

void MainWindow::uiDisplayBoxesInterfaces()
{
    /* Retrieve interface list */
    const qwm::ListInterfaces listIfaces = m_wlanManager->getInterfaces();
    m_mapBtnIdIface.clear();

    /* Prepare UI */
    QVBoxLayout *vLayout = new QVBoxLayout();
    QButtonGroup *btnGroup = new QButtonGroup(this);

    const QString patternMain = QString("%1 - %2 (%3)");
    const QString patternDetails = QString("UUID: %1\n"
                                           "HW address: %2");

    /* Display each interface */
    int index = 0;
    for(auto it = listIfaces.cbegin(); it != listIfaces.cend(); ++it){
        const qwm::Interface &iface = *it;
        const QUuid idIface = iface.getUid();

        // Set iface infos
        const QString main = patternMain.arg(index)
                                        .arg(iface.getName(), iface.getDescription());

        const QString tooltip = patternDetails.arg(idIface.toString(),
                                                   iface.getHwAddress());

        // Create button
        QRadioButton *btn = new QRadioButton(main);
        btn->setToolTip(tooltip);

        // Add to layout
        vLayout->addWidget(btn);
        btnGroup->addButton(btn, index);
        m_mapBtnIdIface.insert(index, idIface);

        /* Manage first UI display */
        if(!m_selectIface.isValid()){
            m_selectIface = iface;
        }

        // Manage case of UI refresh
        if(m_selectIface == iface){
            btn->setChecked(true);
        }

        ++index;
    }

    /* Set layout widget */
    qDeleteAll(ui->widget_listInterfaces->children());
    ui->widget_listInterfaces->setLayout(vLayout);

    /* Manage event */
    connect(btnGroup, &QButtonGroup::idClicked, this, [this](int indexBtn){
        const QUuid idInterface = m_mapBtnIdIface.value(indexBtn);
        m_selectIface = m_wlanManager->getInterface(idInterface);

        uiDisplayBoxesNetworks();
    });

    /* Refresh associated network UI */
    uiDisplayBoxesNetworks();
}

void MainWindow::uiDisplayBoxesNetworks()
{
    /* Retrieve network list */
    const qwm::MapNetworks &listNets = m_selectIface.getMapNetworks();
    const qwm::Network currentNet = m_selectIface.getNetworkConnected();

    m_mapBtnIdNet.clear();
    uiUpdateSelectedNetwork("", -1);

    /* Set interface associated status */
    uiDisplayInterfaceSelected(m_selectIface);
    uiDisplayInterfaceCurrentNet(currentNet);

    /* Prepare UI */
    QVBoxLayout *vLayout = new QVBoxLayout();
    m_btnGroupNet = new QButtonGroup(this);

    /* Display each network */
    int index = 0;
    for(auto it = listNets.cbegin(); it != listNets.cend(); ++it){
        const qwm::Network &net = *it;
        const bool isCurrent = net == currentNet;

        // Create button
        QRadioButton *btn = new QRadioButton();
        uiSetInfosBtnNetwork(btn, index, net, isCurrent);

        // Add to layout
        vLayout->addWidget(btn);
        m_btnGroupNet->addButton(btn, index);
        m_mapBtnIdNet.insert(index, net.getSsid());

        // Manage current UI state
        if(isCurrent){
            uiUpdateSelectedNetwork(net.getSsid(), index);
            btn->setChecked(true);
        }

        ++index;
    }

    /* Set layout widget */
    qDeleteAll(ui->widget_listNetworks->children());
    ui->widget_listNetworks->setLayout(vLayout);

    /* Manage event */
    connect(m_btnGroupNet, &QButtonGroup::idClicked, this, [this](int indexBtn){
        uiUpdateSelectedNetwork(m_mapBtnIdNet.value(indexBtn), indexBtn);
    });
}

void MainWindow::uiSetInfosBtnNetwork(QAbstractButton *btn, int index, const qwm::Network &network, bool isCurrent)
{
    /* Define strings patterns */
    static const QString patternMain = QString("%1 - %2 (%3 %)");
    static const QString patternDetails = QString("Is current network: %1\n"
                                                  "Profile: %2\n"
                                                  "Auth algorithm: %3 (%4)");

    /* Verify data validity */
    if(!btn){
        return;
    }

    /* Prepare network infos */
    const QString main = patternMain
                            .arg(index)
                            .arg(network.getSsid())
                            .arg(network.getSignalQuality());

    const QString profileName = network.getProfileName();
    const QString tooltip = patternDetails.arg(isCurrent ? "Yes" : "No",
                                               profileName.isEmpty() ? "none" : profileName,
                                               qwm::authAlgoToString(network.getAuthAlgo()),
                                               qwm::cipherAlgoToString(network.getCipherAlgo())
    );

    /* Set button informations */
    btn->setText(main);
    btn->setToolTip(tooltip);
}

void MainWindow::uiDisplayInterfaceSelected(const qwm::Interface &interface)
{
    ui->label_valueStatusInterfaceSelected->setText(interface.getName());

    const qwm::CachePolicy &policy = interface.getCachePolicy();
    ui->spinBox_interfaceCacheMaxScans->setValue(policy.getMaxScans());
    ui->spinBox_interfaceCacheMaxDelay->setValue(policy.getMaxDelay());

    const qwm::IfaceOptions opts = interface.getOptions();
    ui->checkBox_interfaceOptRequestQueue->setChecked(opts & qwm::IFACE_OPT_REQUEST);
}

void MainWindow::uiDisplayInterfaceCurrentNet(const qwm::Network &network)
{
    QString string = QString("none");
    if(network.isValid()){
        string = QString("%1 (%2 %)").arg(network.getSsid(), QString::number(network.getSignalQuality()));
    }

    ui->label_valueStatusInterfaceCurrentNet->setText(string);
}

void MainWindow::uiDisplayMsgGeneric(const QString &title, const QString &msg, const QColor &color)
{
    QString text = QString("[%1] %2").arg(title, msg);

    ui->textEdit_logs->setTextColor(color);
    ui->textEdit_logs->append(text);
}

void MainWindow::uiDisplayMsgInfos(const QString &infos)
{
    qInfo() << infos;
    uiDisplayMsgGeneric("Info", infos, Qt::black);
}

void MainWindow::uiDisplayMsgWarning(const QString &warning)
{
    qWarning() << warning;
    uiDisplayMsgGeneric("Warning", warning, QColor::fromRgb(255, 165, 0)); // Orange
}

void MainWindow::uiDisplayMsgErrors(const QString &errors)
{
    qCritical() << errors;
    uiDisplayMsgGeneric("Error", errors, Qt::red);
}

QString MainWindow::uiAskNetworkPassword(const QString &ssid)
{
    const QString title = "Network connection";
    const QString txt = QString("Network '%1' passkey:").arg(ssid);

    return QInputDialog::getText(this, title, txt, QLineEdit::Normal);
}

void MainWindow::uiUpdateSelectedNetwork(const QString &ssid, int idBtn)
{
    m_selectNet = ssid;
    m_selectNetBtn = idBtn;
}

void MainWindow::uiUpdatePermission(qwm::WlanPerm idPerm)
{
    QString strPerm = qwm::wlanPermToString(idPerm);
    strPerm[0] = strPerm.at(0).toUpper();

    ui->label_valuePermsStatus->setText(strPerm);

    const QString info = QString("Wlan permissions updated: %1").arg(strPerm);
    uiDisplayMsgInfos(info);
}

void MainWindow::uiRefreshPermission()
{
    const qwm::WlanPerm idPerm = m_wlanPerms->retrieveWlanPermissions();
    uiUpdatePermission(idPerm);
}

void MainWindow::uiDisplayPopupError(const QString &err)
{
    QMessageBox::critical(this, "Error", err);
    qCritical() << err;
}

void MainWindow::onManagerInterfaceAdded(qwm::Interface interface)
{
    const QString msg = QString("Interface added: '%1'").arg(interface.getName());
    uiDisplayMsgInfos(msg);

    uiDisplayBoxesInterfaces();
}

void MainWindow::onManagerInterfaceRemoved(qwm::Interface interface)
{
    /* Display event */
    const QString msg = QString("Interface removed: '%1'").arg(interface.getName());
    uiDisplayMsgInfos(msg);

    /* If currently selected, reset it */
    if(m_selectIface == interface){
        m_selectIface = qwm::Interface();
    }

    /* Refresh UI interfaces */
    uiDisplayBoxesInterfaces();
}

void MainWindow::onManagerScanStarted(const QUuid &idInterface)
{
    const qwm::Interface iface = m_wlanManager->getInterface(idInterface);

    const QString msg = QString("Scan on interface '%1' started").arg(
        iface.isValid() ? iface.getName() : idInterface.toString()
    );
    uiDisplayMsgInfos(msg);
}

void MainWindow::onManagerScanSucceed(const QUuid &idInterface, QWLANAPP_VAR_UNUSED qwm::ListNetworks listNets)
{
    /* Display event */
    const qwm::Interface iface = m_wlanManager->getInterface(idInterface);

    const QString msg = QString("Scan on interface '%1' succeed").arg(iface.getName());
    uiDisplayMsgInfos(msg);

    /* Update UI */
    if(iface == m_selectIface){
        uiDisplayBoxesNetworks();
    }
}

void MainWindow::onManagerScanFailed(const QUuid &idInterface, qwm::WlanError idErr)
{
    /* Display event */
    const qwm::Interface iface = m_wlanManager->getInterface(idInterface);

    const QString pattern = QString("Scan on interface '%1' failed [err: %2]");
    uiDisplayMsgWarning(pattern.arg(
        iface.isValid() ? iface.getName() : idInterface.toString(),
        qwm::wlanErrorToString(idErr))
    );
}

void MainWindow::onManagerConnectionStarted(const QUuid &idInterface, const QString &ssid)
{
    const qwm::Interface iface = m_wlanManager->getInterface(idInterface);

    const QString msg = QString("Try to connect to network '%1' on interface '%2'").arg(
        ssid,
        iface.isValid() ? iface.getName() : idInterface.toString()
    );
    uiDisplayMsgInfos(msg);
}

void MainWindow::onManagerConnectionSucceed(const QUuid &idInterface, const QString &ssid)
{
    /* Display event */
    const qwm::Interface iface = m_wlanManager->getInterface(idInterface);

    const QString msg = QString("Interface '%1' succeed to connect to '%2'").arg(iface.getName(), ssid);
    uiDisplayMsgInfos(msg);

    /* Update UI */
    if(iface == m_selectIface){
        uiDisplayBoxesNetworks();
    }
}

void MainWindow::onManagerConnectionFailed(const QUuid &idInterface, const QString &ssid, qwm::WlanError idErr)
{
    /* Is error due to wrong passkey ? */
    if(idErr == qwm::WlanError::WERR_NET_PASSKEY){
        const QString passwd = uiAskNetworkPassword(ssid);
        if(!passwd.isEmpty()){
            m_wlanManager->doConnect(idInterface, ssid, passwd);
            return;
        }
    }

    /* Display event */
    const QString pattern = QString("Interface '%1' failed to connect to '%2' [err: %3]");

    const qwm::Interface iface = m_wlanManager->getInterface(idInterface);
    uiDisplayMsgWarning(pattern.arg(
        iface.isValid() ? iface.getName() : idInterface.toString(),
        ssid,
        qwm::wlanErrorToString(idErr))
    );

    /* Update UI */
    if(iface == m_selectIface){
        uiDisplayBoxesNetworks();
    }
}

void MainWindow::onManagerDisconnectionStarted(const QUuid &idInterface)
{
    const qwm::Interface iface = m_wlanManager->getInterface(idInterface);

    const QString msg = QString("Try to disconnect on interface '%1'").arg(
        iface.isValid() ? iface.getName() : idInterface.toString()
    );
    uiDisplayMsgInfos(msg);
}

void MainWindow::onManagerDisconnectionSucceed(const QUuid &idInterface)
{
    /* Display event */
    const qwm::Interface iface = m_wlanManager->getInterface(idInterface);

    const QString msg = QString("Interface '%1' succeed to disconnect").arg(iface.getName());
    uiDisplayMsgInfos(msg);

    /* Update UI */
    if(iface == m_selectIface){
        uiDisplayBoxesNetworks();
    }
}

void MainWindow::onManagerDisconnectionFailed(const QUuid &idInterface, qwm::WlanError idErr)
{    
    /* Display event */
    const QString pattern = QString("Interface '%1' failed to disconnect [err: %2]");

    const qwm::Interface iface = m_wlanManager->getInterface(idInterface);
    uiDisplayMsgWarning(pattern.arg(
        iface.isValid() ? iface.getName() : idInterface.toString(),
        qwm::wlanErrorToString(idErr))
    );

    /* Update UI */
    if(iface == m_selectIface){
        uiDisplayBoxesNetworks();
    }
}

void MainWindow::onManagerForgetStarted(const QUuid &idInterface, const QString &ssid)
{
    const qwm::Interface iface = m_wlanManager->getInterface(idInterface);

    const QString msg = QString("Try to forget network '%1' on interface '%2'").arg(
        ssid,
        iface.isValid() ? iface.getName() : idInterface.toString()
    );
    uiDisplayMsgInfos(msg);
}

void MainWindow::onManagerForgetSucceed(const QUuid &idInterface, const QString &ssid)
{
    /* Display event */
    const qwm::Interface iface = m_wlanManager->getInterface(idInterface);

    const QString msg = QString("Interface '%1' succeed to forget network '%2'").arg(iface.getName(), ssid);
    uiDisplayMsgInfos(msg);

    /* Update UI */
    if(iface == m_selectIface){
        uiDisplayBoxesNetworks();
    }
}

void MainWindow::onManagerForgetFailed(const QUuid &idInterface, const QString &ssid, qwm::WlanError idErr)
{
    /* Display event */
    const QString pattern = QString("Interface '%1' failed to forget network '%2' [err: %3]");

    const qwm::Interface iface = m_wlanManager->getInterface(idInterface);
    uiDisplayMsgWarning(pattern.arg(
        iface.isValid() ? iface.getName() : idInterface.toString(),
        ssid,
        qwm::wlanErrorToString(idErr))
    );

    /* Update UI */
    if(iface == m_selectIface){
        uiDisplayBoxesNetworks();
    }
}

void MainWindow::onManagerSignalQualityChanged(const QUuid &idInterface, QWLANAPP_VAR_UNUSED uint percent)
{
    /* Retrieve associated datas */
    const qwm::Interface iface = m_wlanManager->getInterface(idInterface);
    if(!iface.isValid()){
        return;
    }

    const qwm::Network current = iface.getNetworkConnected();
    QAbstractButton *btn = m_btnGroupNet->button(m_selectNetBtn);

    /* Update current connection infos */
    uiDisplayInterfaceCurrentNet(current);

    /* Update network associated list button */
    uiSetInfosBtnNetwork(btn, m_selectNetBtn, current, true);
}

void MainWindow::on_checkBox_managerOptAllowAdminReqs_checkStateChanged(const Qt::CheckState &arg1)
{
    /* Set manager options */
    qwm::WlanOptions opts = m_wlanManager->getOptions();
    opts.setFlag(qwm::WOPT_ALLOW_ADMIN_REQUESTS, arg1);

    m_wlanManager->setOptions(opts);

    /* Display result */
    const QString strMsg = QString("Set options for manager [opts: %1]").arg(qwm::wlanOptionsToString(opts));
    uiDisplayMsgInfos(strMsg);
}

void MainWindow::on_btn_permStatusRefresh_clicked()
{
    uiRefreshPermission();
}

void MainWindow::on_btn_permAsk_clicked()
{
    const qwm::WlanError wlanErr = m_wlanPerms->askWlanPermissions();
    if(wlanErr != qwm::WlanError::WERR_NO_ERROR){
        const QString err = QString("Failed to open WLAN parameters for permissions [id-err: %1]").arg(qwm::wlanErrorToString(wlanErr));
        uiDisplayPopupError(err);
    }
}

void MainWindow::on_btn_interfaceDoScan_clicked()
{
    m_wlanManager->doScan(m_selectIface.getUid());
}

void MainWindow::on_btn_interfaceOptCacheEdit_clicked()
{
    /* Set cache policy */
    const qwm::CachePolicy policy(
        ui->spinBox_interfaceCacheMaxScans->value(),
        ui->spinBox_interfaceCacheMaxDelay->value()
    );

    m_selectIface.setCachePolicy(policy);

    /* Display result */
    const QString strMsg = QString("Set cache policy for interface '%1' [max-scan: %2, delay: %3]")
                        .arg(m_selectIface.getName())
                        .arg(policy.getMaxScans())
                        .arg(policy.getMaxDelay()
    );

    uiDisplayMsgInfos(strMsg);
}

void MainWindow::on_checkBox_interfaceOptRequestQueue_checkStateChanged(const Qt::CheckState &arg1)
{
    /* Set interface options */
    qwm::IfaceOptions opts = m_selectIface.getOptions();
    opts.setFlag(qwm::IFACE_OPT_REQUEST, arg1);

    m_selectIface.setOptions(opts);

    /* Display result */
    const QString strMsg = QString("Set options for interface '%1' [opts: %2]").arg(
        m_selectIface.getName(),
        qwm::ifaceOptionsToString(opts)
    );

    uiDisplayMsgInfos(strMsg);
}

void MainWindow::on_btn_connectToNetwork_clicked()
{
    m_wlanManager->doConnect(m_selectIface.getUid(), m_selectNet);
}

void MainWindow::on_btn_disconnectFromNetwork_clicked()
{
    m_wlanManager->doDisconnect(m_selectIface.getUid());
}

void MainWindow::on_btn_forgetNetwork_clicked()
{
    m_wlanManager->doForget(m_selectIface.getUid(), m_selectNet);
}

void MainWindow::on_btn_logsClear_clicked()
{
    ui->textEdit_logs->clear();
}
