#ifndef ZOOMTYPEDELEGATE_H
#define ZOOMTYPEDELEGATE_H

#include <QStyledItemDelegate>

class ZoomtypeDelegate : public QStyledItemDelegate {
    Q_OBJECT
    public:
    ZoomtypeDelegate(QObject *parent);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // ZOOMTYPEDELEGATE_H
