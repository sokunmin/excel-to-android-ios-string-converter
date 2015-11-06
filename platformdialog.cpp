#include "platformdialog.h"
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>

PlatformDialog::PlatformDialog(QWidget *parent) :
    QDialog(parent)
{
    radioGroup        = new QButtonGroup(this);
    androidButton     = new QRadioButton(tr("&Android (*.xml)"), this);
//    iosButton         = new QRadioButton(tr("&iOS (*.strings)"), this);
    windowsButton     = new QRadioButton(tr("&Windows (*.xml)"), this);
    macosxButton      = new QRadioButton(tr("&Mac OS X / iOS (*.strings)"), this);
    engineButton      = new QRadioButton(tr("&Engine Board (*.??)"), this);
    marketExcelButton = new QRadioButton(tr("&Marketing Excel (*.xlsx)"), this);

    androidButton->setChecked(true);
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addWidget(androidButton  );
//    vlayout->addWidget(iosButton      );
    vlayout->addWidget(windowsButton  );
    vlayout->addWidget(macosxButton   );
    vlayout->addWidget(engineButton   );
    vlayout->addWidget(marketExcelButton);

    radioGroup->addButton(androidButton, 0);
//    radioGroup->addButton(iosButton    , 1);
    radioGroup->addButton(windowsButton, 1);
    radioGroup->addButton(macosxButton , 2);
    radioGroup->addButton(engineButton , 3);
    radioGroup->addButton(marketExcelButton, 4);

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
