#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QCloseEvent>
#include <QDialogButtonBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_isChangesSaved(true),
    m_fileName(QString()),
    m_eState(ENABLE_CELLS)
{
    ui->setupUi(this);

    //создаем ячейки
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            Cell *cell = new Cell(this);
            m_cellList.append(cell);
            ui->fieldLayout->addWidget(m_cellList.back(),i,j);
        }
    }

    ui->buttonsLayout->setAlignment(Qt::AlignLeft|Qt::AlignTop);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_disableAll_clicked()
{
    setStateForAllCells(false);
}

void MainWindow::loadXML()
{
    QFile file(m_fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Ошибка", "Открыть файл не удалось.");
    }
    else
    {
        bool isFileValid = false;
        int fieldIndex = 0;
        QXmlStreamReader reader(&file);
        while(!reader.atEnd() && !reader.hasError())
        {
            QXmlStreamReader::TokenType token = reader.readNext();
            if(token == QXmlStreamReader::StartDocument)
                continue;
            if(token == QXmlStreamReader::StartElement)
            {
                if(reader.name() == "field")// проверка является ли файл вообще полем
                                            //тэг field должен быть первым
                {
                    isFileValid = true;
                    continue;
                }
                if(!isFileValid)
                {
                    QMessageBox::critical(this,"Неверный файл","Данный файл не является полем");
                    break;
                }

                if(reader.name() == "cell")//парсим ячейки
                {
                    QXmlStreamAttributes attributes = reader.attributes();

                    m_cellList.at(fieldIndex)->setState(true);
                    m_cellList.at(fieldIndex)->setState(true);
                    m_cellList.at(fieldIndex)->setLockedLevel(0);

                    if(attributes.hasAttribute("existed"))//активна ли ячейка
                    {
                        m_cellList.at(fieldIndex)->setState((bool)(attributes.value("existed").toInt()));
                    }

                    if(attributes.hasAttribute("tile"))//наличие плитки
                    {
                        m_cellList.at(fieldIndex)->setTileState((bool)(attributes.value("tile").toInt()));
                    }

                    if(attributes.hasAttribute("chains"))//цепи
                    {
                        m_cellList.at(fieldIndex)->setLockedLevel((bool)(attributes.value("chains").toInt()));
                    }

                    ++fieldIndex;
                }
            }
        }
        if(reader.hasError())
            QMessageBox::critical(this,"Ошибка чтения",reader.errorString());
        reader.clear();
    }
}

void MainWindow::saveXML()
{
   QFile file(m_fileName);
   if (!file.open(QIODevice::WriteOnly))
   {
     QMessageBox::critical(this, "Ошибка", "Запись не удалась.");
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
                    QString paramValue = QString::number(static_cast<int>((*it)->getState()));
                    writer.writeAttribute("existed",paramValue);

                    paramValue = QString::number(static_cast<int>((*it)->getTileState()));
                    writer.writeAttribute("tile",paramValue);

                    paramValue = QString::number((*it)->getNumOfChains());
                    writer.writeAttribute("chains",paramValue);
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

int MainWindow::onCloseMessageBox(const QString title, const QString description)
{
    QMessageBox msgBox;
    msgBox.setText(title);
    msgBox.setInformativeText(description);
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec();
}

//отлавливаем закрытие приложения и предлагаем пользователю сохранить изменения
void MainWindow::closeEvent(QCloseEvent *e)
{
    if(!m_isChangesSaved)
    {
        switch(onCloseMessageBox())
        {
            case QMessageBox::Save:
                on_saveAs_triggered();
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

//сохранены изменения или нет
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

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    m_eState = static_cast<EditorState>(index);
}

void MainWindow::on_saveAs_triggered()
{
    QFileDialog saveDialog(this);
    saveDialog.setNameFilter(tr("XML-файлы (*.xml)"));
    saveDialog.setAcceptMode(QFileDialog::AcceptSave);
    if(saveDialog.exec())
    {
        m_fileName = saveDialog.selectedFiles().at(0);
        saveXML();
    }
}

void MainWindow::on_exit_triggered()
{
    close();
}

void MainWindow::on_save_triggered()
{
    if(m_fileName.isEmpty())
    {
        on_saveAs_triggered();
    }
    else
    {
        saveXML();
    }
}

void MainWindow::on_open_triggered()
{
    if(!m_isChangesSaved)//проверяем был ли сохранен текущий файл, если нет, то делаем запрос на сохранение
    {

        switch(onCloseMessageBox("Закрытие файла без сохранения"))
        {
            case QMessageBox::Save:
                on_saveAs_triggered();
            break;
            case QMessageBox::Discard:
                return;
            break;
            default:
            break;
        }
    }
    QFileDialog openDialog(this);//переделать
    openDialog.setNameFilter(tr("XML-файлы (*.xml)"));
    openDialog.setAcceptMode(QFileDialog::AcceptOpen);
    if(openDialog.exec())
    {
        m_fileName = openDialog.selectedFiles().at(0);
        loadXML();
    }
}

void MainWindow::on_about_triggered()
{
    QMessageBox::about(this,"О программе","Данная программа предназначена для создания полей для игр в жанре match-3.\n"
                       "Автор: Пинаев Дмитрий. 2014г.");
}
