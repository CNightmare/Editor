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
    explicit Cell(MainWindow *parent);
    ~Cell();
     //сеттеры
    void setState(bool existed);
    void setTileState(bool isHasTile);
    void setLockedLevel(int lLevel);

    //аксессоры
    bool getState(){return m_existed;}
    bool getTileState(){return m_isHasTile;}
    int getNumOfChains(){return m_lLevel;}
private:
    Ui::Cell *ui;
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *qre);
    void changePaletteState();//включение/отключение ячейки

    //=======Параметры ячейки
    bool m_existed;// существует или нет
    bool m_isHasTile;//имеет ли плитку
    unsigned char m_lLevel; //количество цепей
    //=======

    MainWindow *m_mainWindow; 
    QPalette whitePalette;
    QPalette blackPalette;

};

#endif // CELL_H
