#include "menuclickedactions.h"
#include "enums.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDebug>
#include <QtGui/QIcon>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QMenu>
#include <QActionGroup>
#include <QLoggingCategory>

void addStatusActions(QAction *&action, QMenu &menu, QActionGroup &actionGroup) {
    action->setCheckable(true);
    actionGroup.addAction(action);
    menu.addAction(action);
}
int main(int argc, char *argv[])
{
  //qputenv("QT_ASSUME_STDERR_HAS_CONSOLE","1");
  QApplication oApp( argc, argv );
  //QLoggingCategory::setFilterRules("*.debug=true");

  QMenu menu;
  QSystemTrayIcon TrayIcon(QIcon(":systemTrayIcon.png"));
  auto menuClicked = new menuClickedActions();

  TrayIcon.show();

  QAction *autoacceptAction=new QAction("Autoaccept", &menu);
  QObject::connect(autoacceptAction, SIGNAL(triggered(bool)), menuClicked, SLOT(accept(bool)));
  autoacceptAction->setCheckable(true);
  menu.addAction(autoacceptAction);
  autoacceptAction->trigger();


  QMenu statusButtonsMenu("Status", &menu);
  QActionGroup statusActionGroup(nullptr);
  menu.addMenu(&statusButtonsMenu);
  statusActionGroup.setExclusive(true);
  QAction *statusAction=new QAction("Online", &statusButtonsMenu); // Online Status
  QObject::connect(statusAction, &QAction::triggered, [&menuClicked] (bool checked) {
                   menuClicked->statusChange(checked, ONLINE);
  });
  addStatusActions(statusAction, statusButtonsMenu, statusActionGroup);
  statusAction=new QAction("Offline", &statusButtonsMenu); // Offline Status
  QObject::connect(statusAction, &QAction::triggered, [&menuClicked] (bool checked) {
                   menuClicked->statusChange(checked, OFFLINE);
  });
  addStatusActions(statusAction, statusButtonsMenu, statusActionGroup);
  statusAction=new QAction("None", &statusButtonsMenu); // Don't try to force a status
  addStatusActions(statusAction, statusButtonsMenu, statusActionGroup);
  QObject::connect(statusAction, &QAction::triggered, [&menuClicked] (bool checked) {
                   menuClicked->statusChange(checked, NONE);
  });
  statusAction->trigger();
  QObject::connect(&TrayIcon, &QSystemTrayIcon::activated, [&menu] (QSystemTrayIcon::ActivationReason r) {
                   if (r==QSystemTrayIcon::ActivationReason::Trigger) menu.popup(QCursor::pos());
  });

  QAction *exitAction=new QAction("Exit", &menu);
  menu.addAction(exitAction);
  QObject::connect(exitAction, SIGNAL(triggered()), &oApp, SLOT(quit()));

  TrayIcon.setContextMenu(&menu);

  return oApp.exec();
}
