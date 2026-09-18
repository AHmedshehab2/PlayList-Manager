#include "mainwindow.h"
#include <QAbstractItemView>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include "addsongdialog.h"
#include "musictrack.h"
#include "selecttwodialog.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Music Playlist Manager");
    resize(900, 560);
    setStyleSheet("QMainWindow { background: #20252b; color: #e8edf2; }"
                  "QPushButton { padding: 6px; }"
                  "QListWidget, QTableWidget { background: #2b323a; color: #e8edf2;"
                  " border: 1px solid #4a5663; }"
                  "QHeaderView::section { background: #39434e; color: #e8edf2; padding: 5px; }");
    auto* central = new QWidget(this);
    auto* mainLayout = new QHBoxLayout(central);
    auto* leftLayout = new QVBoxLayout;
    auto* rightLayout = new QVBoxLayout;
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 3);
    setCentralWidget(central);
    leftLayout->addWidget(new QLabel("Playlists", this));
    playlistList = new QListWidget(this);
    leftLayout->addWidget(playlistList);
    auto* playlistButtons = new QGridLayout;
    auto* addPlaylistButton = new QPushButton("Add Playlist", this);
    auto* copyPlaylistButton = new QPushButton("Copy Playlist", this);
    auto* deletePlaylistButton = new QPushButton("Delete Playlist", this);
    playlistButtons->addWidget(addPlaylistButton, 0, 0);
    playlistButtons->addWidget(copyPlaylistButton, 0, 1);
    playlistButtons->addWidget(deletePlaylistButton, 1, 0, 1, 2);
    leftLayout->addLayout(playlistButtons);
    totalCount = new QLabel(this);
    leftLayout->addWidget(totalCount);
    songTable = new QTableWidget(this);
    songTable->setColumnCount(2);
    songTable->setHorizontalHeaderLabels({"Title", "Duration"});
    songTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    songTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    songTable->horizontalHeader()->setStretchLastSection(true);
    rightLayout->addWidget(songTable);
    emptyState = new QLabel("This playlist has no songs.", this);
    emptyState->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(emptyState);
    auto* songButtons = new QGridLayout;
    const QList<QPair<QString, void (MainWindow::*)()>> actions = {
        {"Add Song", &MainWindow::addSong}, {"Remove Last Song", &MainWindow::removeLastSong},
        {"Play by Index", &MainWindow::playByIndex}, {"Compare", &MainWindow::comparePlaylists},
        {"Common Songs", &MainWindow::showCommonSongs}, {"Unique Songs", &MainWindow::showUniqueSongs},
        {"Longest Song", &MainWindow::showLongestSong}
    };
    for (int i = 0; i < actions.size(); ++i) {
        auto* button = new QPushButton(actions[i].first, this);
        songButtons->addWidget(button, i / 3, i % 3);
        connect(button, &QPushButton::clicked, this, actions[i].second);
    }
    rightLayout->addLayout(songButtons);
    connect(addPlaylistButton, &QPushButton::clicked, this, &MainWindow::addPlaylist);
    connect(copyPlaylistButton, &QPushButton::clicked, this, &MainWindow::copyPlaylist);
    connect(deletePlaylistButton, &QPushButton::clicked, this, &MainWindow::deletePlaylist);
    connect(playlistList, &QListWidget::currentRowChanged, this, &MainWindow::refreshSongs);
    refreshPlaylists();
}

MainWindow::~MainWindow() {
    for (MusicTrack* playlist : Player) delete playlist;
    Player.clear();
}

int MainWindow::currentPlaylist() const { return playlistList->currentRow(); }
bool MainWindow::hasCurrentPlaylist() const { return currentPlaylist() >= 0 && currentPlaylist() < static_cast<int>(Player.size()); }

void MainWindow::refreshPlaylists() {
    const int selected = currentPlaylist();
    playlistList->clear();
    if (Player.empty()) {
        auto* empty = new QListWidgetItem("No playlists yet. Create a playlist to get started.", playlistList);
        empty->setFlags(empty->flags() & ~Qt::ItemIsEnabled);
    } else {
        for (int i = 0; i < static_cast<int>(Player.size()); ++i)
            playlistList->addItem(QString("Playlist %1 (%2 songs)").arg(i + 1).arg(Player[i]->get_Playlist_size()));
        playlistList->setCurrentRow(qBound(0, selected, playlistList->count() - 1));
    }
    totalCount->setText(QString("Total playlists: %1").arg(MusicTrack::get_totalPlayList_created()));
}

void MainWindow::refreshSongs() {
    songTable->setRowCount(0);
    if (!hasCurrentPlaylist()) { emptyState->setVisible(false); return; }
    MusicTrack* playlist = Player[currentPlaylist()];
    const int size = playlist->get_Playlist_size();
    songTable->setRowCount(size);
    for (int i = 0; i < size; ++i) {
        const Song song = playlist->get_song(i);
        songTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(song.title)));
        songTable->setItem(i, 1, new QTableWidgetItem(QString::number(song.duration, 'f', 1)));
    }
    emptyState->setVisible(size == 0);
}

