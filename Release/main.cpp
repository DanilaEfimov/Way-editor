#include "mainwindow.h"
#include "Graphs/DirGraph.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    size_t size = 5;
    byte** mat = new byte*[size];
    for(size_t i = 0; i < size; i++){
        mat[i] = new byte[size]{0};
        for(size_t j = 0; j < size; j++){
            mat[i][j] = i > j ? (2*i+1)%3%2 : (2*j+1)%3%2;
            qDebug() << (int)mat[i][j] << ' ';
        }
        qDebug() << '\n';
    }
    DirGraph G(size, mat);
    std::stack<uint> c;
    c.push(1);
    c.push(2);
    c.push(3);
    qDebug() << G.show();
    G.DirGraph::operator+(c);
    qDebug() << G.show();
    w.show();
    return a.exec();
}
