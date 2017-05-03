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

    unsigned int nextStateThread();

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

    double find_MaxPNorm();// maximum norm of whole image

    double norm(QRgb pix);// norm of the pixel

    double normSub(QRgb pix1,QRgb pix2); // norm of the difference

    double function_G(double x, double max);// regressive func to estimate color 'difference' of to pixels

    void von_Neumann_Neighborhood(unsigned int x, unsigned int y);

    void particialNextState(unsigned int x , unsigned int y, unsigned int xx, unsigned int yy,  unsigned int &counter);

    std::vector<int> width_it;                                         // division of the image
    std::vector<int> Height_it;                                        // ---
    std::vector< std::thread* > threads;                               // ...
    std::vector< unsigned int > counters;                              // counters for each thread
    unsigned int q_treads;                                             // q_treads^2 is a quantity of threads
    std::vector<std::vector<std::vector<double> > > vector_difference; // norm difference between cell and its neighbors
    std::vector< std::vector<Cell > > cells;                           // cells of the image
    std::vector<std::vector<std::vector<class Cell*> > > neighbors;    // pointers to the neighbors
    std::mutex now_it_gonna_work;                                      // mutex for blocking access to changing state of 'cells'
    double max_norm;
    QImage proccessingImg;

};


#endif // GROWCUT_H
