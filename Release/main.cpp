#include "mainwindow.h"
#include <QApplication>
#include"Graphs/DirGraph.h"
#include<ctime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    std::srand(time(0));
    int size = 5;
    byte** mat = new byte*[size];
    for(int i = 0; i < size; i++){
        mat[i] = new byte[size];
        for(int j = 0; j < size; j++){
            mat[i][j] = (i+j)%3%2;
        }
    }
    class DirGraph G2(size, mat);
    std::fstream output("E:/Qt creator/Way_Editor/test.txt");
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            output << (mat[i][j] ? 1 : 0) << ' ';
        }
        output << '\n';
    }
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            mat[i][j] = (j>i);
        }
    }
    output << std::endl;
    for(int i = 0; i < size-1; i++){
        for(int j = 0; j < size-1; j++){
            output << (mat[i][j] ? 1 : 0) << ' ';
        }
        output << '\n';
    }
    class DirGraph G1(size-1, mat);
    G1.print(output);
    G2 -= G1;
    G2.print(output);
    output.close();
    for(size_t i = 0; i < size; i++){
        delete mat[i];
    }
    delete[] mat;
    w.show();
    return a.exec();
}
