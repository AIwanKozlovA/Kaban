#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // Указываем тип для указателя на функцию
    using ReadKeyFunction = void (*)(const QString&, Ui::MainWindow*);

    int yazyk_id = 0;
private slots:
    void on_pushButton_70_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_50_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_69_clicked();

    void on_pushButton_49_clicked();

    void shif_text(QString e, QString n,QString isx_text, QTextEdit* text_field);

    void ra_shif_text(QString d, QString n,QString text_for_rash, QTextEdit* text_field);

    void Shif_file(QString e, QString n, QString myFile,QString Shif_file);

    void raShif_file(QString d, QString n, QString name_shif_file,QString name_rashif_file);

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_55_clicked();

    void on_pushButton_48_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_56_clicked();

    int gen_key(int argc, QString argv[], const QString& file_genkey, ReadKeyFunction readkey);

    void on_pushButton_109_clicked();

    void on_pushButton_11_clicked();


    void on_pushButton_95_clicked();

    void on_pushButton_97_clicked();

    void on_pushButton_103_clicked();

    void on_pushButton_105_clicked();

    void on_pushButton_107_clicked();

    void on_pushButton_99_clicked();

    void on_pushButton_101_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_96_clicked();

    void on_pushButton_98_clicked();

    void on_pushButton_104_clicked();

    void on_pushButton_106_clicked();

    void on_pushButton_108_clicked();

    void on_pushButton_100_clicked();

    void on_pushButton_102_clicked();

    void cop_in_byfer(QString text);

    void on_pushButton_110_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void write_file_short_key(QString line1, QString line2, QString file_read);

public: //private:
    void toggleTheme() {
        if (m_darkMode) {
            setLightTheme();
        } else {
            setDarkTheme();
        }
        m_darkMode = !m_darkMode;
    }
    bool m_darkMode = false;

    void setDarkTheme() {
        this->setStyleSheet(
            "QMainWindow, QWidget {"
            "   background-color: ##f5f7fa;"
            "   color: #ffffff;"
            "}"
            "QPushButton {"
            //"   background-color: #3a3a3a;"
            "   background-color: #e1e6eb;"  // Светло-серый фон кнопок
            "   color: white;"
            "border-radius: 6px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #4a4a4a;"
            "}"
            "QTextEdit {"
            "   background-color: white;"
            "   color: #000000;"
            "}"
            "QLineEdit {"
            "   background-color: white;"
            "   color: #000000;"
            "}"
            );
    }

    void setLightTheme() {
        this->setStyleSheet(
            "QMainWindow, QWidget {"
            "   background-color: #f5f5f5;"
            "   color: #000000;"
            "}"
            "QPushButton {"
            "   background-color: #e0e0e0;"
            "   color: black;"
            "border-radius: 6px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #d0d0d0;"
            "}"
            "QTextEdit {"
            "   background-color: white;"
            "   color: #000000;"
            "}"
            "QLineEdit {"
            "   background-color: white;"
            "   color: #000000;"
            "}"
            );
    }


public: //private:
    Ui::MainWindow *ui;
};
void smena_yazyka(MainWindow &w, int yazyk_id);
void error_file();
void writeFileFromData(const QString &data, const QString &newFilePath);
#endif // MAINWINDOW_H
