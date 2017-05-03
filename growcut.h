#ifndef GROWCUT_H
#define GROWCUT_H

#include <vector>
#include <QImage>
#include <QColor>
#include <QtCore/qmath.h>
#include <thread>
#include <map>
#include <QPoint>
#include <utility>
#include <mutex>


// Algorithm from http://www.graphicon.ru/oldgr/en/publications/text/gc2005vk.pdf

class GrowCut
{
public:

    class Cell;

    enum label
    {
        NONE = 0,
        OBJECT = 1,
        BACKGROUND = 2
    };

    GrowCut();

    bool init(QImage &image, std::vector<QRect> &Object, std::vector<QRect> &Background,unsigned int &n);

    QImage getObject();

    unsigned int Split();

    unsigned int nextState();

    unsigned int RecomendedChange();

    unsigned int nextStateThread();

    void particialNextState(unsigned int x , unsigned int y, unsigned int xx, unsigned int yy,  unsigned int &counter);

    class Cell getCell(int x, int y);

    class Cell
    {
       public:

        Cell();
        Cell(label l, double Strenght, QRgb Feature_vector);

        label Label;
        double Strenght;
        QRgb Feature_vector;

        bool operator <(const Cell& rhs) const {return true;}
    };

private:


    label label_of_the_Cell(std::vector<QRect> Object, std::vector<QRect> Background, QPoint p);

    double find_MaxPNorm();

    double norm(QRgb pix);

    double normSub(QRgb pix1,QRgb pix2);

    double function_G(double x, double max);

    QImage proccessingImg;

    double max_norm;

    void von_Neumann_Neighborhood(unsigned int x, unsigned int y);

    std::vector<int> width_it;
    std::vector<int> Height_it;
    std::vector< std::thread* > t;
    std::vector< unsigned int > counters;
    unsigned int q_treads;
    std::vector<std::vector<std::vector<double> > > vector_difference;
    class MyPoint;
    std::map < MyPoint, Cell> Change;
    std::vector< std::vector<Cell > > cells;
    std::vector<std::vector<std::vector<class Cell*> > > neighbors;
    std::mutex now_it_gonna_work;

    class MyPoint: public QPoint{
    public:
        MyPoint(int x, int y): QPoint(x,y){}

        bool operator<(const MyPoint &rhs) const{return true;}
    };

};


#endif // GROWCUT_H
