#ifndef __KLOTSKISOLVER_H__
#define __KLOTSKISOLVER_H__

class KlotskiSolver
{
public:
    KlotskiSolver();
    ~KlotskiSolver();

    void setMask(int value, int x, int y);
    bool isEmpty(int x, int y);

    void print();
private:
    int m_mask[5][4];
};

#endif // __KLOTSKISOLVER_H__