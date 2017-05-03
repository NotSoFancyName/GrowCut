#include "growcut.h"


GrowCut::GrowCut()
{
// ...
}

//initialize initial conditions
bool GrowCut::init(QImage &image, std::vector<QRect> &Object, std::vector<QRect> &Background, unsigned int &n)
{


    cells = std::vector<std::vector<Cell> >((unsigned int)image.width(),std::vector<Cell>( (unsigned int)image.height() ));
    this->proccessingImg = image;
    neighbors = std::vector<std::vector<std::vector<Cell*> > >((unsigned int)image.width(),std::vector<std::vector<Cell*> >( (unsigned int)image.height() ));
    vector_difference = std::vector<std::vector<std::vector<double> > >((unsigned int)image.width(),std::vector<std::vector<double> >( (unsigned int)image.height() ));

    for(int i = 0; i < image.width(); i++)
    {
        for(int j = 0; j < image.height(); j++ )
        {
            cells[i][j].Label = label_of_the_Cell(Object,Background, QPoint(i,j));
            cells[i][j].Feature_vector =  image.pixel(QPoint(i,j));

            if(cells[i][j].Label == GrowCut::BACKGROUND){
                    cells[i][j].Strenght = 1.0;
            }
            else if(cells[i][j].Label == GrowCut::OBJECT){
                    cells[i][j].Strenght = 1.0;
            }
            else{
                    cells[i][j].Strenght = 0.0;
            }
        }
    }

    max_norm = find_MaxPNorm();

    for(int i = 0; i < image.width(); i++)
    {
        for(int j = 0; j < image.height(); j++ )
        {
            von_Neumann_Neighborhood(i,j);
        }
    }

    for(unsigned int i = 0; i < n+1; i++){
        width_it.push_back(ceil(( int)proccessingImg.width() * (i) / n));
        Height_it.push_back(ceil(( int)proccessingImg.height() * (i) / n));
    }

    q_treads = n;
    counters = std::vector<unsigned int>( q_treads * q_treads , 1);

    return true;
}

GrowCut::Cell::Cell(): Label(NONE), Strenght(0),Feature_vector(QRgb()){

    this->Label = NONE;
    this->Feature_vector = QRgb();
    this->Strenght = 0;
}

GrowCut::Cell::Cell(label l, double S, QRgb Fvector):Label(l), Strenght(S),Feature_vector(Fvector)
{
    this->Feature_vector = Fvector;
    this->Label = l;
    this->Strenght = S;
}

// finding neighbohrs of the current cell
void GrowCut::von_Neumann_Neighborhood(unsigned int x, unsigned int y)
{
    //std::vector<class GrowCut::Cell*> res(0);
    if(x+1 < proccessingImg.width()){
        Cell* c = &cells[x+1][y];
        neighbors[x][y].push_back(c);
        vector_difference[x][y].push_back(
                    function_G(normSub(cells[x][y].Feature_vector,cells[x+1][y].Feature_vector)
                    ,max_norm)
                    );
    }
    if( (y-1) != -1){
         Cell* c = &cells[x][y-1];
         neighbors[x][y].push_back(c);
         vector_difference[x][y].push_back(
                     function_G(normSub(cells[x][y].Feature_vector,cells[x][y-1].Feature_vector)
                     ,max_norm)
                     );
    }
    if( (x-1) != -1){
       Cell* c = &cells[x-1][y];
         neighbors[x][y].push_back(c);
         vector_difference[x][y].push_back(
                     function_G(normSub(cells[x][y].Feature_vector,cells[x-1][y].Feature_vector)
                     ,max_norm)
                     );
    }
    if(x+1 < proccessingImg.height()){
        Cell* c = &cells[x][y+1];
        neighbors[x][y].push_back(c);
        vector_difference[x][y].push_back(
                    function_G(normSub(cells[x][y].Feature_vector,cells[x][y+1].Feature_vector)
                    ,max_norm)
                    );
    }

   // return res;
}

// next state of cellular automaton
unsigned int GrowCut::nextState()
{
    bool counter = 0;
    for(unsigned int i = 0; i < cells.size(); i++)
    {
        for(unsigned int j = 0; j < cells[i].size(); j++)
        {
           // std::vector<Cell> neighborhood = von_Neumann_Neighborhood(i,j);
            if(cells[i][j].Strenght != 1.0){
            for(unsigned int k = 0; k < neighbors[i][j].size(); k++)
            {
               if(neighbors[i][j][k]->Label != cells[i][j].Label) {

                   double cell_assault_power =
                        vector_difference[i][j][k] *
                            neighbors[i][j][k]->Strenght;

                   if( cell_assault_power > cells[i][j].Strenght )
                   {
                        counter++;
                        cells[i][j].Label = neighbors[i][j][k]->Label;
                        cells[i][j].Strenght = cell_assault_power;
                   }
               }
            }
            }
        }
    }

    return counter;

}

