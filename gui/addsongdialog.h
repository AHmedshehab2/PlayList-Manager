#pragma once

#include <QDialog>

class QDoubleSpinBox;
class QLineEdit;

class AddSongDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddSongDialog(QWidget* parent = nullptr);

    QString getTitle() const;
    double getDuration() const;

private:
    QLineEdit* titleEdit;
    QDoubleSpinBox* durationSpinBox;
};
