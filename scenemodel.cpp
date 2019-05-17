#include "scenemodel.h"
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QAbstractTableModel>
#include <QDebug>
#include "video.h"
#include <QString>
#include <QColor>
#include "vsexception.h"

SceneModel::SceneModel(QObject *parent, QString scenePath)
    : QAbstractTableModel(parent) {
    loadScene(scenePath);
}

SceneModel::SceneModel(QObject *parent, Scene * scene)
    : QAbstractTableModel(parent) {
    theScene = scene;
}

QVariant SceneModel::headerData(int section, Qt::Orientation orientation, int role) const {
    QString val = "";

    if (role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal ) {
        switch(section) {
        case 0:
            val = "Name";
            break;
        case 1:
            val = "Midi note";
            break;
        case 2:
            val = "Loop at end";
            break;
        case 3:
            val = "Blur type";
            break;
        case 4:
            val = "Zoom type";
        }

    } else if(orientation == Qt::Vertical) {
        val = val.setNum(section + 1);
    }

    return val;
}

bool SceneModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    qDebug("set header data...");
//    if (value != headerData(section, orientation, role)) {
//        // FIXME: Implement me!
//        emit headerDataChanged(orientation, section, section);
//        return true;
//    }
    return false;
}


int SceneModel::rowCount(const QModelIndex &parent) const
{
    return theScene->videos.size();
}

int SceneModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

/**
 * @brief SceneModel::data row selects videos, column selects video data
 * @param index
 * @param role
 * @return
 */
QVariant SceneModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;

    if(index.isValid() && theScene->videos.size() > index.row()) {
        switch(index.column()) {
        case 0:
            if(role == Qt::EditRole || role == Qt::DisplayRole) {
                ret = theScene->videos[index.row()].name;
            }
            break;
        case 1:
            if(role == Qt::EditRole || role == Qt::DisplayRole) {
                ret = theScene->videos[index.row()].midiNote;
            }
            break;
        case 2:
            if(role == Qt::EditRole) {
                ret = theScene->videos[index.row()].loopAtEnd;
            } else if(role == Qt::DisplayRole) {
                ret = theScene->videos[index.row()].loopAtEnd ? "True" : "False";
            }
            break;
        case 3:
            if(role == Qt::EditRole) {
                ret = theScene->videos[index.row()].blurType;
            } else if(role == Qt::DisplayRole) {
                ret = Video::blurTypes[theScene->videos[index.row()].blurType];
            }

            break;
        case 4:
            if(role == Qt::EditRole) {
                ret = theScene->videos[index.row()].zoomType;
            } else if(role == Qt::DisplayRole) {
                ret = Video::zoomTypes[theScene->videos[index.row()].zoomType];
            }

            break;
        }

        if(role == Qt::ForegroundRole && !theScene->videos[index.row()].ok) {
            ret = QVariant::fromValue(QColor(Qt::red));
        }
    }

    return ret;
}

bool SceneModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug() << "Setting data... " << role;

    QVariant dataz = data(index, role);

    if (dataz.isValid() && dataz != value && role == Qt::EditRole) {

        //updates model
        switch(index.column()) {
        case 0:
            theScene->videos[index.row()].name = value.toString();
            break;
        case 1:
            theScene->videos[index.row()].midiNote = value.toInt();
            break;
        case 2:
            theScene->videos[index.row()].loopAtEnd = value.toBool();
            break;
        case 3:
            theScene->videos[index.row()].blurType = value.toInt();
            break;
        case 4:
            theScene->videos[index.row()].zoomType = value.toInt();
            break;
        }

        //persists changes on file system
        persistVideoData(theScene);
        theScene->verify();
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags SceneModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    //all cells except the ones in the firsto column are editable
    if(index.column() > 0) {
        return Qt::ItemIsEditable| QAbstractTableModel::flags(index);
    } else {
        return Qt::ItemIsSelectable| QAbstractTableModel::flags(index);

    }
}

bool SceneModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);

    endInsertRows();
    return true;
}


bool SceneModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);

    endRemoveRows();
    return true;
}

//Not supported
bool SceneModel::insertColumns(int column, int count, const QModelIndex &parent) {
    return false;
}

bool SceneModel::removeColumns(int column, int count, const QModelIndex &parent) {
    return false;
}
//Not supported


//scene stuff
QString SceneModel::getScenePath() {
    QString ret("");
    if(theScene != nullptr) {
        ret = theScene->scenePath;
    }

    return ret;
}
/**
 * @brief SceneModel::persistVideoData persist data about videos
 * @param s
 */
void SceneModel::persistVideoData(Scene * s) {
   s->saveVideoData();
}

void SceneModel::persistVideoData() {
    persistVideoData(theScene);
}

void SceneModel::persistNewVideo(Scene * s, Video * newVideo) {
    persistVideoData(s);
}
void SceneModel::deleteVideo(Scene * s, Video * killVideo) {
    persistVideoData(s);
}
void SceneModel::loadScene(QString scenePath) {
    if(theScene != nullptr) {
        delete(theScene);
    }

    theScene = new Scene(scenePath);
}

void SceneModel::reloadScene() {
    beginResetModel();
    theScene->reload();
    endResetModel();
}

void SceneModel::selectVideo(int index) {
    qDebug() << "selected video " << index;
    if(index >= 0 && index < theScene->videos.size()) {
        selectedVideoIndex = index;
    }
}

void SceneModel::removeSelectedVideo() {
    qDebug() << "remove selected...";
    removeVideo(selectedVideoIndex);
    qDebug() << "done";
}

void SceneModel::removeVideo(int indexToRemove) {
    qDebug() << "About to remove video " << indexToRemove;
    if(indexToRemove != -1) {
        const QModelIndex qmi;
        beginRemoveRows(qmi, indexToRemove, indexToRemove);
        qDebug() << "Begin emitted";
        theScene->removeVideo(indexToRemove);
        endRemoveRows();
        qDebug() << "End emitted";
    }
}


void SceneModel::addNewVideo(QString videoPath) {
    const QModelIndex qmi;
    beginInsertRows(qmi, theScene->videos.size(), theScene->videos.size());
    theScene->addVideoFile(videoPath);
    endInsertRows();
}

void SceneModel::replaceVideo(QString videoPath) {
    QFileInfo fi(videoPath);
    removeVideo(theScene->indexOf(fi.fileName()));
    addNewVideo(videoPath);
}

bool SceneModel::hasVideo(QString path) {
    QFileInfo fi(path);
    return theScene->hasVideo(fi.fileName());
}
