#ifndef HEXEXPORTER_H
#define HEXEXPORTER_H

#include <QObject>

class HexBlocker;


class HexExporter : public QObject
{
    Q_OBJECT
public:
    HexExporter();
    HexExporter(HexBlocker *);

    void exporBlockMeshDict(QString filename);
    HexBlocker *hexB;

};

#endif // HEXEXPORTER_H
