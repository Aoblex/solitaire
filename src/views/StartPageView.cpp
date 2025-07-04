#include "StartPageView.h"
#include "models/Board.h" // Include BoardType definition
#include "utils/ButtonStyles.h"
#include <QFont>
#include <QHBoxLayout> // Ensure this is included

StartPageView::StartPageView(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20); // Overall spacing
    mainLayout->setAlignment(Qt::AlignCenter);

    // Title Label
    titleLabel = new QLabel("Select Game Mode", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24); // Increased title font size
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Columns Layout
    columnsLayout = new QHBoxLayout();
    columnsLayout->setSpacing(30); // Spacing between columns

    // --- Column 1: Classic Mode ---
    classicModeLayout = new QVBoxLayout();
    classicModeLabel = new QLabel("Classic Mode", this);
    classicModeLabel->setFont(titleFont); // Reuse title font for section headers
    classicModeLayout->addWidget(classicModeLabel, 0, Qt::AlignHCenter);
    EnglishButton = new QPushButton("English Board", this);
    DiamondButton = new QPushButton("Diamond Board", this);
    SquareButton = new QPushButton("Square Board", this);

    // Apply game mode button styles
    ButtonStyles::applyGameModeStyle(EnglishButton);
    ButtonStyles::applyGameModeStyle(DiamondButton);
    ButtonStyles::applyGameModeStyle(SquareButton);
    classicModeLayout->addWidget(EnglishButton, 0, Qt::AlignHCenter);
    classicModeLayout->addWidget(DiamondButton, 0, Qt::AlignHCenter);
    classicModeLayout->addWidget(SquareButton, 0, Qt::AlignHCenter);
    classicModeLayout->addStretch(); // Pushes buttons to the top
    columnsLayout->addLayout(classicModeLayout);

    // --- Column 2: Special Game Mode ---
    specialModeLayout = new QVBoxLayout();
    specialModeLabel = new QLabel("Special Modes", this);
    specialModeLabel->setFont(titleFont);
    specialModeLayout->addWidget(specialModeLabel, 0, Qt::AlignHCenter);
    specialAntiPegButton = new QPushButton("Anti-Peg Mode", this);
    specialEndgameButton = new QPushButton("Endgame Mode", this);
    
    // Apply game mode button styles
    ButtonStyles::applyGameModeStyle(specialAntiPegButton);
    ButtonStyles::applyGameModeStyle(specialEndgameButton);

    specialModeLayout->addWidget(specialAntiPegButton, 0, Qt::AlignHCenter);
    specialModeLayout->addWidget(specialEndgameButton, 0, Qt::AlignHCenter);
    specialModeLayout->addStretch();
    columnsLayout->addLayout(specialModeLayout);

    mainLayout->addLayout(columnsLayout);    // Back Button
    backButton = new QPushButton("Back to Home", this);
    ButtonStyles::applySecondaryStyle(backButton);
    mainLayout->addWidget(backButton, 0, Qt::AlignHCenter); // Center back button

    setLayout(mainLayout);

    // Connect signals
    connect(backButton, &QPushButton::clicked, this, &StartPageView::backClicked);

    // Classic Mode Buttons
    connect(EnglishButton, &QPushButton::clicked, this, [this]()
            { emit gameModeSelected(BoardType::English); });
    connect(DiamondButton, &QPushButton::clicked, this, [this]()
            { emit gameModeSelected(BoardType::Diamond); });
    connect(SquareButton, &QPushButton::clicked, this, [this]()
            { emit gameModeSelected(BoardType::Square); });

    // Special Mode Buttons
    connect(specialAntiPegButton, &QPushButton::clicked, this, [this]()
            { emit gameModeSelected(BoardType::AntiPeg); });
    connect(specialEndgameButton, &QPushButton::clicked, this, [this]()
            { emit gameModeSelected(BoardType::Endgame); });

    // Peg Duo Mode Button
    connect(pegDuoButton, &QPushButton::clicked, this, [this]()
            { emit gameModeSelected(BoardType::English); });
}

StartPageView::~StartPageView()
{
    // Qt's parent-child mechanism should handle deletion of layouts and widgets.
}
