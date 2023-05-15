#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "password_crypt.h"
#include "api_helpers.h"
#include "httplib.h"
#include "logindialog.h"

#include <QMediaPlayer>
#include <QNetworkRequest>
#include <QPixmap>
#include <QShortcut>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::try_login() {
    std::string key = "VQZBJ6TD8M9WBUWT";
    std::string iv = "joiwef08u23j341a";

    LoginDialog dlg(this);

    if (dlg.exec()) {
        this->pass = dlg.getPass().toStdString();
        this->email = dlg.getEmail().toStdString();
        std::string pass_encrypted = password_crypt(this->pass, iv, key);
        std::string url = "/api/login.action?m=1&uid="+this->email+"&pwd="+pass_encrypted;
        httplib::Client cli("https://www.storytel.com");
        auto res = cli.Get(url);
        if (res->status!=200) {
            return false;
        }
        this->loginResponse = res->body;
        return true;
    }
    return false;
}

void MainWindow::showList() {
    if (try_login()) {
        this->token = getToken(this->loginResponse);
        std::string bookshelf = getBookshelf(token);
        this->books = parseEntries(bookshelf);

        for (auto &it : this->books) {
            QListWidgetItem * newItem = new QListWidgetItem;
            newItem->setText(QString::fromStdString(it.book.title));
            ui->listWidget->addItem(newItem);
        }

        connect(ui->listWidget, &QListWidget::currentRowChanged, this, &MainWindow::displayCover);

    } else {
        QMessageBox::warning(
            this,
            tr("Login"),
            tr("Incorrect login information.") );
        this->close();
    }
}

void MainWindow::displayCover() {
    this->idx = ui->listWidget->currentRow();
    const QUrl coverUrl("https://storytel.com"+QString::fromStdString(this->books[this->idx].imgUrl));
    this->cd = new CoverDownloader(coverUrl, this);
    connect(this->cd, SIGNAL (downloaded()), this, SLOT(loadImage()));
}

void MainWindow::loadImage() {
    QPixmap img;
    img.loadFromData(this->cd->downloadedData());
    ui->coverView->setPixmap(img);
}

void MainWindow::mediaPlayerInit() {
    this->player = new QMediaPlayer;
    this->audioOutput = new QAudioOutput;
    this->audioOutput->setVolume(80);
    this->player->setAudioOutput(audioOutput);
}

void MainWindow::on_playBtn_clicked()
{
    this->idx = ui->listWidget->currentRow();
    const QUrl streamUrl = QUrl("https://www.storytel.com/mp3streamRangeReq?startposition=0&programId="+
                                QString::number(books[this->idx].abook.id)+"&token="+QString::fromStdString(this->token));

    this->player->setSource(streamUrl);
    this->player->play();
    sliderInit();
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        ui->horizontalSlider->setValue(ui->horizontalSlider->value() + 1000);
    });
    timer->start(1000);
    connect(this->player, &QMediaPlayer::mediaStatusChanged, this, [=]() {
        player->setPosition(ui->horizontalSlider->value());
    });

}
void MainWindow::on_pauseBtn_clicked()
{
    int64_t duration = player->position();
    setBookmark(this->token, std::to_string(this->books[this->idx].bookmark.id), duration);
    player->pause();
}


void MainWindow::on_forwardBtn_clicked()
{
    if (player->playbackState() == QMediaPlayer::PlayingState) player->setPosition(player->position() + 5000);

}


void MainWindow::on_backBtn_clicked()
{
    if (player->playbackState() == QMediaPlayer::PlayingState) player->setPosition(player->position() - 5000);

}

void MainWindow::sliderInit() {
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(this->books[this->idx].length);
    ui->horizontalSlider->setValue((this->books[this->idx].bookmark.position == -1) ? 0 : this->books[this->idx].bookmark.position/1000);
    QObject::connect(ui->horizontalSlider, &QSlider::sliderMoved, this, &MainWindow::sliderSeek);
}

void MainWindow::initKeyboardShortcuts() {
    QShortcut *shortcut_Fwd = new QShortcut(QKeySequence(Qt::Key_Right), this);
    QShortcut *shortcut_Back = new QShortcut(QKeySequence(Qt::Key_Left), this);

    QObject::connect(shortcut_Fwd, &QShortcut::activated, this, &MainWindow::on_forwardBtn_clicked);
    QObject::connect(shortcut_Back, &QShortcut::activated, this, &MainWindow::on_backBtn_clicked);

}

void MainWindow::sliderSeek(int millis) {
    this->player->setPosition(millis);
}

