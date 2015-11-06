#include <QtWidgets>

#include "finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent)
{

    label = new QLabel(tr("Find &what:"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);
    lineEdit->setFocus(Qt::TabFocusReason);

    wholeWordBox = new QCheckBox(tr("Whole Word"));

    nextButton = new QPushButton(tr("&Next"));
    nextButton->setDefault(true);

    prevButton = new QPushButton(tr("&Previous"));
    nextButton->setDefault(true);

    extension = new QWidget;

    buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(nextButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(prevButton, QDialogButtonBox::ActionRole);

    QVBoxLayout *extensionLayout = new QVBoxLayout;
    extensionLayout->setMargin(0);
    extension->setLayout(extensionLayout);

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(lineEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(wholeWordBox);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(leftLayout, 0, 0);
    mainLayout->addWidget(buttonBox, 0, 1);
    mainLayout->addWidget(extension, 1, 0, 1, 2);
    mainLayout->setRowStretch(2, 1);

    setLayout(mainLayout);

    setWindowTitle(tr("Find..."));
    extension->hide();

    connect(nextButton, SIGNAL(clicked()), this, SLOT(nextClick()));
    connect(prevButton, SIGNAL(clicked()), this, SLOT(prevClick()));

}

void FindDialog::nextClick()
{
    qDebug() << Q_FUNC_INFO;
    emit findNext(lineEdit->text(), wholeWordBox->isChecked());
}

void FindDialog::prevClick()
{
    qDebug() << Q_FUNC_INFO;
    emit findPrev(lineEdit->text(), wholeWordBox->isChecked());
}
