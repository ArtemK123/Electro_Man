#include "window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    try
    {
        QApplication a(argc, argv);
        Window w;
        w.show();

        return a.exec();

    } catch(string e) {
        cout<<"Error: "<<e<<endl;
    }
}
