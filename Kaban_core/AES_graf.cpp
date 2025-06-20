#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QFileSystemWatcher>
// здесь описаны графические функции для алгоритма шифрования AES


// Функция для чтения ключа
void read_key_AES(const QString& save_key, Ui::MainWindow* ui)
{
    QFile file(save_key);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Не удалось открыть файл для чтения!");
        return;
    }

    QByteArray data = file.readAll(); // Считываем все данные из файла
    ui->lineEdit_5->setText(QString(data)); // Выводим данные в lineEdit
    file.close();
}

void Shif_file_AES(QString key, QString myFile,QString Shif_file){
    // отображаем диалоговое окно

    if(myFile != "" && Shif_file != ""){
        // Получаем текущую директорию
        QString workingDir = QDir::currentPath();

        #ifdef Q_OS_WIN
            QString programPath = workingDir + "/AES_C.exe";
        #elif defined(Q_OS_LINUX)
            QString programPath = workingDir + "/AES_C";
        #elif defined(Q_OS_MAC)
            QString programPath = workingDir + "/AES_C";
        #endif
        // Создаем объект QProcess
        QProcess *process = new QProcess();

        // Устанавливаем рабочую директорию
        process->setWorkingDirectory(workingDir);

        // Указываем аргументы для запускаемой программы
        QStringList arguments;
        arguments << myFile << Shif_file<< key<<"0"; // Здесь добавьте ваши аргументы

        // Запускаем программу с аргументами
        process->start(programPath, arguments);

        // Проверяем, запустилась ли программа успешно
        if (!process->waitForStarted()) {
            qWarning("Не удалось запустить программу!");
            error_file();
        }

        qDebug() << "Программа запущена успешно!";
    }
}

void raShif_file_AES(QString key, QString Shif_file,QString raShif_file)
{
    if(raShif_file != "" && Shif_file != ""){
        // Получаем текущую директорию
        QString workingDir = QDir::currentPath();

        // Указываем путь к программе, которую хотим запустить
        #ifdef Q_OS_WIN
        QString programPath = workingDir + "/AES_C.exe";
        #elif defined(Q_OS_LINUX)
        QString programPath = workingDir + "/AES_C";
        #elif defined(Q_OS_MAC)
        QString programPath = workingDir + "/AES_C";
        #endif

        // Создаем объект QProcess
        QProcess *process = new QProcess();

        // Устанавливаем рабочую директорию
        process->setWorkingDirectory(workingDir);

        // Указываем аргументы для запускаемой программы
        QStringList arguments;
        arguments << Shif_file << raShif_file<< key<<"1"; // Здесь добавьте ваши аргументы

        // Запускаем программу с аргументами
        process->start(programPath, arguments);

        // Проверяем, запустилась ли программа успешно
        if (!process->waitForStarted()) {
            qWarning("Не удалось запустить программу!");
            error_file();
        }

        qDebug() << "Программа запущена успешно!";
    }
}

