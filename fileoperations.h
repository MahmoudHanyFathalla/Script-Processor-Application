#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H
#include <QString>
#include <QProgressBar>
#include <QRadioButton>
#include <QTextEdit>

class fileoperations
{
public:
    fileoperations();
    static int countImagesInFolder(const QString& folderPath);
    static int getSelectedOption(QRadioButton* radioButton1, QRadioButton* radioButton2, QRadioButton* radioButton3);
    static void executePythonCode(const QString& inputFolder, const QString& outputFolder, int selectedOption, QTextEdit* textEdit, QProgressBar* progressBar);

};

#endif // FILEOPERATIONS_H

