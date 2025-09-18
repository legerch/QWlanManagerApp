#include "menubar.h"

#include "qlogger/qloggerfactory.h"
#include "qwlanmanager/qwlantypes.h"
#include "toolboxqt/widgets/dialogabout.h"

#include <QLibraryInfo>
#include <QMessageBox>

MenuBar::MenuBar(QWidget *parent)
    : QMenuBar(parent)
{
    initMenus();
}

void MenuBar::initMenus()
{
    initMenuHelp();
}

void MenuBar::initMenuHelp()
{
    /* Create "about" menu */
    QMenu *menu = new QMenu("Help", this);

    /* Create all "about" actions */
    QAction *aboutApp = menu->addAction("About");
    QAction *aboutQt = menu->addAction("About Qt");

    /* Associate actions to slots */
    QObject::connect(aboutApp, &QAction::triggered, this, &MenuBar::showHelpAbout);
    QObject::connect(aboutQt, &QAction::triggered, this, &MenuBar::showHelpAboutQt);

    /* Add menu to bar */
    addMenu(menu);
}

void MenuBar::showHelpAbout()
{
    /* Create dialog about */
    tbq::DialogAbout *dialogAbout = new tbq::DialogAbout(this);
    dialogAbout->setAttribute(Qt::WA_DeleteOnClose);

    /* Set informations */
    // Set app main informations
    dialogAbout->addSectionAbout("This application is used to manage networks",
        tbq::RichLink(QUrl("https://github.com/legerch/QWlanManagerApp")),
        tbq::RichLink(QUrl("https://github.com/legerch/QWlanManagerApp/issues"))
    );

    // Set dependency section
    const tbq::DialogAbout::ListDeps listDeps = {
        tbq::DepInfos(tbq::RichLink(QUrl("https://www.qt.io/"), "Qt Framework"), QLibraryInfo::version()),

        tbq::DepInfos(tbq::RichLink(QUrl("https://github.com/legerch/QLogger"), "QLogger"), QLogger::QLoggerFactory::getLibraryVersion()),
        tbq::DepInfos(tbq::RichLink(QUrl("https://github.com/legerch/QWlanManager"), "QWlanManager"), qwm::getLibraryVersion()),
        tbq::DepInfos(tbq::RichLink(QUrl("https://github.com/legerch/ToolBoxQt"), "Toolbox Qt"), QVersionNumber::fromString(TOOLBOXQT_VERSION_STR))
    };
    dialogAbout->addSectionDeps(listDeps);

    // Set credit section
    const tbq::DialogAbout::ListResGroups listRes = {
        {
            .name = "Icons",
            .listRes = {
                {.source = tbq::RichLink(QUrl("https://www.flaticon.com/free-icon/wifi-router_2876860"), "Main logo"), .author = "Freepik", .license = "Flaticon License"},
                {.source = tbq::RichLink(QUrl("https://www.flaticon.com/free-icon/reload_560463"), "Refresh"), .author = "Freepik", .license = "Flaticon License"},
                {.source = tbq::RichLink(QUrl("https://www.flaticon.com/free-icon/compose_10747217"), "Edit"), .author = "Arkinasi", .license = "Flaticon License"},
            }
        }
    };
    dialogAbout->addSectionRessources(listRes);

    // Add useful documents
    dialogAbout->addSectionChangelog(QUrl("qrc:/docs/changelog"));
    dialogAbout->addSectionLicense(QUrl("qrc:/docs/license"));

    /* Display dialog about */
    dialogAbout->show();
}

void MenuBar::showHelpAboutQt()
{
    QMessageBox::aboutQt(this);
}
