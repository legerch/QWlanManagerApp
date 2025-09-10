#include "base/qwlanapp_global.h"
#include "ui/widgets/mainwindow.h"

#include <QApplication>
#include <QStyleHints>

#include "qlogger/qloggerfactory.h"

/*****************************/
/* Macro definitions         */
/*****************************/
#define APP_LOG_FILE            "logs/log.txt"
#define APP_LOG_NB_FILES        3
#define APP_LOG_SIZE            (1024 * 1024 * 5) // Equals 5 megabytes (Mb)
#define APP_LOG_ENABLE_CONSOLE  true

/*****************************/
/* Main method               */
/*****************************/

int main(int argc, char *argv[])
{
    /* Initialize logs */
    QLogger::QLoggerFactory::instance().initLoggerRotating(QFileInfo(APP_LOG_FILE), APP_LOG_NB_FILES, APP_LOG_SIZE, APP_LOG_ENABLE_CONSOLE);
    qInfo("Application %s is started [version: %s]", QWLANAPP_INFO_ID, QWLANAPP_VERSION_STR);

    /* Set application properties */
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/logo/main"));
    app.setApplicationName(QWLANAPP_INFO_ID);
    app.setApplicationVersion(QWLANAPP_VERSION_STR);
    app.addLibraryPath(":/plugins/imageformats");

    /* Manage theme */
    // Set ressources theme path
    QStringList themePathsList = QIcon::themeSearchPaths();
    themePathsList << ":/assets/icons";
    QIcon::setThemeSearchPaths(themePathsList);
    QIcon::setFallbackThemeName(QIcon::themeName());

    // Force theme to light (since dark is not supported on our app)
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    app.styleHints()->setColorScheme(Qt::ColorScheme::Light);
#endif
    QIcon::setThemeName("light");

    /* Start the application */
    MainWindow window;
    window.show();

    return app.exec();
}
