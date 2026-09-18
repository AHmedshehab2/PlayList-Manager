#include "selecttwodialog.h"
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include "musictrack.h"

SelectTwoDialog::SelectTwoDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Select Playlists");
    firstCombo = new QComboBox(this);
    secondCombo = new QComboBox(this);
    for (int i = 0; i < static_cast<int>(Player.size()); ++i) {
        const QString label = QString("Playlist %1 (%2 songs)").arg(i + 1).arg(Player[i]->get_Playlist_size());
        firstCombo->addItem(label, i);
        secondCombo->addItem(label, i);
    }
    auto* form = new QFormLayout(this);
    form->addRow("First", firstCombo);
    form->addRow("Second", secondCombo);
    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    form->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

int SelectTwoDialog::getFirst() const { return firstCombo->currentData().toInt(); }
int SelectTwoDialog::getSecond() const { return secondCombo->currentData().toInt(); }
