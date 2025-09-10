#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

class MenuBar : public QMenuBar
{
    Q_OBJECT

public:
    MenuBar(QWidget *parent = nullptr);

private:
    void initMenus();

    void initMenuHelp();

private:
    void showHelpAbout();
    void showHelpAboutQt();
};

#endif // MENUBAR_H
