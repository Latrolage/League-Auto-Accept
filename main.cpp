#include "menuclickedactions.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDebug>
#include <QtGui/QIcon>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QMenu>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
  if (argc != 1 && static_cast<std::string>(argv[1]) != "--nopkexec") system("pkexec sh -c 'sysctl -w abi.vsyscall32=0' > /dev/null");
  //qputenv("QT_ASSUME_STDERR_HAS_CONSOLE","1");
  QApplication oApp( argc, argv );
  //QLoggingCategory::setFilterRules("*.debug=true");

  QMenu menu;
  QSystemTrayIcon TrayIcon(QIcon(":systemTrayIcon.png"));

  TrayIcon.show();

  QAction *autoacceptAction=new QAction("Autoaccept", NULL);
  autoacceptAction->setCheckable(true);
  QAction *offlineAction=new QAction("Offline", NULL);
  offlineAction->setCheckable(true);
  QAction *exitAction=new QAction("Exit", NULL);

  menu.addAction(autoacceptAction);
  menu.addAction(offlineAction);
  menu.addAction(exitAction);

  TrayIcon.setContextMenu(&menu);
  auto menuClicked = new menuClickedActions();

  QObject::connect(autoacceptAction, SIGNAL(triggered(bool)), menuClicked, SLOT(accept(bool)));
  autoacceptAction->trigger();
  QObject::connect(offlineAction, SIGNAL(triggered(bool)), menuClicked, SLOT(offline(bool)));
  //offlineAction->trigger();
  QObject::connect(exitAction, SIGNAL(triggered()), &oApp, SLOT(quit()));
  return oApp.exec();
}
