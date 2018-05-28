#ifndef SCENEMODEL_H
#define SCENEMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include "scene.h"
#include "video.h"

class SceneModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SceneModel(QObject *parent = nullptr, QString scenePath = "");
    explicit SceneModel(QObject *parent, Scene * scene);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    QString getScenePath();
    void reloadScene();
    int selectedVideoIndex = -1;
    void selectVideo(int index);
    void removeSelectedVideo();
    void persistVideoData();
private:
    //Modification of underlying scene
    void persistVideoData(Scene * s);
    Scene * theScene;
    void persistNewVideo(Scene * s, Video * newVideo);
    void deleteVideo(Scene * s, Video * killVideo);
    void loadScene(QString scenePath);
};

#endif // SCENEMODEL_H
