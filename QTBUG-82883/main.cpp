#include <QAxBindable>
#include <QAxFactory>
#include <QMainWindow>


class IdlVersion : public QMainWindow, public QAxBindable
{
    Q_OBJECT
    Q_CLASSINFO("ClassID",     "{7604C891-E4B8-47F0-BF5B-756C955D8D14}")
    Q_CLASSINFO("InterfaceID", "{65756C41-F8B2-4CAE-9201-10AD922E35E0}")
    Q_CLASSINFO("EventsID",    "{B9EB3BAF-CD26-4874-B24D-1D8F8F1F79C9}")
public:
    explicit IdlVersion(QWidget *parent = nullptr)
    : QMainWindow(parent)
    {
    }

    ~IdlVersion() override {
    }
    
signals:
    void someSignal();
};

#include "main.moc"

QAXFACTORY_BEGIN(
    "{E8D9236B-C212-4952-A386-6F63A8A8F30A}", // type library ID
    "{0065F399-D6D9-458F-AC92-B879571A0C9C}") // application ID
    QAXCLASS(IdlVersion)
QAXFACTORY_END()