void MainWindow::addPlaylist() { Player.push_back(new MusicTrack(5)); refreshPlaylists(); }

void MainWindow::deletePlaylist() {
    if (!hasCurrentPlaylist()) return;
    const int row = currentPlaylist();
    if (QMessageBox::question(this, "Delete Playlist", "Delete the selected playlist?") != QMessageBox::Yes) return;
    delete Player[row];
    Player.erase(Player.begin() + row);
    MusicTrack::decrementTotal();
    refreshPlaylists();
    refreshSongs();
}

void MainWindow::copyPlaylist() {
    if (!hasCurrentPlaylist()) return;
    Player.push_back(new MusicTrack(*Player[currentPlaylist()]));
    refreshPlaylists();
}

void MainWindow::addSong() {
    if (!hasCurrentPlaylist()) return;
    AddSongDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted || dialog.getTitle().isEmpty()) return;
    MusicTrack* playlist = Player[currentPlaylist()];
    playlist->addNewSong({dialog.getTitle().toStdString(), dialog.getDuration()}, playlist->get_Playlist_size());
    refreshPlaylists();
    refreshSongs();
}

void MainWindow::removeLastSong() {
    if (!hasCurrentPlaylist()) return;
    (*Player[currentPlaylist()])--;
    refreshPlaylists();
    refreshSongs();
}

void MainWindow::playByIndex() {
    if (!hasCurrentPlaylist() || Player[currentPlaylist()]->get_Playlist_size() == 0) {
        QMessageBox::information(this, "Play by Index", "The selected playlist has no songs.");
        return;
    }
    bool ok = false;
    const int index = QInputDialog::getInt(this, "Play by Index", "Song index:", 0, 0, Player[currentPlaylist()]->get_Playlist_size() - 1, 1, &ok);
    if (!ok) return;
    const Song song = (*Player[currentPlaylist()])[index];
    QMessageBox::information(this, "Now Playing", QString::fromStdString(song.title) + "\nDuration: " + QString::number(song.duration, 'f', 1) + " min");
}

void MainWindow::comparePlaylists() {
    if (Player.size() < 2) { QMessageBox::information(this, "Compare", "Create at least two playlists first."); return; }
    SelectTwoDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) return;
    const int first = dialog.getFirst();
    const int second = dialog.getSecond();
    const QString result = (*Player[first] >= *Player[second]) ? QString("Playlist %1 has more or equal songs.").arg(first + 1) : QString("Playlist %1 has more songs.").arg(second + 1);
    QMessageBox::information(this, "Compare", result);
}

void MainWindow::showSongResult(const QString& heading, MusicTrack& result) {
    QString text;
    for (int i = 0; i < result.get_Playlist_size(); ++i) {
        const Song song = result.get_song(i);
        text += QString::fromStdString(song.title) + " - " + QString::number(song.duration, 'f', 1) + " min\n";
    }
    QMessageBox::information(this, heading, text.isEmpty() ? "No songs found." : text);
}

void MainWindow::showCommonSongs() {
    if (Player.size() < 2) { QMessageBox::information(this, "Common Songs", "Create at least two playlists first."); return; }
    SelectTwoDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) return;
    MusicTrack result = *Player[dialog.getFirst()] + *Player[dialog.getSecond()];
    showSongResult("Common Songs", result);
    // Operator+ creates temporary MusicTrack objects that are not stored in Player.
    MusicTrack::decrementTotal();
    MusicTrack::decrementTotal();
}

void MainWindow::showUniqueSongs() {
    if (Player.size() < 2) { QMessageBox::information(this, "Unique Songs", "Create at least two playlists first."); return; }
    SelectTwoDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) return;
    MusicTrack result = *Player[dialog.getFirst()] - *Player[dialog.getSecond()];
    showSongResult("Unique Songs", result);
    // Operator- creates temporary MusicTrack objects that are not stored in Player.
    MusicTrack::decrementTotal();
    MusicTrack::decrementTotal();
}

void MainWindow::showLongestSong() {
    if (Player.empty()) { QMessageBox::information(this, "Longest Song", "You have 0 playlists."); return; }
    Song longest{"", 0};
    for (MusicTrack* playlist : Player) {
        for (int i = 0; i < playlist->get_Playlist_size(); ++i) {
            const Song song = playlist->get_song(i);
            if (song.duration > longest.duration) longest = song;
        }
    }
    if (longest.title.empty()) { QMessageBox::information(this, "Longest Song", "There are no songs yet."); return; }
    QMessageBox::information(this, "Longest Song", QString::fromStdString(longest.title) + "\nDuration: " + QString::number(longest.duration, 'f', 1) + " min");
}
