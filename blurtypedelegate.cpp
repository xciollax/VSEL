#include "blurtypedelegate.h"
#include <QComboBox>
#include "video.h"


BlurtypeDelegate::BlurtypeDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

QWidget *BlurtypeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    //init editor
    QComboBox *editor = new QComboBox(parent);
    editor->setFrame(false);

    for(int i = 0; i < 3; i++) {
        editor->insertItem(i, Video::blurTypes[i], i);
    }

    return editor;
}

void BlurtypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //init editor value
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QComboBox *cBox = static_cast<QComboBox*>(editor);

    cBox->setCurrentIndex(cBox->findData(value, Qt::EditRole));
}

void BlurtypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //modify model data
    QComboBox *cBox = static_cast<QComboBox*>(editor);
    int value = cBox->currentData().toInt();

    model->setData(index, value, Qt::EditRole);
}

void BlurtypeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

