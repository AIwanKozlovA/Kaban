#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QThread>
#include <mainwindow.moc>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QDir>
#include <QtWidgets/QTextEdit>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// RSA графические функции
void Shif_file(QString e, QString n, QString myFile,QString Shif_file){
    if(myFile != "" && Shif_file != ""){
        /*QMessageBox msgBox;  // диалоговое окно
        msgBox.setText("Шифрование ./RSA \""+ Shif_file+ "\" \"" + Shif_file + "\" \"" + myFile + "\" 0" + e+ " "+ n);  // устанавливаем текст
        msgBox.exec();*/


        // Получаем текущую директорию
        QString workingDir = QDir::currentPath();
        // Указываем путь к программе, которую хотим запустить
        #ifdef Q_OS_WIN
                QString programPath = workingDir + "/RSA_C.exe";
        #elif defined(Q_OS_LINUX)
                QString programPath = workingDir + "/RSA_C";
        #elif defined(Q_OS_MAC)
                QString programPath = workingDir + "/RSA_C";
        #endif
        //QString programPath = workingDir + "/RSA_C"; // Замените "proga" на имя вашего исполняемого файла

        // Создаем объект QProcess
        QProcess *process = new QProcess();

        // Устанавливаем рабочую директорию
        process->setWorkingDirectory(workingDir);

        // Указываем аргументы для запускаемой программы
        QStringList arguments;
        arguments << myFile << Shif_file<< e << n << "0"; // Здесь добавьте ваши аргументы

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

void raShif_file(QString d, QString n, QString name_shif_file,QString name_rashif_file){
    if(name_shif_file != "" && name_rashif_file != ""){
        /*QMessageBox msgBox;  // диалоговое окно
        msgBox.setText("Шифрование ./RSA \""+ Shif_file+ "\" \"" + Shif_file + "\" \"" + myFile + "\" 0" + e+ " "+ n);  // устанавливаем текст
        msgBox.exec();*/


        // Получаем текущую директорию
        QString workingDir = QDir::currentPath();

        // Указываем путь к программе, которую хотим запустить
        #ifdef Q_OS_WIN
            QString programPath = workingDir + "/RSA_C.exe";
        #elif defined(Q_OS_LINUX)
            QString programPath = workingDir + "/RSA_C";
        #elif defined(Q_OS_MAC)
            QString programPath = workingDir + "/RSA_C";
        #endif
        //QString programPath = workingDir + "/RSA_C"; // Замените "proga" на имя вашего исполняемого файла

        // Создаем объект QProcess
        QProcess *process = new QProcess();

        // Устанавливаем рабочую директорию
        process->setWorkingDirectory(workingDir);

        // Указываем аргументы для запускаемой программы
        QStringList arguments;
        arguments << name_shif_file << name_rashif_file << d << n << "1"; // Здесь добавьте ваши аргументы

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

void MainWindow::on_pushButton_70_clicked() // шифруем файл
{
    QString e=ui->lineEdit_15->text();
    QString n=ui->textEdit_69->toPlainText();
    QString myFile= QFileDialog::getOpenFileName();
    if(myFile != ""){
        QString name_shif_file = QFileDialog::getSaveFileName(this, "Сохранить как");
        Shif_file(e,n, myFile, name_shif_file);
    }
}


void MainWindow::on_pushButton_3_clicked() // шифруем файл
{
    QString e=ui->lineEdit->text();
    QString n=ui->textEdit_3->toPlainText();
    QString myFile= QFileDialog::getOpenFileName();
    if(myFile != ""){
        QString name_shif_file = QFileDialog::getSaveFileName(this, "Сохранить как");
        Shif_file(e,n, myFile, name_shif_file);
    }
}


void MainWindow::on_pushButton_50_clicked() // расшифруем файл
{
    QString d=ui->textEdit_14->toPlainText();
    QString n=ui->textEdit_15->toPlainText();
    QString name_shif_file= QFileDialog::getOpenFileName();
    if(name_shif_file != ""){
        QString name_rashif_file = QFileDialog::getSaveFileName(this, "Сохранить как");
        //Shif_file(e,n, myFile, name_shif_file);
        raShif_file(d, n, name_shif_file, name_rashif_file);
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    QString d=ui->textEdit->toPlainText();
    QString n=ui->textEdit_3->toPlainText();
    QString name_shif_file= QFileDialog::getOpenFileName();
    if(name_shif_file != ""){
        QString name_rashif_file = QFileDialog::getSaveFileName(this, "Сохранить как");
        //Shif_file(e,n, myFile, name_shif_file);
        raShif_file(d, n, name_shif_file, name_rashif_file);
    }
}


void MainWindow::on_pushButton_15_clicked()
{
    QString save_key = QFileDialog::getSaveFileName(this, "Сохранить как");
    QFile file(save_key);
    if (file.open(QIODevice::WriteOnly) && save_key != "")
    {
        QString p = ui->textEdit_2->toPlainText();
        if (p.back() != "\n") p += "\n";
        file.write(p.toUtf8()); // Сохраняет

        QString q = ui->textEdit_4->toPlainText();
        if (q.back() != "\n") q += "\n";
        file.write(q.toUtf8()); // Сохраняет

        QString e = ui->lineEdit->text();
        if (e.back() != "\n") e += "\n";
        file.write(e.toUtf8()); // Сохраняет

        QString n = ui->textEdit_3->toPlainText();
        if (n.back() != "\n") n += "\n";
        file.write(n.toUtf8()); // Сохраняет

        QString d = ui->textEdit->toPlainText();
        if (d.back() != "\n") d += "\n";
        file.write(d.toUtf8()); // Сохраняет
        file.close();
        QMessageBox msgBox;  // диалоговое окно
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Ключ успешно сохранён");  // устанавливаем текст
        msgBox.exec();
    }
    file.close();
}
void read_file_key(const QString& file_name, Ui::MainWindow* ui){ // читаем файл с целым сключом
    QFile file(file_name);
    //QByteArray data; // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (!file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
        return; // если это сделать невозможно, то завершаем функцию
    QByteArray p = file.readLine(); //считываем все данные с файла в объект data
    ui->textEdit_2->setText(QString(p));

    QByteArray q = file.readLine(); //считываем все данные с файла в объект data
    ui->textEdit_4->setText(QString(q));


    QByteArray e = file.readLine(); //считываем все данные с файла в объект data
    ui->lineEdit->setText(QString(e));  // Выводим данные в lineEdit, предварительно создав строку из полученных данных

    QByteArray n = file.readLine(); //считываем все данные с файла в объект data
    ui->textEdit_3->setText(QString(n));

    QByteArray d = file.readLine(); //считываем все данные с файла в объект data
    ui->textEdit->setText(QString(d));
    file.close();
}


void MainWindow::on_pushButton_13_clicked()
{
    QString scan_key = QFileDialog::getOpenFileName();
    QFile file(scan_key);
    if(scan_key != ""){
        read_file_key(scan_key, ui);
    }
}
void MainWindow::shif_text(QString e, QString n,QString isx_text, QTextEdit* text_field){ // шифрование текста
    QFile file("last_isx_text");
    QFile file1("last_shif_text");
    if(file.open(QIODevice::WriteOnly) && file1.open(QIODevice::WriteOnly) && isx_text != ""){
        file.write(isx_text.toUtf8()); // Сохраняет
        file.close();
        file1.close();


        QString fileKey = "last_shif_text"; // Замените на ваш путь к файлу

        // Создаем объект для наблюдения за файлами
        QFileSystemWatcher *watcher = new QFileSystemWatcher(this);

        // Добавляем список путей к файлам или папкам для наблюдения
        QStringList paths = { "last_shif_text" };
        foreach (const QString &path, paths) {
            watcher->addPath(path);
        }
        // Сигнал, который будет срабатывать при изменении файла
        connect(watcher, &QFileSystemWatcher::fileChanged, this, [this, text_field](const QString &path) {
            QFile file("last_shif_text");
            if (!file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
                return;
            QByteArray fileContents = file.readAll();
            QString abstractRepresentation;
            for (char byte : fileContents) {
                abstractRepresentation.append(QString::number(static_cast<unsigned char>(byte), 16) + " ");
            }
            text_field->setPlainText(abstractRepresentation);
            //tex->append(abstractRepresentation);
            file.close();
        });


        Shif_file(e,n, "last_isx_text", "last_shif_text");
    }
}


void MainWindow::ra_shif_text(QString d, QString n,QString text_for_rash, QTextEdit* text_field){ // расшифровка текста
    QFile file("last_shif_text");
    QFile file1("last_rashif_text");
    QByteArray fileContents;
    QByteArray filelo;
    if(file.open(QIODevice::WriteOnly) && file1.open(QIODevice::WriteOnly) && text_for_rash != ""){
        //saveFile(ui->textEdit_5->toPlainText());
        writeFileFromData(text_for_rash, "last_shif_text");
        file1.close();
        QFileSystemWatcher *watcher = new QFileSystemWatcher(this);

        // Добавляем список путей к файлам или папкам для наблюдения
        QStringList paths = { "last_rashif_text" };
        foreach (const QString &path, paths) {
            watcher->addPath(path);
        }

        // Сигнал, который будет срабатывать при изменении файла
        connect(watcher, &QFileSystemWatcher::fileChanged, this, [this, text_field](const QString &path) {
            QFile filesc("last_rashif_text");
            if (!filesc.open(QIODevice::ReadOnly))
                return;
            QByteArray byteArray;
            byteArray = filesc.readAll();
            text_field->setPlainText(QString(byteArray));
        });

        raShif_file(d,n, "last_shif_text", "last_rashif_text");
    }
}
void MainWindow::on_pushButton_clicked() // шифрование текста
{
    QString isx_text = ui->textEdit_5->toPlainText();
    QString e=ui->lineEdit->text();
    QString n=ui->textEdit_3->toPlainText();
    shif_text(e, n, isx_text, ui->textEdit_6);
}


void MainWindow::on_pushButton_2_clicked() // расшифровка текста
{
    QString d = ui->textEdit->toPlainText();
    QString n=ui->textEdit_3->toPlainText();
    QString text_for_rash = ui->textEdit_5->toPlainText();
    ra_shif_text(d, n, text_for_rash, ui->textEdit_6);
}

void MainWindow::on_pushButton_14_clicked() // генерация ключа и считывание его
{
    QString long_key = ui->lineEdit_2->text();
    QString file_genkey = QFileDialog::getSaveFileName(this, "Сохранить как");
    QString dop_sid = ui->lineEdit_4->text();
    if (dop_sid == "" ) dop_sid = "1232131242342";
    QPoint globalPos = QCursor::pos(); // получаем координаты мыши и переподим их в текст
    int x = globalPos.x();
    int y = globalPos.y();
    QString xStr = QString::number(x);
    QString yStr = QString::number(y);
    QString argv[] = {"-g", long_key, dop_sid, file_genkey, xStr, yStr };
    gen_key(4, argv, file_genkey, read_file_key);
}


void MainWindow::on_pushButton_69_clicked() // быстрое шифрование текста
{
    QString isx_text = ui->textEdit_70->toPlainText();
    QString e=ui->lineEdit_15->text();
    QString n=ui->textEdit_69->toPlainText();
    shif_text(e, n, isx_text, ui->textEdit_71);
}


void MainWindow::on_pushButton_49_clicked() // быстрая расшифровка текста
{
    QString text_for_rash = ui->textEdit_119->toPlainText();
    QString d=ui->textEdit_14->toPlainText();
    QString n=ui->textEdit_15->toPlainText();
    ra_shif_text(d, n, text_for_rash, ui->textEdit_118);
}

void read_file_short_key(QString* line1, QString* line2) // считывание коротного ключа для шифрования или расшифровки
{
    QString file_read = QFileDialog::getOpenFileName();
    QFile file(file_read);
    if(!file.open(QIODevice::ReadOnly))
        return;
    *line1 = file.readLine();
    *line2 = file.readLine();
    //line1.settext
    file.close();
}

void write_file_short_key(QString line1, QString line2, QString file_read){
    QFile file(file_read);
    if (file.open(QIODevice::WriteOnly) && file_read != "")
    {
        if (line1.back() != "\n") line1 += "\n";
        file.write((line1).toUtf8()); // Сохраняет

        file.write(line2.toUtf8()); // Сохраняет
        QMessageBox msgBox;  // диалоговое окно
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Ключ успешно сохранён");  // устанавливаем текст
        msgBox.exec();
    }
    file.close();
}

void MainWindow::on_pushButton_16_clicked() // считывание ключа для шифрования
{
    QString e, n;
    read_file_short_key(&e, &n);
    ui->lineEdit_15->setText(e);
    ui->textEdit_69->setPlainText(n);
}


void MainWindow::on_pushButton_17_clicked() // сохранение ключа для шифрования
{
    QString file_read = QFileDialog::getSaveFileName(this, "Сохранить как");
    QString e = ui->lineEdit_15->text(), n = ui->textEdit_69->toPlainText();
    write_file_short_key(e, n, file_read);
}


void MainWindow::on_pushButton_19_clicked() // чтение ключа для расшифровки
{
    QString d, n;
    read_file_short_key(&d, &n);
    ui->textEdit_14->setPlainText(d);
    ui->textEdit_15->setPlainText(n);
}


void MainWindow::on_pushButton_18_clicked() // сохранение  ключа для расшифровки
{
    QString file_read = QFileDialog::getSaveFileName(this, "Сохранить как");
    QString d = ui->textEdit_14->toPlainText(), n = ui->textEdit_15->toPlainText();
    write_file_short_key(d, n, file_read);
}
