#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
