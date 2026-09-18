#pragma once
#include <QMainWindow>
class QLabel;
class QListWidget;
class QTableWidget;
class MusicTrack;
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
private slots:
    void refreshSongs();
    void addPlaylist();
    void deletePlaylist();
    void copyPlaylist();
    void addSong();
    void removeLastSong();
    void playByIndex();
    void comparePlaylists();
    void showCommonSongs();
    void showUniqueSongs();
    void showLongestSong();
private:
    void refreshPlaylists();
    int currentPlaylist() const;
    bool hasCurrentPlaylist() const;
    void showSongResult(const QString& heading, MusicTrack& result);
    QListWidget* playlistList;
    QTableWidget* songTable;
    QLabel* emptyState;
    QLabel* totalCount;
};
