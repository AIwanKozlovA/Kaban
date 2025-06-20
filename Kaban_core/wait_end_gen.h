#ifndef WAIT_END_GEN_H
#define WAIT_END_GEN_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
class Wait_End_gen : public QObject
{
    Q_OBJECT
public:
    Wait_End_gen();
    explicit Wait_End_gen(QString threadName);

    // Переопределяем метод run(), в котором будет
    // располагаться выполняемый код
    void run();
private:
    QString file_genkey;   // Имя потока
signals:
    void updateTextEdit(const QString &text);
};

#endif // WAIT_END_GEN_H
