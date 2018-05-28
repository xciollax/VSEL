#ifndef SCENEEDITDELEGATE_H
#define SCENEEDITDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QAbstractItemModel>

class NoteEditDelegate : public QStyledItemDelegate {
    Q_OBJECT

private:
    int minValue;
    int maxValue;
public:
    NoteEditDelegate(QObject *parent, int minValue, int maxValue);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // SCENEEDITDELEGATE_H
