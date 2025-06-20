#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QThread>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QDir>
#include <QtWidgets/QTextEdit>

// файл служебных функций

void error_file(){ //  Ошибка на случай если файл с ядром шифра пропал или не имеет доступа
    QMessageBox msgBox;  // диалоговое окно
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText("Исполняемый файл с ядром шифра либо отсутствует либо не имеет достаточно прав доступа для запуска");  // устанавливаем текст
    msgBox.exec();
}

void writeFileFromData(const QString &data, const QString &newFilePath) {
    QByteArray byteArray;
    QStringList hexValues = data.split(" ", QString::SkipEmptyParts);

    for (const QString &hex : hexValues) {
        bool ok;
        char byte = static_cast<char>(hex.toUInt(&ok, 16));
        if (ok) {
            byteArray.append(byte);
        }
    }

    QFile newFile(newFilePath);
    if (!newFile.open(QIODevice::WriteOnly)) {
        // Обработка ошибки открытия файла
        return;
    }

    newFile.write(byteArray);
    newFile.close();
}

// Метод gen_key генерирует ключ передавая в отдельную программу(мой генератор) аргументы для генерации ключа, для разных алгоритмов шифрования аргументы различаются
// также в функцию передаётся указатель на функцию, которая выводит ключ в нужное поле(Для RSA одни поля для AES другие)
int MainWindow::gen_key(int argc, QString argv[], const QString& file_genkey, ReadKeyFunction readkey)
{
    QFile file(file_genkey);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Не удалось открыть файл для записи!");
        return -1;
    }
    file.close();

    QString workingDir = QDir::currentPath();

    // Создаем объект для наблюдения за файлами
    QFileSystemWatcher* watcher = new QFileSystemWatcher(this);

    // Добавляем файл для наблюдения
    if (!watcher->addPath(file_genkey)) {
        qWarning("Не удалось добавить файл в наблюдатель!");
        delete watcher;
        return -1;
    }

    // Сигнал, который будет срабатывать при изменении файла
    connect(watcher, &QFileSystemWatcher::fileChanged, this, [this, readkey, file_genkey](const QString& path) {
        if (path == file_genkey) {
            readkey(file_genkey, ui); // Используем переданную функцию
        }
    });

    // Определяем путь к программе в зависимости от ОС
    QString programPath;
#ifdef Q_OS_WIN
    programPath = workingDir + "/GEN_key.exe";
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    programPath = workingDir + "/GEN_key";
#endif

    // Создаем объект QProcess
    QProcess* process = new QProcess(this);

    // Устанавливаем рабочую директорию
    process->setWorkingDirectory(workingDir);

    // Указываем аргументы для запускаемой программы
    QStringList arguments;
    for (int i = 0; i < argc; i++) {
        arguments << argv[i];
    }

    // Запускаем программу с аргументами
    process->start(programPath, arguments);

    // Проверяем, запустилась ли программа успешно
    if (!process->waitForStarted()) {
        qWarning("Не удалось запустить программу!");
        delete watcher;
        delete process;
        error_file();
        return -1;
    }

    qDebug() << "Программа запущена успешно!";
    return 0;
}
