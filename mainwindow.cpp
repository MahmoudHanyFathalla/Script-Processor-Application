#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileoperations.h" // Include the header file

#include <QFileDialog>
#include <QDirIterator>
#include <QTimer>
#include <QProcess>
#include <QSettings>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    try {
        ui->setupUi(this);
        ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::browseFolder);
        connect(ui->radioButton, &QRadioButton::clicked, this, &MainWindow::radioButtonClicked);
        connect(ui->radioButton_2, &QRadioButton::clicked, this, &MainWindow::radioButtonClicked);
        connect(ui->radioButton_3, &QRadioButton::clicked, this, &MainWindow::radioButtonClicked);

        // Connect push button signal to slot
        connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::executePythonCode);

        ui->progressBar->setValue(0);
        // Read config file to get the output folder path
        QSettings settings("config.ini", QSettings::IniFormat);
        outputFolder = settings.value("Paths/output_folder").toString();
        // Check if the output folder exists, if not, create it
        QDir outputDir(outputFolder);
        if (!outputDir.exists()) {
            if (!outputDir.mkpath(".")) {
                qDebug() << "Failed to create output folder: " << outputFolder;
                // Handle the failure to create the folder, if necessary
            }
        }
    } catch (const std::exception& e) {
        // Handle exception (e.g., display error message)
        qDebug() << "Error browsing folder:" << e.what();
    }
}

void MainWindow::browseFolder()
{
    try {
        QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"), QDir::homePath());

        if (!folderPath.isEmpty()) {
            ui->textEdit->setText(folderPath);
            int imageCount = fileoperations::countImagesInFolder(folderPath);
            ui->progressBar->setMaximum(imageCount);
            inputFolder = ui->textEdit->toPlainText();
        }
    } catch (const std::exception& e) {
        // Handle exception (e.g., display error message)
        qDebug() << "Error browsing folder:" << e.what();
    }
}

void MainWindow::radioButtonClicked()
{
    try {
        // Determine which radio button is checked and set the selected option accordingly
        selectedOption = fileoperations::getSelectedOption(ui->radioButton, ui->radioButton_2, ui->radioButton_3);
    } catch (const std::exception& e) {
        // Handle exception (e.g., display error message)
        qDebug() << "Error browsing folder:" << e.what();
    }
}

void MainWindow::executePythonCode()
{
    try {
        // Execute the Python code using FileOperations
        fileoperations::executePythonCode(inputFolder, outputFolder, selectedOption, ui->textEdit, ui->progressBar);
    } catch (const std::exception& e) {
        // Handle exception (e.g., display error message)
        qDebug() << "Error executing Python code:" << e.what();
    }
}

MainWindow::~MainWindow()
{
    QString deleteCommand2;
    #ifdef _WIN32
         deleteCommand2 = "powershell -Command \"Remove-Item -Path '" + outputFolder + "' -Recurse -Force\"";
    #else
         deleteCommand2 = "rm -rf '" + outputFolder + "'";
    #endif
    QProcess::execute(deleteCommand2);
    delete ui;
}
