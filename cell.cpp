#include "cell.h"
#include "ui_cell.h"
#include <QMessageBox>
#include "mainwindow.h"
#include <QPicture>
Cell::Cell(MainWindow *parent) :
    ui(new Ui::Cell),
    m_existed(true),
    m_isHasTile(false),
    m_lLevel(0),
    m_mainWindow(parent)
{
    ui->setupUi(this);

    ui->tile->setVisible(false);//по умолчанию плитки не нужны
    whitePalette.setColor(QPalette::Background,Qt::white);
    blackPalette.setColor(QPalette::Background,Qt::black);
    changePaletteState();
}

void Cell::changePaletteState()
{
    m_existed ? setPalette(whitePalette) : setPalette(blackPalette);
}

void Cell::setTileState(bool isHasTile)
{
    m_isHasTile = isHasTile;
    ui->tile->setVisible(m_isHasTile);
}

void Cell::setLockedLevel(int lLevel)
{
    if(lLevel < 0 || lLevel>2)
        m_lLevel = 0;
    else
        m_lLevel = lLevel;
    if(m_lLevel == 0)
        ui->locked->setVisible(false);
    else
    {
        ui->locked->setVisible(true);
        ui->locked->setPixmap(QPixmap(QString(":/chain%1.png").arg(m_lLevel)));
    }

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

Cell::~Cell()
{
    delete ui;
}

//меняем состояние ячейки по клику
void Cell::mousePressEvent(QMouseEvent*)
{
    switch(m_mainWindow->getEditorState())
    {
        case ENABLE_CELLS://включение/отключение
            m_mainWindow->setSaveState(false);
            m_existed = !m_existed;
            changePaletteState();
        break;
        case ADD_TILES://плитка
            if(m_existed)
            {
                setTileState(!m_isHasTile);
            }
        break;
        case ADD_LOCKED://цепи
            if(m_existed)
            {
                setLockedLevel(++m_lLevel); //внутри есть проверка, так что все нормально
            }
        break;
    }
}
