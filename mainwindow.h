#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cell.h"
#include <QList>
#include <QString>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

const QString defaultTitle = "Редактор полей";
typedef enum
{
    ENABLE_CELLS = 0,
    ADD_TILES,
    ADD_LOCKED
}EditorState;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_disableAll_clicked();
    void on_enableAll_clicked();

    void on_comboBox_currentIndexChanged(int index);

    //слоты меню
    void on_saveAs_triggered();
    void on_exit_triggered();
    void on_save_triggered();
    void on_open_triggered();

    void on_about_triggered();

private:
    Ui::MainWindow *ui;
    QList<Cell*> m_cellList;//контейнер с ячейками поля
    bool m_isChangesSaved;//был ли сохранен текущий файл или нет
    QString m_fileName;//имя сохраненного файла
    EditorState m_eState;//режим редактирования

    void setStateForAllCells(bool existed);//включаем/выключаем ячейки
    void saveXML();//сохранение поля в файл
    void loadXML();//загрузка поля

    int onCloseMessageBox(const QString title = "Выход без сохранения",
                          const QString description = "Сохранить изменения?");
    //создает диалоговое окно для случая, когда пользователь выходит без сохранения

    void createFileDialog(QFileDialog::AcceptMode mode);

    virtual void closeEvent(QCloseEvent *e);

public:
    void setSaveState(bool isSaved);
    EditorState getEditorState(){return m_eState;}
};

#endif // MAINWINDOW_H
