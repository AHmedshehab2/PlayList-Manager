#include "addsongdialog.h"

#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLineEdit>

AddSongDialog::AddSongDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Add Song");
    titleEdit = new QLineEdit(this);
    durationSpinBox = new QDoubleSpinBox(this);
    durationSpinBox->setRange(0.1, 600.0);
    durationSpinBox->setDecimals(1);
    durationSpinBox->setSuffix(" min");

    auto* form = new QFormLayout(this);
    form->addRow("Title", titleEdit);
    form->addRow("Duration", durationSpinBox);
    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    form->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString AddSongDialog::getTitle() const { return titleEdit->text().trimmed(); }

double AddSongDialog::getDuration() const { return durationSpinBox->value(); }
