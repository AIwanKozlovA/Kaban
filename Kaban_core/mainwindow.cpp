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
#include <QtConcurrent/QtConcurrentRun>
#ifdef Q_OS_WIN
#include <QStringDecoder>
#endif

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
void MainWindow::Shif_file(QString e, QString n, QString myFile,QString Shif_file){
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




void MainWindow::raShif_file(QString d, QString n, QString name_shif_file,QString name_rashif_file){
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
    if( e != "" && n != ""){
        QString myFile = QFileDialog::getOpenFileName();
        QString name_shif_file = QFileDialog::getSaveFileName(this, "Сохранить как");
        if(myFile != "") Shif_file(e,n, myFile, name_shif_file);
    } else{
        QMessageBox msgBox;  // диалоговое окно
        //msgBox.setText("Чтобы расшифровать текст введите его в верхнее поле ввода и считайте ключ шифрования или скопируйте его самостоятельно в меню ключей");  // устанавливаем текст
        QString yved[10] = {"Чтобы быстро зашифровать текст или файл, скопируйте или считайте открытый ключ шифрования. Если вы хотите зашифровать текст верхнее поле ввода не должно быть пусто!!!", "To quickly encrypt a text or file, copy or read the public encryption key. If you want to encrypt the text, the upper input field should not be empty!!!", "Um Text oder eine Datei schnell zu verschlüsseln, kopieren oder lesen Sie den öffentlichen Verschlüsselungsschlüssel. Wenn Sie den Text verschlüsseln möchten, sollte das obere Eingabefeld nicht leer sein!!!", "Pour chiffrer rapidement un texte ou un fichier, copiez ou lisez la clé de chiffrement publique. Si vous voulez crypter le texte le champ de saisie supérieur ne doit pas être vide!!!", "Para cifrar rápidamente un texto o un archivo, copie o lea la clave de cifrado pública. Si desea cifrar el texto, el campo de entrada superior no debe estar vacío.!!"};
        msgBox.setText(yved[yazyk_id]);
        msgBox.exec();
    }
}


void MainWindow::on_pushButton_3_clicked() // шифруем файл
{
    QString e=ui->lineEdit->text();
    QString n=ui->textEdit_3->toPlainText();
    if(e != "" && n != ""){
        QString myFile= QFileDialog::getOpenFileName();
        if(myFile != ""){
            QString name_shif_file = QFileDialog::getSaveFileName(this, "Сохранить как");
            if(name_shif_file != "") Shif_file(e,n, myFile, name_shif_file);
        }
    }else{
        QMessageBox msgBox;  // диалоговое окно
        QString yved[10] = {"Чтобы зашифровать файл сгенерируйте или считайте ключ шифрования в меню ключей", "To encrypt a file, generate or read the encryption key in the keys menu", "Um eine Datei zu verschlüsseln, erzeugen Sie den Verschlüsselungsschlüssel im Schlüsselmenü oder lesen Sie ihn aus", "Pour chiffrer un fichier, générez ou lisez la clé de chiffrement dans le menu des clés", "Para cifrar un archivo, genere o lea la clave de cifrado en el menú de claves"};
        msgBox.setText(yved[yazyk_id]);  // устанавливаем текст
        msgBox.exec();
    }
}


void MainWindow::on_pushButton_50_clicked() // расшифруем файл
{
    QString d=ui->textEdit_14->toPlainText();
    QString n=ui->textEdit_15->toPlainText();
    if(d != "" && n != ""){
        QString name_shif_file= QFileDialog::getOpenFileName();
        if(name_shif_file != ""){
            QString name_rashif_file = QFileDialog::getSaveFileName(this, "Сохранить как");
            //Shif_file(e,n, myFile, name_shif_file);
            if(name_rashif_file != "") raShif_file(d, n, name_shif_file, name_rashif_file);
        }
    }else{
        QMessageBox msgBox;  // диалоговое окно
        QString yved[10] = {"Чтобы расшифровать файл скопируйте или считайте ключ шифрования", "To decrypt a file, copy or read the encryption key", "Um eine Datei zu entschlüsseln, kopieren oder lesen Sie den Verschlüsselungsschlüssel", "Pour déchiffrer un fichier, copiez ou lisez la clé de chiffrement dans", "Para descifrar el archivo, copie o lea la clave de cifrado"};
        msgBox.setText(yved[yazyk_id]);  // устанавливаем текст
        msgBox.exec();
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    QString d=ui->textEdit->toPlainText();
    QString n=ui->textEdit_3->toPlainText();
    if(d != "" && n != ""){
        QString name_shif_file= QFileDialog::getOpenFileName();
        if(name_shif_file != ""){
            QString name_rashif_file = QFileDialog::getSaveFileName(this, "Сохранить как");
            //Shif_file(e,n, myFile, name_shif_file);
            if(name_rashif_file != "") raShif_file(d, n, name_shif_file, name_rashif_file);
        }
    }else{
        QMessageBox msgBox;  // диалоговое окно
        QString yved[10] = {"Чтобы расшифровать файл скопируйте или считайте ключ шифрования в меню ключей", "To decrypt a file, copy or read the encryption key in the keys menu", "Um eine Datei zu entschlüsseln, kopieren oder lesen Sie den Verschlüsselungsschlüssel im Schlüsselmenü", "Pour déchiffrer un fichier, copiez ou lisez la clé de chiffrement dans le menu des clés", "Para descifrar el archivo, copie o lea la clave de cifrado en el menú de claves"};
        msgBox.setText(yved[yazyk_id]);  // устанавливаем текст
        msgBox.exec();
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
        QString yved[10] = {"Ключ успешно сохранён", "The key has been saved successfully", "Der Schlüssel wurde erfolgreich gespeichert", "Clé enregistrée avec succès", "Clave guardada correctamente"};
        msgBox.setText(yved[yazyk_id]);  // устанавливаем текст
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
    if(isx_text != ""){
        /*std::string shif = Shif_RSA_text(isx_text.toStdString(), e.toStdString(), n.toStdString()); // шифрование текста RSA
        text_field->setPlainText(QString::fromStdString(shif));*/

        QProcess *process = new QProcess(this);  // Важно: создаём в куче и указываем parent (this)
        QString workingDir = QDir::currentPath();
        // Указываем путь к программе, которую хотим запустить
        #ifdef Q_OS_WIN
                QString programPath = workingDir + "/RSA_C.exe";
        #elif defined(Q_OS_LINUX)
                QString programPath = workingDir + "/RSA_C";
        #elif defined(Q_OS_MAC)
                QString programPath = workingDir + "/RSA_C";
        #endif



        // Соединяем сигналы процесса
        connect(process, &QProcess::readyReadStandardOutput, [process, this, text_field]() {
            QString output = process->readAllStandardOutput().trimmed();
            qDebug() << "Получены данные:" << output;
            text_field->setPlainText(output);
            // Можно обновить GUI (например, QLabel->setText(output))
        });

        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                [process, this](int exitCode, QProcess::ExitStatus status) {
                    if (status == QProcess::CrashExit) {
                        qDebug() << "Процесс упал!";
                    } else {
                        qDebug() << "Процесс завершён с кодом:" << exitCode;
                    }
                    process->deleteLater();  // Удаляем процесс после завершения
                });
        QStringList arguments;
        arguments << isx_text << e << n << "-t0"; // Здесь добавьте ваши аргументы
        // Запускаем процесс (путь может быть абсолютным или относительным)
        process->start(programPath, arguments);

        // Передаём данные (если нужно)
       // process->write("Данные для обработки\n");
        process->closeWriteChannel();
    }
}


void MainWindow::ra_shif_text(QString d, QString n,QString text_for_rash, QTextEdit* text_field){ // расшифровка текста
    if(text_for_rash != ""){
        /*std::string rashif = RaShif_RSA_text(text_for_rash.toStdString(), d.toStdString(), n.toStdString()); // шифрование текста RSA
        text_field->setPlainText(QString::fromStdString(rashif));*/

        QProcess *process = new QProcess(this);  // Важно: создаём в куче и указываем parent (this)
        QString workingDir = QDir::currentPath();
// Указываем путь к программе, которую хотим запустить
#ifdef Q_OS_WIN
        QString programPath = workingDir + "/RSA_C.exe";
#elif defined(Q_OS_LINUX)
        QString programPath = workingDir + "/RSA_C";
#elif defined(Q_OS_MAC)
        QString programPath = workingDir + "/RSA_C";
#endif
        #ifdef Q_OS_WIN
        connect(process, &QProcess::readyReadStandardOutput, [process, this, text_field]() {
            //QString output = process->readAllStandardOutput().trimmed();

            QByteArray hexData = process->readAllStandardOutput().trimmed();

            // Конвертируем hex в байты
            QByteArray bytes = QByteArray::fromHex(hexData);

            // Создаём декодер (автоматически определяет системную кодировку)
            QStringDecoder decoder(QStringDecoder::System);

            // Декодируем в QString
            QString result = decoder.decode(bytes);

            // Если известно, что данные в UTF-8:
            // QStringDecoder utf8Decoder(QStringDecoder::Utf8);
            // QString result = utf8Decoder.decode(bytes);

            if (decoder.hasError()) {
                qWarning() << "Ошибка декодирования!";
                result = "Неверная кодировка данных";
            }


            qDebug() << "Получены данные:" << result;
            text_field->setPlainText(result);
            // Можно обновить GUI (например, QLabel->setText(output))
        });

        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                [process, this](int exitCode, QProcess::ExitStatus status) {
                    if (status == QProcess::CrashExit) {
                        qDebug() << "Процесс упал!";
                    } else {
                        qDebug() << "Процесс завершён с кодом:" << exitCode;
                    }
                    process->deleteLater();  // Удаляем процесс после завершения
                });
        #else
        // Соединяем сигналы процесса
        connect(process, &QProcess::readyReadStandardOutput, [process, this, text_field]() {
            QString output = process->readAllStandardOutput().trimmed();
            qDebug() << "Получены данные:" << output;
            text_field->setPlainText(output);
            // Можно обновить GUI (например, QLabel->setText(output))
        });

        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                [process, this](int exitCode, QProcess::ExitStatus status) {
                    if (status == QProcess::CrashExit) {
                        qDebug() << "Процесс упал!";
                    } else {
                        qDebug() << "Процесс завершён с кодом:" << exitCode;
                    }
                    process->deleteLater();  // Удаляем процесс после завершения
                });
        #endif
        QStringList arguments;
        arguments << text_for_rash << d << n << "-t1"; // Здесь добавьте ваши аргументы
        // Запускаем процесс (путь может быть абсолютным или относительным)
        process->start(programPath, arguments);

        // Передаём данные (если нужно)
        // process->write("Данные для обработки\n");
        process->closeWriteChannel();
    } else{
        QMessageBox msgBox;  // диалоговое окно
        //msgBox.setText("Чтобы расшифровать текст введите его в верхнее поле ввода и считайте ключ шифрования или скопируйте его самостоятельно в меню ключей");  // устанавливаем текст
        QString yved[10] = {"Чтобы расшифровать текст введите его в верхнее поле ввода и считайте ключ шифрования или скопируйте его самостоятельно в меню ключей", "To decrypt a text, enter it in the upper input field and read the encryption key or copy it yourself in the keys menu", "Um den Text zu entschlüsseln, geben Sie ihn in das obere Eingabefeld ein und lesen Sie den Verschlüsselungsschlüssel aus oder kopieren Sie ihn selbst in das Schlüsselmenü", "Pour déchiffrer le texte, entrez-le dans le champ de saisie supérieur et lisez la clé de chiffrement ou copiez-le vous-même dans le menu des clés", "Para descifrar el texto, escríbalo en el campo de entrada superior y lea la clave de cifrado o cópielo usted mismo en el menú de claves" };
        msgBox.setText(yved[yazyk_id]);
        msgBox.exec();
    }
}
void MainWindow::on_pushButton_clicked() // шифрование текста
{
    QString isx_text = ui->textEdit_5->toPlainText();
    QString e=ui->lineEdit->text();
    QString n=ui->textEdit_3->toPlainText();
    if(isx_text != "" && e != "" && n != "") shif_text(e, n, isx_text, ui->textEdit_6);
    else{
        QMessageBox msgBox;  // диалоговое окно
        QString yved[10] = {"Чтобы зашифровать текст введите его в верхнее поле ввода и сгенерируйте или считайте ключ шифрования в меню ключей", "To encrypt a text, enter it in the upper input field and generate or read the encryption key in the keys menu", "Um den Text zu verschlüsseln, geben Sie ihn in das obere Eingabefeld ein und generieren oder lesen Sie den Verschlüsselungsschlüssel im Schlüsselmenü", "Pour chiffrer le texte, entrez-le dans le champ de saisie supérieur et générez ou lisez la clé de chiffrement dans le menu des clés", "Para cifrar texto, escríbalo en el campo de entrada superior y genere o lea la clave de cifrado en el menú de claves"};
        msgBox.setText(yved[yazyk_id]);  // устанавливаем текст
        msgBox.exec();
    }
}


