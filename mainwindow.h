#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cell.h"
#include <QList>
#include <QString>

namespace Ui {
class MainWindow;
}

const QString defaultTitle = "Редактор полей";

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_disableAll_clicked();
    void on_enableAll_clicked();
    void saveAs_clicked();
    void save_clicked();
    void exit_clicked();
    void open_clicked();

private:
    Ui::MainWindow *ui;
    QList<Cell*> m_cellList;
    bool m_isChangesSaved;
    QString m_fileName;

    void setStateForAllCells(bool existed);
    void saveXML();
    virtual void closeEvent(QCloseEvent *e);

public:
    void setSaveState(bool isSaved);
};

#endif // MAINWINDOW_H
