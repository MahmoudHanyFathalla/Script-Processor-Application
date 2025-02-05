#include "code.h"

#include <QFileDialog>
#include <QDirIterator>
#include <QTimer>
#include <QProcess>
#include <QSettings>
#include <QDebug>


Code::Code() {}

void Code::browseFolder()
{
    try{
        QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"), QDir::homePath());

        if (!folderPath.isEmpty()) {
            ui->textEdit->setText(folderPath);
            int imageCount = countImagesInFolder(folderPath);
            ui->progressBar->setMaximum(imageCount);
            inputFolder = ui->textEdit->toPlainText();
        }


    }catch(const std::exception& e) {
        // Handle exception (e.g., display error message)
        qDebug() << "Error browsing folder:" << e.what();
    }
}
int Code::countImagesInFolder(const QString& folderPath)
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


void Code::radioButtonClicked()
{
    try{

        // Determine which radio button is checked and set the selected option accordingly
        if(ui->radioButton->isChecked())
        {
            selectedOption = 1;
        }
        else if(ui->radioButton_2->isChecked())
        {
            selectedOption = 2;
        }
        else if(ui->radioButton_3->isChecked())
        {
            selectedOption = 3;
        }
    }catch(const std::exception& e) {
        // Handle exception (e.g., display error message)
        qDebug() << "Error browsing folder:" << e.what();
    }
}

void Code::executePythonCode()
{
    try{
        // Depending on the selected option, execute the corresponding Python code
        inputFolder = ui->textEdit->toPlainText();
        if (inputFolder.isEmpty()) {
            qDebug() << "Please select a folder path before executing the Python script.";
            return;
        }

        QString pythonScript;

        switch(selectedOption)
        {
        case 1:
            pythonScript = "C:\\Users\\hp\\Desktop\\script\\detection\\detect.py";
            scriptFolderName = "detection";
            break;
        case 2:
            pythonScript = "C:\\Users\\hp\\Desktop\\script\\seg\\seg.py";
            scriptFolderName = "seg";
            break;
        case 3:
            pythonScript = "C:\\Users\\hp\\Desktop\\script\\play\\play.py";
            scriptFolderName = "play";
            break;
        }

        // Get the input folder path from the QTextEdit widget


        // Define the output folder path (replace with your desired output folder path)
        // QString outputFolder = "C:\\Users\\hp\\Desktop\\script\\detection\\out";


        QString zipFilePath = inputFolder+ ".zip";
        QDir inputDir(inputFolder);
        QString inputDirName = inputDir.dirName();
        QString zipCommand = "powershell -Command \"Compress-Archive -Path '" + inputFolder + "' -DestinationPath '" + zipFilePath + "'\"";
        QProcess::execute(zipCommand);




        /*   LINUX COMMANDS
                   QString zipFilePath = inputFolder + QDir::separator() + scriptFolderName + ".zip";
                    QString zipCommand = "zip -r '" + zipFilePath + "' '" + inputFolder + "'";
                    QProcess::execute(zipCommand);

                    // Delete the original folder
                    QString deleteCommand = "rm -rf '" + inputFolder + "'";
                    QProcess::execute(deleteCommand);
       */

        // Update the config file with the selected scriptFolderName
        QSettings settings("config.ini", QSettings::IniFormat);
        QStringList folderNames = settings.value("Folders_Names").toStringList();

        // Append the folder name with index to the configuration file
        int folderIndex = folderNames.size() + 1; // Get the next index
        QString folderEntry = QString::number(folderIndex) + "." + scriptFolderName;
        folderNames.append(folderEntry);
        settings.setValue("Folders_Names", folderNames);



        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, [=]() {
            int generatedImageCount = countImagesInFolder(outputFolder);
            ui->progressBar->setValue(generatedImageCount);
            if (generatedImageCount >= ui->progressBar->maximum()) {
                QDir inputDir(inputFolder);
                if (inputDir.exists()) {
                    QString deleteCommand = "powershell -Command \"Remove-Item -Path '" + inputFolder + "' -Recurse -Force\"";
                    QProcess::execute(deleteCommand);
                } else {
                    qDebug() << "Input folder does not exist: " << inputFolder;
                }
                QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"); // Generate timestamp
                QString zipFilePath2 = outputFolder + "_" + timestamp + ".zip";
                QDir inputDir2(outputFolder);
                QString inputDirName2 = inputDir2.dirName();
                QString zipCommand2 = "powershell -Command \"Compress-Archive -Path '" + outputFolder + "' -DestinationPath '" + zipFilePath2 + "'\"";
                QProcess::execute(zipCommand2);

                QString deleteCommand2 = "powershell -Command \"Remove-Item -Path '" + outputFolder + "' -Recurse -Force\"";
                QProcess::execute(deleteCommand2);

            }
        });
        timer->start(100); // 100 milliseconds interval


        // Execute the Python script using QProcess
        QStringList arguments;
        arguments << inputFolder << outputFolder.trimmed(); // Trim any extra whitespace
        QProcess::startDetached("python", QStringList() << pythonScript << arguments);


    }catch(const std::exception& e) {
        // Handle exception (e.g., display error message)
        qDebug() << "Error browsing folder:" << e.what();
    }
}

