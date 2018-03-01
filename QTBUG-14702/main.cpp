#include <QCoreApplication>
#include <Qt3DCore/Qt3DCore>
#include <Qt3DExtras/Qt3DExtras>

#include <d3d11.h> // inclding D3D headers after Qt causes compiler errors

int main(int argc, char *argv[]) {
    QGuiApplication a(argc, argv);
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->setTitle(QStringLiteral("3D application"));
    view->defaultFrameGraph()->setClearColor(QColor(210, 210, 220));

    auto *root = new Qt3DCore::QEntity();
    view->setRootEntity(root);
    view->show();

    return a.exec();
}
