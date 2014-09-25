#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QCloseEvent>
#include <QDialogButtonBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_isChangesSaved(true),
    m_fileName(QString())
{
    ui->setupUi(this);
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            Cell *cell = new Cell(true,this);
            m_cellList.append(cell);
            ui->fieldLayout->addWidget(m_cellList.back(),i,j);
        }
    }
    ui->buttonsLayout->setAlignment(Qt::AlignLeft|Qt::AlignTop);

    //привязка слотов для пунктов меню
    connect(ui->saveAs,SIGNAL(triggered()),this,SLOT(saveAs_clicked()));
    connect(ui->save,SIGNAL(triggered()),this,SLOT(save_clicked()));
    connect(ui->exit,SIGNAL(triggered()),this,SLOT(exit_clicked()));
    connect(ui->open,SIGNAL(triggered()),this,SLOT(open_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open_clicked()
{

}

void MainWindow::on_disableAll_clicked()
{
    setStateForAllCells(false);
}

void MainWindow::exit_clicked()
{
    this->close();
}

void MainWindow::save_clicked()
{
    if(m_fileName.isEmpty())
    {
        saveAs_clicked();
    }
    else
    {
        saveXML();
    }
}

void MainWindow::saveXML()
{
   QFile file(m_fileName);
   if (!file.open(QIODevice::WriteOnly))
   {
     QMessageBox::warning(0, "Ошибка", "Данный файл предназначен только для чтения.");
   }
   else
   {
       QXmlStreamWriter writer(&file);
       writer.setAutoFormatting(true);
       writer.writeStartDocument();
       {
        writer.writeStartElement("field");
        {
            for(QList<Cell*>::iterator it = m_cellList.begin(); it != m_cellList.end(); ++it)
            {
                writer.writeStartElement("cell");
                {
                    QString existedValue = QString::number(static_cast<int>((*it)->getState()));
                    writer.writeAttribute("existed",existedValue);
                }
                writer.writeEndElement();
            }
        }
        writer.writeEndElement();
       }
       writer.writeEndDocument();
       setSaveState(true);
   }
}

void MainWindow::saveAs_clicked()
{
    QFileDialog saveDialog(this);
    saveDialog.setFileMode(QFileDialog::AnyFile);
    saveDialog.setNameFilter(tr("XML-файлы (*.xml)"));
    saveDialog.setAcceptMode(QFileDialog::AcceptSave);
    if(saveDialog.exec())
    {
        m_fileName = saveDialog.selectedFiles().at(0);
        saveXML();
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(!m_isChangesSaved)
    {
        QMessageBox msgBox;
        msgBox.setText("Выход без сохранения");
        msgBox.setInformativeText("Сохранить изменения?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int res = msgBox.exec();
        switch(res)
        {
        case QMessageBox::Save:
            save_clicked();
            e->accept();
        break;
        case QMessageBox::Discard:
            e->accept();
        break;
        case QMessageBox::Cancel:
            e->ignore();
        break;
        }
    }
}

void MainWindow::setSaveState(bool isSaved)
{
    m_isChangesSaved = isSaved;
    if(m_isChangesSaved)
    {
        ui->save->setEnabled(false);
        this->setWindowTitle(defaultTitle);
    }
    else
    {
        ui->save->setEnabled(true);
        this->setWindowTitle("*" + defaultTitle);
    }
}

void MainWindow::setStateForAllCells(bool existed)
{
    setSaveState(false);
    for(QList<Cell*>::iterator it = m_cellList.begin(); it != m_cellList.end(); ++it)
    {
        (*it)->setState(existed);
    }
}

void MainWindow::on_enableAll_clicked()
{
    setStateForAllCells(true);
}
