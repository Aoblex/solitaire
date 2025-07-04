#include "mainwindow.h"
#include <QStackedWidget>
#include <QDebug>                   // For logging
#include <QIcon>                    // For window icon
#include "models/Board.h"           // For creating board instances

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      homePageView(new HomePageView(this)),
      homePageController(new HomePageController(homePageView, this)),
      startPageView(new StartPageView(this)),
      startPageController(new StartPageController(startPageView, this)),
      settingsPageView(new SettingsPageView(this)),
      settingsPageController(new SettingsPageController(settingsPageView, this)),
      gameView(new GameView(this)), // Changed from BoardView to GameView
      currentBoardModel(nullptr) // Initialize currentBoardModel
{
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(homePageView);
    stackedWidget->addWidget(startPageView);
    stackedWidget->addWidget(settingsPageView);
    stackedWidget->addWidget(gameView); // Add gameView to stackedWidget

    setCentralWidget(stackedWidget);

    // Set window properties
    setWindowTitle("Peg Solitaire");
    setWindowIcon(QIcon(":/icon.svg"));
    
    // Set minimum window size for better usability
    setMinimumSize(800, 600);    // Connect signals for navigation
    connect(homePageController, &HomePageController::startClicked, this, &MainWindow::showStartPage);
    connect(homePageController, &HomePageController::settingsClicked, this, &MainWindow::showSettingsPage);
    connect(startPageController, &StartPageController::navigateToHome, this, &MainWindow::showHomePage);
    connect(settingsPageController, &SettingsPageController::navigateToHome, this, &MainWindow::showHomePage);
    connect(settingsPageController, &SettingsPageController::setFullscreen, this, &MainWindow::toggleFullscreen);
    connect(gameView, &GameView::navigateToHome, this, &MainWindow::showHomePage);

    // Direct connection now possible as StartPageController::startGame emits BoardType
    connect(startPageController, &StartPageController::startGame, this, &MainWindow::startGame);

    showHomePage(); // Show home page initially
    setWindowTitle("Peg Solitaire");
    resize(800, 600); // Adjusted default size for better board visibility
}

MainWindow::~MainWindow()
{
    delete currentBoardModel; // Clean up the board model
    // Other owned objects (views, controllers) are children of MainWindow or QStackedWidget
    // and should be deleted automatically by Qt's parent-child mechanism.
}

void MainWindow::showHomePage()
{
    stackedWidget->setCurrentWidget(homePageView);
}

void MainWindow::showStartPage()
{
    stackedWidget->setCurrentWidget(startPageView);
}

void MainWindow::showSettingsPage() // Add this function
{
    settingsPageView->setFullscreenChecked(windowState().testFlag(Qt::WindowFullScreen));
    stackedWidget->setCurrentWidget(settingsPageView);
}

void MainWindow::startGame(BoardType boardType)
{
    qDebug() << "Main window: Attempting to start game with board type (enum):" << static_cast<int>(boardType);

    // Clean up previous board model if it exists
    if (currentBoardModel)
    {
        delete currentBoardModel;
        currentBoardModel = nullptr;
    }
    // If you had a boardController, you would delete it here too.
    // if (boardController) { delete boardController; boardController = nullptr; }

    // Create the appropriate board model based on boardType
    switch (boardType)
    {
        case BoardType::English:
            currentBoardModel = new Board(BoardType::English, this);
            break;
        case BoardType::Diamond:
            currentBoardModel = new Board(BoardType::Diamond, this);
            break;
        case BoardType::Square:
            currentBoardModel = new Board(BoardType::Square, this);
            break;
        case BoardType::AntiPeg:
            currentBoardModel = new Board(BoardType::AntiPeg, this);
            break;
        case BoardType::Endgame:
            currentBoardModel = new Board(BoardType::Endgame, this);
            break;
        default:
            qDebug() << "Main window: Unknown or unsupported board type (enum):" << static_cast<int>(boardType);
            currentBoardModel = new Board(BoardType::English, this); // Fallback
            break;
    }

    if (currentBoardModel)
    {
        qDebug() << "Main window: Board model created with type (enum):" << static_cast<int>(currentBoardModel->getBoardType())
                 << "Pegs:" << currentBoardModel->getPegCount();
        gameView->setBoard(currentBoardModel);
        stackedWidget->setCurrentWidget(gameView); // Switch to the game view
    }
    else
    {
        qDebug() << "Main window: Failed to create board model for type:" << static_cast<int>(boardType);
        showHomePage(); // Fallback to home page if board creation fails
    }
}

void MainWindow::toggleFullscreen(bool fullscreen) // Add this function
{
    if (fullscreen)
    {
        showFullScreen();
    }
    else
    {
        showNormal();
    }
}
