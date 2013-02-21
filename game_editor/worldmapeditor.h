#ifndef WORLDMAPEDITOR_H
#define WORLDMAPEDITOR_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "datatree.hpp"

namespace Ui {
class WorldmapEditor;
}

class WorldmapEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit WorldmapEditor(QWidget *parent = 0);
    ~WorldmapEditor();

public slots:
    void Load(void);
    void Save(void);

    void SelectedCity(QString);
    void UpdateCityData(void);
    void UpdateMapData(void);

private:
    typedef QPair<QString, QGraphicsEllipseItem*> CityHalo;
    typedef QList<CityHalo>                       CityHalos;

    CityHalo& GetCityHalo(const QString& name);
    void      AddCityHalo(const QString& name);
    void      DelCityHalo(const QString& name);

    Ui::WorldmapEditor  *ui;
    QGraphicsScene      scene;
    QGraphicsPixmapItem pixmap_worldmap;
    DataTree*           file_map;
    DataTree*           file_cities;

    CityHalos           city_halos;

    bool                lock_cities;
};

#endif // WORLDMAPEDITOR_H
