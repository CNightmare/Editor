#ifndef CELL_H
#define CELL_H

#include <QWidget>
//#include "mainwindow.h"
class MainWindow;
namespace Ui {
class Cell;
}

class Cell : public QWidget
{
    Q_OBJECT

public:
    explicit Cell(bool existed,MainWindow *parent);
    ~Cell();
    void setState(bool existed);
    bool getState();
private:
    Ui::Cell *ui;
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *qre);
    void changePaletteState();

    MainWindow *m_mainWindow;
    bool m_existed;
    QPalette whitePalette;
    QPalette blackPalette;
};

#endif // CELL_H
