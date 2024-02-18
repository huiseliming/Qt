#include "GitInfo.h"
#include <QApplication>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QtWidgets>
#include <private/qhooks_p.h>

void (*fpQtAddObject)(QObject *) = nullptr;
void (*fpHookAddObject)(QObject *) = [](QObject *object) {
  if (fpQtAddObject) {
    fpQtAddObject(object);
  }
};

int main(int argc, char *argv[]) {
  fpQtAddObject = (decltype(fpQtAddObject))qtHookData[QHooks::AddQObject];
  qtHookData[QHooks::AddQObject] =
      (decltype(qtHookData[QHooks::AddQObject]))fpHookAddObject;

  QApplication app(argc, argv);

  static QMenu *trayMenu = new QMenu();
  trayMenu->addAction(QIcon(), "关于", [&] {
    static QWidget *aboutWidget = nullptr;
    if (!aboutWidget) {
      aboutWidget = new QWidget();
      QObject::connect(aboutWidget, &QWidget::destroyed,
                       [&] { aboutWidget = nullptr; });
      aboutWidget->setAttribute(Qt::WA_DeleteOnClose);
      aboutWidget->setWindowTitle("关于");
      aboutWidget->show();
      QVBoxLayout *vBoxLayout = new QVBoxLayout();
      {
        vBoxLayout->addWidget(
            new QLabel(QString("Version: %1").arg(GitDescribe())));
        vBoxLayout->addWidget(
            new QLabel(QString("GitDate: %1").arg(GitCommitDate())));
        vBoxLayout->addWidget(new QLabel(
            QString("GitSHA1: %1").arg(GitIsDirty() ? "?" : GitCommitSHA1())));
      }
      aboutWidget->setLayout(vBoxLayout);
      aboutWidget->setMaximumSize(aboutWidget->sizeHint());
      // aboutWidget->setSizePolicy(QSizePolicy::MinimumExpanding,
      // QSizePolicy::MinimumExpanding);
    } else {
    }
  });
  trayMenu->addAction(QIcon(), "退出", [&] { QApplication::quit(); });

  QSystemTrayIcon *systemTray = new QSystemTrayIcon(&app);
  systemTray->setContextMenu(trayMenu);
  systemTray->setIcon(QIcon(":/img/SystemTray.Icon.png"));
  systemTray->setToolTip("DesktopApp");
  systemTray->show();

  return app.exec();
}