#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QProgressBar>
#include <QTextEdit>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void browseFolder();
    void radioButtonClicked();
    void executePythonCode();

private:
    Ui::MainWindow *ui;
    int selectedOption;
   // QString folderPath;
   // int countImagesInFolder(const QString &folderPath);
    QString outputFolder;
    QString inputFolder;
  //  QString scriptFolderName;


};

//#include "funs.cpp" // Include the definition of non-UI functions

#endif // MAINWINDOW_H
/*
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QProgressBar>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void browseFolder();
    void radioButtonClicked();
    void executePythonCode();

private:
    Ui::MainWindow *ui;
    QString inputFolder;
    QString outputFolder;
    int selectedOption;
};
#endif // MAINWINDOW_H


*/
