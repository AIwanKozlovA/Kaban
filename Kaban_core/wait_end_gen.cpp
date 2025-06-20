#include "wait_end_gen.h"
#include "QFile"
#include <QFileDialog>
#include <QThread>
Wait_End_gen::Wait_End_gen() {}

Wait_End_gen::Wait_End_gen(QString file_genkey) :
    file_genkey(file_genkey)
{

}

void Wait_End_gen::run()
{
    QString lock_file_genkey = file_genkey;
    QFile file3(lock_file_genkey);
    if (!file3.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
        return;

    QFileInfo fileInfo(file3);
    qint64 size = fileInfo.size();
    qint64 size1 = size;

    /*// Ждем, пока размер файла не изменится
    while (size == size1) {
        QFileInfo fileInfo1(file3);
        size1 = fileInfo1.size();
        //QThread::msleep(100); // Добавляем небольшую задержку для предотвращения избыточной загрузки процессора
    }*/

    emit updateTextEdit("123");
}
