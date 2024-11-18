#include "mainwindow.h"
#include <QApplication>
#include"Graphs/UDirGraph.h"
#include"Graphs/DirGraph.h"
#include<ctime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    std::srand(time(0));
    int size = rand()%50 + 20;
    byte** mat = new byte*[size];
    for(int i = 0; i < size; i++){
        mat[i] = new byte[size];
        for(int j = 0; j < size; j++){
            mat[i][j] = rand()%2;
        }
    }
    DirGraph G1(size, mat);
    UDirGraph G2(size, mat);
    std::fstream output("E:/Qt creator/Way_Editor/test.txt");
    G1.print(output);
    G2.print(output);
    output.close();
    for(size_t i = 0; i < size; i++){
        delete mat[i];
    }
    delete[] mat;
    w.show();
    return a.exec();
}
