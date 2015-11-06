#ifndef PLATFORMDIALOG_H
#define PLATFORMDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QRadioButton;
class QButtonGroup;
QT_END_NAMESPACE

namespace Platform {
    enum {
        ANDROID,
//        IOS,
        WINDOWS,
        MACOSX,
        ENGINE_BOARD,
        MARKET_EXCEL
    };
}

class PlatformDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PlatformDialog(QWidget *parent = 0);

signals:

    void ok(const int index);

private slots:
    void okClick();
    void cancelClick();

private:
    QButtonGroup *radioGroup;
    QRadioButton *androidButton;
//    QRadioButton *iosButton;
    QRadioButton *windowsButton;
    QRadioButton *macosxButton;
    QRadioButton *engineButton;
    QRadioButton *marketExcelButton;


    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // PLATFORMDIALOG_H
