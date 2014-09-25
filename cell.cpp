#include "cell.h"
#include "ui_cell.h"
#include <QMessageBox>
#include "mainwindow.h"
Cell::Cell(bool existed,MainWindow *parent) :
    ui(new Ui::Cell),
    m_mainWindow(parent),
    m_existed(existed)
{
    ui->setupUi(this);

    whitePalette.setColor(QPalette::Background,Qt::white);
    blackPalette.setColor(QPalette::Background,Qt::black);
    changePaletteState();
}

void Cell::changePaletteState()
{
    m_existed ? this->setPalette(whitePalette) : this->setPalette(blackPalette);
}


void Cell::resizeEvent(QResizeEvent*)
{
    //Масштабирование с сохранением пропорций виджета
    const int size = std::min(this->width(),this->height());
    resize(size,size);
}

void Cell::setState(bool existed)
{
    if(existed != m_existed)
    {
        m_existed = existed;
        changePaletteState();
    }
}

bool Cell::getState()
{
    return m_existed;
}

Cell::~Cell()
{
    delete ui;
}

//меняем состояние ячейки по клику
void Cell::mousePressEvent(QMouseEvent*)
{
    m_mainWindow->setSaveState(false);
    m_existed =! m_existed;
    changePaletteState();
}
