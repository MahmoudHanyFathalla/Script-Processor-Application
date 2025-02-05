#include "fileoperations.h"
#include <QDirIterator>
#include <QProcess>
#include <QSettings>
#include <QDateTime>
#include <QTimer>
#include <QDebug>

int fileoperations::countImagesInFolder(const QString& folderPath)
{
    try {
        QDirIterator it(folderPath, QStringList() << "*.jpg" << "*.png" << "*.jpeg" << "*.txt", QDir::Files, QDirIterator::Subdirectories);
        int count = 0;
        while (it.hasNext()) {
            it.next();
            count++;
        }
        return count;
    } catch (const std::exception& e) {
        qDebug() << "An error occurred while counting images:" << e.what();
        return -1; // or any other error code you want to use
    }
}

int fileoperations::getSelectedOption(QRadioButton* radioButton1, QRadioButton* radioButton2, QRadioButton* radioButton3)
{
    if (radioButton1->isChecked())
        return 1;
    else if (radioButton2->isChecked())
        return 2;
    else if (radioButton3->isChecked())
        return 3;
    else
        return -1; // Or handle the case when no radio button is selected
}

void fileoperations::executePythonCode(const QString& inputFolder, const QString& outputFolder, int selectedOption, QTextEdit* textEdit, QProgressBar* progressBar)
{
    try {
        QSettings settings("config.ini", QSettings::IniFormat);
        QString outputFolder = settings.value("Paths/output_folder").toString();
        QString pythonScript1 = settings.value("Paths/pythonScript1").toString();
        QString pythonScript2 = settings.value("Paths/pythonScript2").toString();
        QString pythonScript3 = settings.value("Paths/pythonScript3").toString();

        if (inputFolder.isEmpty()) {
            qDebug() << "Please select a folder path before executing the Python script.";
            return;
        }

        QString pythonScript;

        switch(selectedOption)
        {
        case 1:
            pythonScript = pythonScript1;
            break;
        case 2:
            pythonScript = pythonScript2;
            break;
        case 3:
            pythonScript = pythonScript3;
            break;
        }
        QString zipFilePath = inputFolder+ ".zip";
        QDir inputDir(inputFolder);
        QString inputDirName = inputDir.dirName();
        QString zipCommand;
        #ifdef _WIN32
          zipCommand = "powershell -Command \"Compress-Archive -Path '" + inputFolder + "' -DestinationPath '" + zipFilePath + "'\"";
        #else
          zipCommand = "zip -r '" + zipFilePath + "' '" + inputFolder + "'";
        #endif
        QProcess::execute(zipCommand);


        QTimer* timer = new QTimer();
        QObject::connect(timer, &QTimer::timeout, [=]() {
            int generatedImageCount = countImagesInFolder(outputFolder);
            progressBar->setValue(generatedImageCount);
            if (generatedImageCount >= progressBar->maximum()) {
                QDir inputDir(inputFolder);
                if (inputDir.exists()) {
                    QString deleteCommand;
                   #ifdef _WIN32
                     deleteCommand = "powershell -Command \"Remove-Item -Path '" + inputFolder + "' -Recurse -Force\"";
                    #else
                     deleteCommand = "rm -rf '" + inputFolder + "'";
                    #endif
                    QProcess::execute(deleteCommand);
                } else {
                    qDebug() << "Input folder does not exist: " << inputFolder;
                }
                QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
                QString zipFilePath2 = outputFolder + "_" + timestamp + ".zip";
                QDir inputDir2(outputFolder);
                QString inputDirName2 = inputDir2.dirName();

                QString zipCommand2;
                QString deleteCommand2;

                #ifdef _WIN32
                   zipCommand2 = "powershell -Command \"Compress-Archive -Path '" + outputFolder + "' -DestinationPath '" + zipFilePath2 + "'\"";
                   deleteCommand2 = "powershell -Command \"Remove-Item -Path '" + outputFolder + "' -Recurse -Force\"";
                #else
                   zipCommand2 = "zip -r '" + zipFilePath2 + "' '" + outputFolder + "'";
                   deleteCommand2 = "rm -rf '" + outputFolder + "'";
                #endif
                QProcess::execute(zipCommand2);
                QProcess::execute(deleteCommand2);
            }
        });
        timer->start(100);

        QStringList arguments;
        arguments << inputFolder << outputFolder.trimmed();
        QProcess::startDetached("python", QStringList() << pythonScript << arguments);
    } catch(const std::exception& e) {
        qDebug() << "Error executing Python code:" << e.what();
    }
}
