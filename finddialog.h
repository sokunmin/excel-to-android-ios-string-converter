#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class FindDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FindDialog(QWidget *parent = 0);

signals:
    void findNext(const QString &text, bool checkWholeWord);
    void findPrev(const QString &text, bool checkWholeWord);

public slots:
    void nextClick();
    void prevClick();

private:

    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *wholeWordBox;
    QDialogButtonBox *buttonBox;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QWidget *extension;
};

#endif // FINDDIALOG_H
