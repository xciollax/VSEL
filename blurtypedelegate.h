#ifndef BLURTYPEDELEGATE_H
#define BLURTYPEDELEGATE_H

#include <QStyledItemDelegate>

class BlurtypeDelegate : public QStyledItemDelegate {
    Q_OBJECT

private:
    int minValue;
    int maxValue;
public:
    BlurtypeDelegate(QObject *parent);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // BLURTYPEDELEGATE_H
