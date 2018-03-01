#include <QApplication>
#include <atlbase.h>    // example COM header that causes MOC problems


class MyClass : public QObject {
    Q_OBJECT
public:
    MyClass(QObject *parent = 0) {
        emit Signal();
    }
    ~MyClass() {}

signals:
    void Signal();
};


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    return a.exec();
}
