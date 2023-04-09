#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    timelineWidget = new TimelineWidget(this);
    setCentralWidget(timelineWidget);

    // Create the menu bar
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // Create the "File" menu
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar->addMenu(fileMenu);

    // Create the "Open" action
    QAction *openAction = new QAction(tr("&Open"), this);
    fileMenu->addAction(openAction);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    // Create the toolbar
    QToolBar *toolbar = addToolBar(tr("Tools"));
    toolbar->setMovable(false);

    // Add the "FIT" button to the toolbar
    QAction *fitAction = new QAction(tr("&FIT"), this);
    fitAction->setShortcut(tr("Ctrl+F"));
    toolbar->addAction(fitAction);
    connect(fitAction, &QAction::triggered, timelineWidget, &TimelineWidget::fitInView);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        tr("CSV Files (*.csv);;All Files (*)"));

    if (!fileName.isEmpty()) {
        timelineWidget->readEventsFromFile(fileName);
    }
}