// AES графические функции
void MainWindow::on_pushButton_55_clicked() // шифрование текста
{
    QMessageBox msgBox;  // диалоговое окно
    QString isx_text = ui->textEdit_20->toPlainText();
    QString key=ui->lineEdit_5->text();
    QFile file("last_isx_text_AES");
    QFile file1("last_shif_text_AES");
    if(file.open(QIODevice::WriteOnly) && file1.open(QIODevice::WriteOnly) && isx_text != ""){
        file.write(isx_text.toUtf8()); // Сохраняет
        file.close();
        file1.close();


        QString fileKey = "last_shif_text_AES"; // Замените на ваш путь к файлу

        // Создаем объект для наблюдения за файлами
        QFileSystemWatcher *watcher = new QFileSystemWatcher(this);

        // Добавляем список путей к файлам или папкам для наблюдения
        QStringList paths = { "last_shif_text_AES" };
        foreach (const QString &path, paths) {
            watcher->addPath(path);
        }
        // Сигнал, который будет срабатывать при изменении файла
        connect(watcher, &QFileSystemWatcher::fileChanged, this, [this](const QString &path) {
            QFile file("last_shif_text_AES");
            if (!file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
                return;
            QByteArray fileContents = file.readAll();
            QString abstractRepresentation;
            for (char byte : fileContents) {
                abstractRepresentation.append(QString::number(static_cast<unsigned char>(byte), 16) + " ");
            }
            ui->textEdit_19->setPlainText(abstractRepresentation);
            //tex->append(abstractRepresentation);
            file.close();
        });


        Shif_file_AES(key, "last_isx_text_AES", "last_shif_text_AES");
    }
}

void MainWindow::on_pushButton_48_clicked() // расшифровка текста
{
    QMessageBox msgBox;  // диалоговое окно
    QString text_for_rash = ui->textEdit_20->toPlainText();
    QString key=ui->lineEdit_5->text();
    QFile file("last_shif_text_AES");
    QFile file1("last_rashif_text_AES");
    QByteArray fileContents;
    QByteArray filelo;
    if(file.open(QIODevice::WriteOnly) && file1.open(QIODevice::WriteOnly) && text_for_rash != ""){
        //saveFile(ui->textEdit_5->toPlainText());
        writeFileFromData(text_for_rash, "last_shif_text_AES");
        file1.close();
        QFileSystemWatcher *watcher = new QFileSystemWatcher(this);

        // Добавляем список путей к файлам или папкам для наблюдения
        QStringList paths = { "last_rashif_text_AES" };
        foreach (const QString &path, paths) {
            watcher->addPath(path);
        }

        // Сигнал, который будет срабатывать при изменении файла
        connect(watcher, &QFileSystemWatcher::fileChanged, this, [this](const QString &path) {
            QFile filesc("last_rashif_text_AES");
            if (!filesc.open(QIODevice::ReadOnly))
                return;
            QByteArray byteArray;
            byteArray = filesc.readAll();
            ui->textEdit_19->setPlainText(QString(byteArray));
        });

        raShif_file_AES(key, "last_shif_text_AES", "last_rashif_text_AES");
    }
}


void MainWindow::on_pushButton_5_clicked() // шифрование файла
{
    QString key=ui->lineEdit_5->text(); // QString s;
    QString myFile = QFileDialog::getOpenFileName();
    if(myFile != ""){
        QString Shif_file = QFileDialog::getSaveFileName(this, "Сохранить как");
        Shif_file_AES(key, myFile, Shif_file);
    }
}


void MainWindow::on_pushButton_8_clicked() // расшифровка файла
{
    QString key=ui->lineEdit_5->text(); // QString s;
    QString Shif_file = QFileDialog::getOpenFileName();
    if(Shif_file != ""){
        QString raShif_file = QFileDialog::getSaveFileName(this, "Сохранить как");
        raShif_file_AES(key, Shif_file, raShif_file);
    }

}


void MainWindow::on_pushButton_6_clicked() // сохранение ключа
{
    QString save_key = QFileDialog::getSaveFileName(this, "Сохранить как");
    QFile file(save_key);
    if (file.open(QIODevice::WriteOnly) && save_key != "")
    {
        QString text = ui->lineEdit_5->text();
        file.write(text.toUtf8()); // Сохраняет
        file.close();
        QMessageBox msgBox;  // диалоговое окно
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Ключ успешно сохранён");  // устанавливаем текст
        msgBox.exec();
    }
    file.close();
}


void MainWindow::on_pushButton_7_clicked() // считание ключа
{
    QString scan_key = QFileDialog::getOpenFileName();
    QFile file(scan_key);
    if(scan_key != ""){
        QByteArray data; // Создаем объект класса QByteArray, куда мы будем считывать данные
        if (!file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
            return; // если это сделать невозможно, то завершаем функцию
        data = file.readAll(); //считываем все данные с файла в объект data
        ui->lineEdit_5->setText(QString(data));  // Выводим данные в lineEdit, предварительно создав строку из полученных данных
        file.close();
    }
}


void MainWindow::on_pushButton_56_clicked() // генерация ключа
{
    QMessageBox msgBox;  // диалоговое окно
    QString save_key = QFileDialog::getSaveFileName(this, "Сохранить как");
    QPoint globalPos = QCursor::pos(); // получаем координаты мыши и переdодим их в текст
    int x = globalPos.x();
    int y = globalPos.y();
    QString xStr = QString::number(x);
    QString yStr = QString::number(y);
    QString  dop_sid = ui->lineEdit_6->text();
    if (dop_sid == "" ) dop_sid = "1232131242342";
    QString argv[] = {"-g", "-s", dop_sid, save_key, xStr, yStr};
    gen_key(4, argv, save_key, read_key_AES);
}
