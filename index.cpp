#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <cstdlib>

using namespace std;

#define counter 30

double mx = 0;

void Swap(double &a, double &b);
void sort(int n, int m, double **Xval, double **Yval);
void chuyenVi(int n, int m, double **A, double **B);
void nhanMaTran(double **A, int n, int m, double **B, int n2, int m2, double **C);
void daoHam(int m, double **nhanXX, double **nhanXY, double **weight, double **g);
int findMax(int m, double **vio);

int main()
{
    ifstream mtx("x.txt");
    ifstream mty("y.txt");
    ofstream mtyo("yo.txt");
    int n, m, i, j;
    mtx >> n;
    mtx >> m;

    // cap phat bo nho va doc du lieu
    // tran tran n hang
    double **Xval = new double *[n];
    double **Yval = new double *[n];
    for (i = 0; i < n; i++)
    {
        Xval[i] = new double[m];
        Yval[i] = new double[1];
    }

    // doc file
    for (i = 0; i < n; ++i)
    {
        mty >> Yval[i][0];
        for (j = 0; j < m; j++)
        {
            mtx >> Xval[i][j];
        }
    }

    sort(n, m, Xval, Yval);

    // ma tran m hang
    double **nhanXX = new double *[m];    // mang ma tran XX
    double **nhanXY = new double *[m];    // mang ma tran XY
    double **weight = new double *[m];    // mang weight
    double **chuyenViX = new double *[m]; // mang chuyen vi
    double **g = new double *[m];         // mang ma tran dao ham
    double **vio = new double *[m];       // mang ma tran vio
    for (i = 0; i < m; i++)
    {
        nhanXX[i] = new double[m];
        nhanXY[i] = new double[1];
        weight[i] = new double[1];
        chuyenViX[i] = new double[n];
        g[i] = new double[1];
        vio[i] = new double[1];
    }

    // tao init
    chuyenVi(n, m, Xval, chuyenViX);                 // tao ma tran chuyen vi
    nhanMaTran(chuyenViX, m, n, Xval, n, m, nhanXX); // nhan ma tran X voi ma tran chuyen vi
    nhanMaTran(chuyenViX, m, n, Yval, n, 1, nhanXY); // nhan ma tran X voi ma tran Y
    daoHam(m, nhanXX, nhanXY, weight, g);            // dao ham ma tran

    int index = findMax(m, g);
    int q = 0;
    int count = 0;
    while (mx > 1e-5 || count < counter)
    {
        weight[index][0] = weight[index][0] - (g[index][0] / nhanXX[index][index]); // xu dung new ton toi uu trong so
        daoHam(m, nhanXX, nhanXY, weight, g);                                       //  tinh lai dao ham
        index = findMax(m, g);                                                      // tim lon nhat
        count++;
    }

    // in ra man hinh phuong trinh tong quat
    double y;
    double ss = 0;
    // weight[0][0] = 100;
    // weight[1][0] = 10;
    // weight[2][0] = 10;
    // weight[3][0] = -20;

    for (int i = 0; i < n; i++)
    {
        y = 0;
        for (int j = 0; j < m; j++)
        {
            y += weight[j][0]*Xval[i][j];
        }
        // cout << Yval[i][0] << " ";
        cout << y << "\n";
    }
    // cout << ss / n;

    // thu nghiem
    cout << "\n\nPhep thu:\n";
    double end = 1;
    while (1)
    {
        double *x = new double[m];
        y = 0;
        cout << "\n";
        for (i = 0; i < m; i++)
        {
            fflush(stdin);
            cout << "Nhap x" << i + 1 << ": ";
            cin >> x[i];
            y += x[i] * weight[i][0];
        }
        fflush(stdin);
        delete[] x;
        cout << "\nGia tri cua y du doan duoc: " << y << endl
             << "\nket thuc nhap 0: ";
        cin >> end;
        if (end == 0)
            break;
    }

    return 0;
}

void chuyenVi(int n, int m, double **A, double **B)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            B[i][j] = A[j][i];
        }
    }
}

void nhanMaTran(double **A, int n, int m, double **B, int n2, int m2, double **C)
{
    if (m != n2)
    {
        cout << "loi\n";
        return;
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            C[i][j] = 0;
            for (int t = 0; t < m; ++t)
            {
                C[i][j] += A[i][t] * B[t][j];
            }
        }
    }
}

void daoHam(int m, double **nhanXX, double **nhanXY, double **weight, double **g)
{
    for (int i = 0; i < m; i++)
    {
        g[i][0] = 0;
        for (int j = 0; j < m; j++)
        {
            g[i][0] += nhanXX[i][j] * weight[j][0];
        }
    }
    for (int i = 0; i < m; i++)
    {
        g[i][0] -= nhanXY[i][0];
    }
}

int findMax(int m, double **g)
{
    double h = fabs(g[0][0]);
    int index = 0;
    for (int i = 1; i < m; i++)
        h = (h < fabs(g[i][0])) ? (fabs(g[i][0]) + (index = i)) : (h);

    mx = h;
    return index;
}

void sort(int n, int m, double **Xval, double **Yval)
{
    double temp;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (Xval[i][0] > Xval[j][0])
            {
                Swap(Yval[i][0], Yval[j][0]);
                for (int k = 0; k < m; k++)
                {
                    Swap(Xval[j][k], Xval[i][k]);
                }
            }
        }
    }
}

void Swap(double &a, double &b)
{
    double temp = a;
    a = b;
    b = temp;
}





