#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTimerEvent>
//#include <thread>
#include <QThread>
#include <QImage>
#include "growcut.h"
#include <QApplication>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //void on_pushButton_clicked();

    //void on_label_destroyed();

    //void on_label_mouseClickedEvent(QMouseEvent *);

    //void on_label_mouseReleaseEvent(QMouseEvent *);

    //void on_pushButton_pressed();

    void on_pushButton_clicked(bool checked);

   // void on_label_mousePressedEvent(QMouseEvent *);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    //void on_label_mousePressEvent(QMouseEvent *);

   // void on_label_linkActivated(const QString &link);

    //void on_label_clicked(QMouseEvent* event);

    //void on_label_release(QMouseEvent* event);

    void onClick(QMouseEvent* event);

    void onRelease(QMouseEvent* event);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
public slots:
    //void onFinished();
    void updateCaption();

public:
   // void paintEvent(QPaintEvent *event);





private:
    Ui::MainWindow *ui;
};

class MyThread : public QThread
{
    Q_OBJECT

public:
    void run();
    MyThread(GrowCut* c ,std::vector<QRect> &object_parts, std::vector<QRect> &background_parts,QImage *i);
   // MyThread();
    GrowCut * c;
    std::vector<QRect> o;
    std::vector<QRect> b;
    QImage *i;
signals:
   // void finished();
};
#endif // MAINWINDOW_H
