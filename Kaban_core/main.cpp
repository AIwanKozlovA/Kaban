#include "mainwindow.h"
#include <QTimer>
#include <QApplication>
#include "ui_mainwindow.h"
void smena_yazyka(MainWindow &w, int yazyk_id){
    QString frasy[5][30] = {
        {"Зашифровать текст", "Расшифровать текст", "Зашифровать файл", "Расшифровать файл", "Считать ключ", "Сохранить ключ", "Сгенерировать ключ", "Считать ключ", "Сохранить ключ", "Зашифровать текст", "Зашифровать файл", "Считать ключ", "Сохранить ключ", "Расшифровать текст", "Расшифровать файл",    "Зашифровать текст", "Расшифровать текст", "Зашифровать файл", "Расшифровать файл", "Сохранить ключ", "Считать ключ", "Сгенерировать ключ", "Ключ шифрования",    "Открытый ключ", "Закрытый ключ", "Открытый ключ", "Закрытый ключ",   "Длина ключа", "доп. Сид", "доп. Сид"},
        {"Encrypt text", "Decrypt text", "Encrypt file", "Decrypt file", "Read key", "Save key", "Generate key", "Read key", "Save key", "Encrypt text", "Encrypt file", "Read key", "Save key", "Decrypt text", "Decrypt file", "Encrypt text", "Decrypt text", "Encrypt file", "Decrypt file", "Save key", "Read key", "Generate key", "Encryption key", "Public key", "Private key", "Public key", "Private key", "Key length", "add. Sid", "add. Sid"},
        {"Text verschlüsseln", "Text entschlüsseln", "Datei verschlüsseln", "Datei entschlüsseln", "Schlüssel lesen", "Schlüssel speichern", "Schlüssel generieren", "Schlüssel lesen", "Schlüssel speichern", "Text verschlüsseln", "Datei verschlüsseln", "Schlüssel lesen", "Schlüssel speichern", "Text entschlüsseln", "Datei entschlüsseln", "Text verschlüsseln", "Text entschlüsseln", "Datei verschlüsseln", "Datei entschlüsseln", "Schlüssel speichern", "Schlüssel lesen", "Schlüssel generieren", "Verschlüsselungsschlüssel", "Öffentlicher Schlüssel", "Privater Schlüssel", "Öffentlicher Schlüssel", "Privater Schlüssel", "Schlüssellänge", "Zusatz-LED", "Zusatz-LED"},
        {"Chiffrer le texte", "Déchiffrer le texte", "Chiffrer le fichier", "Déchiffrer le fichier", "Lire la clé", "Enregistrer la clé", "Générer la clé", "Lire la clé", "Enregistrer la clé", "Chiffrer le texte", "Chiffrer le fichier", "Lire la clé", "Enregistrer la clé", "Déchiffrer le texte", "Déchiffrer le fichier", "Chiffrer le texte", "Déchiffrer le texte", "Chiffrer le fichier", "Déchiffrer le fichier", "Enregistrer la clé", "Lire la clé", "Générer la clé", "Clé de chiffrement", "Clé publique", "clé Privée", "clé Publique", "clé Privée", "Longueur de clé", "LED supplémentaire", "LED supplémentaire"},
        {"Cifrar texto","Descifrar texto", "Cifrar archivo", "Descifrar archivo", "Contar clave", "Guardar clave", "Generar clave", "Contar clave", "Guardar clave", "Cifrar texto","Cifrar archivo", "Leer clave", "Guardar clave", "Descifrar texto", "Descifrar archivo", "Cifrar texto", "Descifrar texto", "Cifrar archivo", "Descifrar archivo", "Guardar clave", "Contar clave", "Generar clave", "Clave de cifrado", "Clave pública", "clave Privada", "clave Pública", "clave Privada", "Longitud de la llave", "DOP. LED","DOP. LED"},

        };
    QWidget*    kn[30] = {
                       w.ui->pushButton,   w.ui->pushButton_2, w.ui->pushButton_3, w.ui->pushButton_4, w.ui->pushButton_13, w.ui->pushButton_15, w.ui->pushButton_14, w.ui->pushButton_16, w.ui->pushButton_17, w.ui->pushButton_69, w.ui->pushButton_70, w.ui->pushButton_19, w.ui->pushButton_18, w.ui->pushButton_49, w.ui->pushButton_50, w.ui->pushButton_55, w.ui->pushButton_48, w.ui->pushButton_5, w.ui->pushButton_8, w.ui->pushButton_6, w.ui->pushButton_7, w.ui->pushButton_56, w.ui->label_12, w.ui->label, w.ui->label_2, w.ui->label_8, w.ui->label_19, w.ui->label_9, w.ui->label_10, w.ui->label_13
    };
    //w.ui->label_12->setText("");
    for(int i = 0; i < 30; i++){
        //for(int j = 0; j< 2;j++){
            if (kn[i]->inherits("QPushButton")) qobject_cast<QPushButton*>(kn[i])->setText(frasy[yazyk_id][i]);
            else if (kn[i]->inherits("QLabel")) qobject_cast<QLabel*>(kn[i])->setText(frasy[yazyk_id][i]);
        //}
    }
    const QString frasy_tab[5][30] = {
        {"Основное", "Меню ключей", "Быстро зашифровать", "Быстро расшифровать"},
        {"Main", "Key menu", "Quickly encrypt", "Quickly decrypt"},
        {"Hauptmenü", "Schlüsselmenü", "Schnell verschlüsseln", "Schnell entschlüsseln"},
        {"Principal", "menu clé", "chiffrer Rapidement", "décrypter Rapidement"},
        {"Principal", "menú de claves", "cifrar Rápidamente", "descifrar Rápidamente"},
    };
    for(int i = 0; i < 4;i++)
    w.ui->tabWidget->setTabText(i, frasy_tab[yazyk_id][i]); ;
    /*for (QObject* obj : kn) {
        if (obj->inherits("QPushButton")) {
            qobject_cast<QPushButton*>(obj)->setText(frasy[yazyk_id][i]);
        }
        /*else if (obj->inherits("QLabel")) {
            qobject_cast<QLabel*>(obj)->setText(frasy[yazyk_id][i]);
        }*/
        /*i++;
    }*/

}
void monitoring_tem(MainWindow &w) {
    QFile file("tema");
    //QByteArray data; // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (!file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
        return; // если это сделать невозможно, то завершаем функцию
    QByteArray p = file.readAll();
    file.close();
    if(p == "🌕"){
        w.toggleTheme();
        w.ui->pushButton_110->setText("🌕");
    }
}


