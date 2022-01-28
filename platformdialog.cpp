#include "platformdialog.h"
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>

PlatformDialog::PlatformDialog(QWidget *parent) :
    QDialog(parent)
{
    radioGroup    = new QButtonGroup(this);
    excelButton   = new QRadioButton(tr("&Excel (*.xlsx)"), this);
    androidButton = new QRadioButton(tr("&Android (*.xml)"), this);
    iosButton     = new QRadioButton(tr("&iOS (*.strings)"), this);
//    webButton     = new QRadioButton(tr("&Web (*.json)"), this);

    excelButton->setChecked(true);
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addWidget(excelButton);
    vlayout->addWidget(androidButton);
    vlayout->addWidget(iosButton);
//    vlayout->addWidget(webButton);

    radioGroup->addButton(excelButton, 0);
    radioGroup->addButton(androidButton, 1);
    radioGroup->addButton(iosButton , 2);
//    radioGroup->addButton(webButton , 3);
    okButton = new QPushButton(tr("OK"));
    cancelButton = new QPushButton(tr("Cancel"));

    QGroupBox  *vgroup   = new QGroupBox();
    vgroup->setLayout(vlayout);

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(okButton);
    hlayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(vgroup);
    mainLayout->addLayout(hlayout);

    setLayout(mainLayout);

    connect(okButton, SIGNAL(clicked()), this, SLOT(okClick()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClick()));
}


void PlatformDialog::okClick()
{
    emit ok(radioGroup->checkedId());
    close();
}

void PlatformDialog::cancelClick()
{
    this->close();
}
