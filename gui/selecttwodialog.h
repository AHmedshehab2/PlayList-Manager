#pragma once
#include <QDialog>
class QComboBox;
class SelectTwoDialog : public QDialog {
    Q_OBJECT
public:
    explicit SelectTwoDialog(QWidget* parent = nullptr);
    int getFirst() const;
    int getSecond() const;
private:
    QComboBox* firstCombo;
    QComboBox* secondCombo;
};
