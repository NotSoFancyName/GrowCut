#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QException>
#include <QFileDialog>


namespace Ui {
class myQlabel;
}

class myQlabel : public QLabel
{
    Q_OBJECT

public:
      explicit myQlabel(QWidget* parent=0 );
    ~myQlabel();

private:
    Ui::myQlabel *ui;

signals:
   void clicked(QMouseEvent* event);
   void release(QMouseEvent* event);
public:
   void mousePressEvent(QMouseEvent* event) ;
   void mouseReleaseEvent(QMouseEvent* event);
  // void paintEvent(QPaintEvent* e);
};

#endif // MYQLABEL_H