// defines the "difference" between two cells
double GrowCut::function_G(double x, double max)
{

    double res = 1.0 - ( x / max);

    return res;
}

// get norm of feature vector
double GrowCut::norm(QRgb pix)
{
    QColor c(pix);
    return qSqrt(c.red()*c.red() + c.blue()*c.blue() + c.green()*c.green());
}

// get norm of substitution of two feature vectors
double GrowCut::normSub(QRgb pix1,QRgb pix2)
{
    QColor c(pix1);
    QColor t(pix2);

    return qSqrt( (c.red()-t.red())*(c.red()-t.red()) +
              (c.blue()-t.blue())*(c.blue()-t.blue())+
            (c.green()-t.green())*(c.green()-t.green()));
}

// find feature vector with maximum norm
double GrowCut::find_MaxPNorm()
{
    double max = 0.0;
    for(unsigned int i = 0; i < cells.size(); i++)
    {
        for(unsigned int j = 0; j < cells[i].size(); j++)
        {
            if(max < norm(cells[i][j].Feature_vector))
                max = norm(cells[i][j].Feature_vector);
        }
    }

    return max;
}

//check if the cell in the rectangular and if so get its label
GrowCut::label GrowCut::label_of_the_Cell(std::vector<QRect> Object, std::vector<QRect> Background, QPoint p)
{
    for(unsigned int i = 0; i < Object.size(); i++)
    {
        if(Object[i].contains(p))
            return GrowCut::OBJECT;
    }

    for(unsigned int i = 0; i < Background.size(); i++)
    {
        if(Background[i].contains(p))
            return GrowCut::BACKGROUND;

    }

    return GrowCut::NONE;
}

// split object and background,was used for total split, now deprecated
unsigned int GrowCut::Split(){

    unsigned int counter = 0;
    //double max_fv = find_MaxPNorm();

    while(nextState()){
        counter++;

        if (counter > 1000)
            break;
    }

    return counter;
}

//get image without background (set black color to background pixels)
QImage GrowCut::getObject()
{
    for(int i = 0; i < proccessingImg.width(); i++)
    {
        for(int j = 0; j < proccessingImg.height(); j++)
        {
            if(cells[i][j].Label == BACKGROUND)
            {
                proccessingImg.setPixel(QPoint(i,j),QRgb() );
            }
        }
    }

    return proccessingImg;
}

//get cellular automata state using threads (here I am using 4 thread becouse i have 4core proc)
unsigned int GrowCut::nextStateThread(){

    unsigned int res = 0;
    for(unsigned int i = 0; i < q_treads; i++){
        for(unsigned int j = 0; j < q_treads; j++){
                 threads.push_back(new std::thread( &GrowCut::particialNextState ,this, width_it[i], width_it[i+1],
                             Height_it[j], Height_it[j+1], std::ref( counters[i*q_treads + j] ) ));
        }
    }

    for(unsigned int i = 0; i < threads.size(); i++){
        threads[i]->join();
    }

    for(unsigned int i = 0; i < threads.size(); i++){
         delete threads[i];
    }

    threads.erase(threads.begin(),threads.end());

    for(unsigned int i = 0; i < counters.size(); i++){
       res += counters[i];
    }

    return res;
}

//next state of the part of the area and counter for quantity of automata changes
void GrowCut::particialNextState(unsigned int x , unsigned int xx, unsigned int y, unsigned int yy, unsigned int &counter){

    if(!counter){
        return;
    }

    counter = 0;

    for( int i = x; i < xx; i++)
    {
        for( int j = y; j < yy; j++)
        {
           // std::vector<Cell> neighborhood = von_Neumann_Neighborhood(i,j);
            if(cells[i][j].Strenght != 1.0){
            for( int k = 0; k < neighbors[i][j].size(); k++)
            {
               if(neighbors[i][j][k]->Label != cells[i][j].Label) {

                   double cell_assault_power =
                        vector_difference[i][j][k] *
                            neighbors[i][j][k]->Strenght;

                   if( cell_assault_power > cells[i][j].Strenght )
                   {
                        //Cell change_cell(neighbors[i][j][k]->Label,cell_assault_power,cells[i][j].Feature_vector);

                        counter++;

                        now_it_gonna_work.lock();
                        cells[i][j].Label = neighbors[i][j][k]->Label;
                        cells[i][j].Strenght = cell_assault_power;
                        now_it_gonna_work.unlock();
                   }
               }
            }
            }
        }
    }
}
