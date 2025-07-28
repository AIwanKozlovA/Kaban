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
#include <QMessageBox>
#include <QClipboard>
#include <QToolTip>
// файл служебных функций

void error_file(){ //  Ошибка на случай если файл с ядром шифра пропал или не имеет доступа
    QMessageBox msgBox;  // диалоговое окно
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText("Исполняемый файл с ядром шифра либо отсутствует либо не имеет достаточно прав доступа для запуска");  // устанавливаем текст
    msgBox.exec();
}


void writeFileFromData(const QString &data, const QString &newFilePath) {
    QByteArray byteArray;
    //QStringList hexValues = data.split(" ", QString::SkipEmptyParts);
    QStringList hexValues = data.split(" ", Qt::SkipEmptyParts);

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

void MainWindow::on_pushButton_109_clicked() // смена языка
{
    QString lang[10] = {"ru","en", "ger", "fr", "esp"};
    int az = 0;
    for(int i = 0; i < 5; i++)
    {
        if (ui->pushButton_109->text() == lang[i] && i < 4){
            ui->pushButton_109->setText(lang[i + 1]);
            az = i + 1;
            yazyk_id = az;
            break;
        }else if(ui->pushButton_109->text() == lang[i] && i == 4){
            ui->pushButton_109->setText(lang[0]);
            az = 0;
            yazyk_id = 0;
            break;
        }
    }
    QFile file("yasuk");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Не удалось открыть файл для записи!");
    }
    file.write(lang[az].toUtf8());
    smena_yazyka(*this,az);
    file.close();
}

// Очищение полей ввода


void MainWindow::on_pushButton_11_clicked() // очищение поля ввода
{
    ui->textEdit_5->setText("");
}


void MainWindow::on_pushButton_95_clicked() // очищение поля ввода
{
    ui->textEdit_6->setText("");
}


void MainWindow::on_pushButton_97_clicked() // очищение поля ввода
{
    ui->textEdit_70->setText("");
}



void MainWindow::on_pushButton_103_clicked()
{
    ui->textEdit_71->setText("");
}


void MainWindow::on_pushButton_105_clicked()
{
    ui->textEdit_119->setText("");
}


void MainWindow::on_pushButton_107_clicked()
{
    ui->textEdit_118->setText("");
}


void MainWindow::on_pushButton_99_clicked()
{
    ui->textEdit_20->setText("");
}


void MainWindow::on_pushButton_101_clicked()
{
    ui->textEdit_19->setText("");
}

// копирование текста из помей ввода

void MainWindow::cop_in_byfer(QString text){

    // Копируем в буфер обмена
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text);
    QString yved[10] = {"Текст скопирован!", "The text has been copied!", "Der Text wurde kopiert!", "Texte copié!", "¡Texto copiado!"};
    // Опционально: показать уведомление
    QToolTip::showText(QCursor::pos(), yved[yazyk_id], this);
}


void MainWindow::on_pushButton_9_clicked()
{
    // Получаем текст из QTextEdit
    //QString text = ui->textEdit_5->toPlainText();
    cop_in_byfer(ui->textEdit_5->toPlainText());
    /*// Копируем в буфер обмена
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text);
    QString yved[10] = {"Текст скопирован!", "The text has been copied!", "Der Text wurde kopiert!", "Texte copié!", "¡Texto copiado!"};
    // Опционально: показать уведомление
    QToolTip::showText(QCursor::pos(), yved[yazyk_id], this);*/
}


void MainWindow::on_pushButton_96_clicked()
{
    cop_in_byfer(ui->textEdit_6->toPlainText());
}


void MainWindow::on_pushButton_98_clicked()
{
    // Получаем текст из QTextEdit
    //QString text = ui->textEdit_70->toPlainText();
    cop_in_byfer(ui->textEdit_70->toPlainText());
}


void MainWindow::on_pushButton_104_clicked()
{
    cop_in_byfer(ui->textEdit_71->toPlainText());
}


void MainWindow::on_pushButton_106_clicked()
{
    cop_in_byfer(ui->textEdit_119->toPlainText());
}


void MainWindow::on_pushButton_108_clicked()
{
    cop_in_byfer(ui->textEdit_118->toPlainText());
}


void MainWindow::on_pushButton_100_clicked()
{
    cop_in_byfer(ui->textEdit_20->toPlainText());
}


void MainWindow::on_pushButton_102_clicked()
{
    cop_in_byfer(ui->textEdit_19->toPlainText());
}

void MainWindow::on_pushButton_110_clicked() // смена темы
{
    QFile file("tema");
    if (file.open(QIODevice::WriteOnly))
    {
        toggleTheme();
        if(ui->pushButton_110->text() == "☀"){
            ui->pushButton_110->setText("🌕");
            file.write(QString("🌕").toUtf8());
        } else if(ui->pushButton_110->text() == "🌕"){
            ui->pushButton_110->setText("☀");
            file.write(QString("☀").toUtf8());
        }
        file.close();
    }
}