void MainWindow::on_pushButton_2_clicked() // расшифровка текста
{
    QString d = ui->textEdit->toPlainText();
    QString n=ui->textEdit_3->toPlainText();
    QString text_for_rash = ui->textEdit_5->toPlainText();
    if(text_for_rash != "" && d != "" && n != "") ra_shif_text(d, n, text_for_rash, ui->textEdit_6);
    else{
        QMessageBox msgBox;  // диалоговое окно
        //msgBox.setText("Чтобы расшифровать текст введите его в верхнее поле ввода и считайте ключ шифрования или скопируйте его самостоятельно в меню ключей");  // устанавливаем текст
        QString yved[10] = {"Чтобы расшифровать текст введите его в верхнее поле ввода и считайте ключ шифрования или скопируйте его самостоятельно в меню ключей", "To decrypt a text, enter it in the upper input field and read the encryption key or copy it yourself in the keys menu", "Um den Text zu entschlüsseln, geben Sie ihn in das obere Eingabefeld ein und lesen Sie den Verschlüsselungsschlüssel aus oder kopieren Sie ihn selbst in das Schlüsselmenü", "Pour déchiffrer le texte, entrez-le dans le champ de saisie supérieur et lisez la clé de chiffrement ou copiez-le vous-même dans le menu des clés", "Para descifrar el texto, escríbalo en el campo de entrada superior y lea la clave de cifrado o cópielo usted mismo en el menú de claves" };
        msgBox.setText(yved[yazyk_id]);
        msgBox.exec();
    }
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
    if(isx_text != "" && e != "" && n != "")shif_text(e, n, isx_text, ui->textEdit_71);
    else{
        QMessageBox msgBox;  // диалоговое окно
        //msgBox.setText("Чтобы расшифровать текст введите его в верхнее поле ввода и считайте ключ шифрования или скопируйте его самостоятельно в меню ключей");  // устанавливаем текст
        QString yved[10] = {"Чтобы быстро зашифровать текст или файл, скопируйте или считайте открытый ключ шифрования. Если вы хотите зашифровать текст верхнее поле ввода не должно быть пусто!!!", "To quickly encrypt a text or file, copy or read the public encryption key. If you want to encrypt the text, the upper input field should not be empty!!!", "Um Text oder eine Datei schnell zu verschlüsseln, kopieren oder lesen Sie den öffentlichen Verschlüsselungsschlüssel. Wenn Sie den Text verschlüsseln möchten, sollte das obere Eingabefeld nicht leer sein!!!", "Pour chiffrer rapidement un texte ou un fichier, copiez ou lisez la clé de chiffrement publique. Si vous voulez crypter le texte le champ de saisie supérieur ne doit pas être vide!!!", "Para cifrar rápidamente un texto o un archivo, copie o lea la clave de cifrado pública. Si desea cifrar el texto, el campo de entrada superior no debe estar vacío.!!"};
        msgBox.setText(yved[yazyk_id]);
        msgBox.exec();
    }
}


