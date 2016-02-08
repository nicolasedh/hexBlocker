#ifndef VERTICEEDITORWIDGET_H
#define VERTICEEDITORWIDGET_H

#include <QDockWidget>

//Predeclarations
class HexBlocker;
class PointsTableModel;

namespace Ui {
class VerticeEditorWidget;
}

class VerticeEditorWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit VerticeEditorWidget(QWidget *parent = 0);
    ~VerticeEditorWidget();
    
    void setHexBlocker(HexBlocker *hexBlk);

public slots:
    void updateVertices();
    void slotPointChanged();
    void slotSetScale();
    void displayScale(double scale);
    void slotSetScaleGeometry(int state);

signals:
    void apply();
    void setStatusText(QString);

private:
    Ui::VerticeEditorWidget *ui;
    PointsTableModel * table;
    //DATA
    HexBlocker *hexBlocker;
};

#endif // VERTICEEDITORWIDGET_H
