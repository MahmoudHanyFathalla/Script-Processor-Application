#ifndef CODE_H
#define CODE_H

#include <QString>

class Code {
public:
    Code();
    void browseFolder();
    int countImagesInFolder(const QString& folderPath);
    void radioButtonClicked();
    void executePythonCode();
};

#endif // CODE_H