void MainWindow::on_pushButton_49_clicked() // быстрая расшифровка текста
{
    QString text_for_rash = ui->textEdit_119->toPlainText();
    QString d=ui->textEdit_14->toPlainText();
    QString n=ui->textEdit_15->toPlainText();
    if(text_for_rash != "" && d != "" && n != "")ra_shif_text(d, n, text_for_rash, ui->textEdit_118);
    else{
        QMessageBox msgBox;  // диалоговое окно
        QString yved[10] = {"Для расшифровки текста введите или считайте закрытый ключ. И верхнее поле ввода не должно быть пусто!!!", "To decrypt the text, enter or read the private key. And the top input field should not be empty!!!", "Um den Text zu entschlüsseln, geben Sie den privaten Schlüssel ein oder lesen Sie ihn ein. Und das obere Eingabefeld sollte nicht leer sein!!!", "Pour déchiffrer le texte, entrez ou lisez la clé privée. Et le champ de saisie supérieur ne doit pas être vide!!!", "Para descifrar el texto, escriba o lea la clave privada. ¡Y el campo de entrada superior no debe estar vacío!!!"};
        msgBox.setText(yved[yazyk_id]);
        msgBox.exec();
    }
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

void MainWindow::write_file_short_key(QString line1, QString line2, QString file_read){
    QFile file(file_read);
    if (file.open(QIODevice::WriteOnly) && file_read != "")
    {
        if (line1.back() != "\n") line1 += "\n";
        file.write((line1).toUtf8()); // Сохраняет

        file.write(line2.toUtf8()); // Сохраняет
        QMessageBox msgBox;  // диалоговое окно
        msgBox.setIcon(QMessageBox::Information);
        QString yved[10] = {"Ключ успешно сохранён", "The key has been saved successfully", "Der Schlüssel wurde erfolgreich gespeichert", "Clé enregistrée avec succès", "Clave guardada correctamente"};
        msgBox.setText(yved[yazyk_id]);  // устанавливаем текст
        msgBox.exec();
    }
    file.close();
}

void MainWindow::on_pushButton_16_clicked() // считывание ключа для шифрования
{

    QString file_read = QFileDialog::getOpenFileName();
    QFile file(file_read);
    if(!file.open(QIODevice::ReadOnly))
        return;
    int zz = file.readAll().count("\n");
    file.close();
    if(zz < 3){
        QFile file0(file_read);
        if(!file0.open(QIODevice::ReadOnly))
            return;
        ui->lineEdit_15->setText(file0.readLine());
        ui->textEdit_69->setPlainText(file0.readLine());
        file0.close();
    }else{ // если считывается ключ из файла в котором он сгенерирован. Где есть закрытый и открытый ключ
        QFile file1(file_read);
        if(!file1.open(QIODevice::ReadOnly))
            return;
        file1.readLine();
        file1.readLine();
        ui->lineEdit_15->setText(file1.readLine());
        ui->textEdit_69->setPlainText(file1.readLine());
        file1.close();
    }
}


void MainWindow::on_pushButton_17_clicked() // сохранение ключа для шифрования
{
    QString file_read = QFileDialog::getSaveFileName(this, "Сохранить как");
    QString e = ui->lineEdit_15->text(), n = ui->textEdit_69->toPlainText();
    write_file_short_key(e, n, file_read);
}


void MainWindow::on_pushButton_19_clicked() // чтение ключа для расшифровки
{
    QString file_read = QFileDialog::getOpenFileName();
    QFile file(file_read);
    if(!file.open(QIODevice::ReadOnly))
        return;
    int zz = file.readAll().count("\n");
    file.close();
    if(zz < 3){
        QFile file0(file_read);
        if(!file0.open(QIODevice::ReadOnly))
            return;
        ui->textEdit_14->setPlainText(file0.readLine());
        ui->textEdit_15->setPlainText(file0.readLine());
        file0.close();
    }else{ // если считывается ключ из файла в котором он сгенерирован. Где есть закрытый и открытый ключ
        QFile file1(file_read);
        if(!file1.open(QIODevice::ReadOnly))
            return;
        file1.readLine();
        file1.readLine();
        file1.readLine();
        ui->textEdit_15->setPlainText(file1.readLine());
        ui->textEdit_14->setPlainText(file1.readLine());
        file1.close();
    }
}


void MainWindow::on_pushButton_18_clicked() // сохранение  ключа для расшифровки
{
    QString file_read = QFileDialog::getSaveFileName(this, "Сохранить как");
    QString d = ui->textEdit_14->toPlainText(), n = ui->textEdit_15->toPlainText();
    write_file_short_key(d, n, file_read);
}


void MainWindow::on_pushButton_20_clicked() // сохранение открытого ключа
{
    QString file_read = QFileDialog::getSaveFileName(this, "Сохранить как");
    QString e = ui->lineEdit->text(), n = ui->textEdit_3->toPlainText();
    write_file_short_key(e, n, file_read);
}


void MainWindow::on_pushButton_21_clicked() // сохранение закрытого ключа
{
    QString file_read = QFileDialog::getSaveFileName(this, "Сохранить как");
    QString d = ui->textEdit->toPlainText(), n = ui->textEdit_3->toPlainText();
    write_file_short_key(d, n, file_read);
}

