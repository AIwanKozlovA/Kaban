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

private:
    Ui::MainWindow *ui;
};

void error_file();
void writeFileFromData(const QString &data, const QString &newFilePath);
#endif // MAINWINDOW_H