void monitoring_yazyka(MainWindow &w) { // при запуске программы проверяем какой язык выбран
    QString lang[10] = {"ru","en", "ger", "fr", "esp"};
    QFile file("yasuk");
    //QByteArray data; // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (!file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
        return; // если это сделать невозможно, то завершаем функцию
    QByteArray p = file.readAll();
    file.close();
    QString sc = QString(p);
    for(int i = 0; i < 5; i++)
    {
        if (sc == lang[i] && i < 4){
            w.ui->pushButton_109->setText(lang[i]);
            if(i != 0)smena_yazyka(w,i); // запускаем смену
            w.yazyk_id = i;
            break;
        }
    }
}


int main(int argc, char *argv[])
{
    QGuiApplication::setDesktopFileName("Kaban_imresizer.png"); // Имя .desktop-файла
    QApplication a(argc, argv);


    MainWindow w;
    //w.setWindowIcon(QIcon("Kaban.ICO"));
    //w.setWindowIcon(QIcon("icon.ico"));
    QPixmap pix("Kaban_imresizer.png");
    if (!pix.isNull()) {
        w.setWindowIcon(QIcon(pix));
    } else {
        return 0;
        //qDebug() << "Не удалось загрузить иконку!";
    }
   // w.setWindowIcon(QIcon("Kaban_imresizer.svg"));
    QFont font("Segoe UI");
    font.setPixelSize(12);
    qApp->setFont(font);
    w.show();
    // Вызов функций через QTimer::singleShot с задержкой 0
    QTimer::singleShot(0, [&w](){
        //smena_yazyka(w);
        monitoring_yazyka(w);
        monitoring_tem(w);
    });
    return a.exec();
}
