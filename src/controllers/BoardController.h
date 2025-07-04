#ifndef BOARDCONTROLLER_H
#define BOARDCONTROLLER_H

#include <QObject>
#include <QList>
#include <QSet>     // Added for symmetry optimization
#include <QMutex>   // Added for thread safety
#include "models/Board.h"
#include "views/BoardView.h"
#include "views/LoadingCircle.h"
#include "controllers/StrategyWorker.h"

/**
 * @brief Controller for managing the Peg Solitaire game board interactions
 * 
 * This controller handles the game logic, user interactions with the board,
 * and coordinates between the board model and view.
 */
class BoardController : public QObject
{
    Q_OBJECT

public:
    explicit BoardController(QObject *parent = nullptr);

    /**
     * @brief Set the board model for this controller
     * @param model Pointer to the board model
     */
    void setBoardModel(Board *model);    /**
     * @brief Set the board view for this controller
     * @param view Pointer to the board view
     */
    void setBoardView(BoardView *view);

    /**
     * @brief Get the current board model
     * @return Pointer to the board model
     */
    Board* getBoardModel() const;

signals:
    /**
     * @brief Emitted when the board state changes
     */
    void boardChanged();

    /**
     * @brief Emitted to highlight valid moves from a selected position
     * @param moves List of valid moves to highlight
     */
    void highlightMovesSignal(const QList<Move> &moves);

    /**
     * @brief Emitted when the number of remaining pegs changes
     * @param count Current number of pegs on the board
     */
    void pegsRemainingChanged(int count);

    /**
     * @brief Emitted when the game is over (no more valid moves)
     */
    void gameOver();    /**
     * @brief Emitted when user requests to navigate back to home
     */
    void navigateToHome();
      /**
     * @brief Emitted when the game is determined to be unwinnable
     */
    void deadGameDetected();
    
    /**
     * @brief Emitted when information about current operation needs to be displayed
     * @param message Information message to display
     */
    void informationUpdated(const QString &message);

public slots:
    /**
     * @brief Handle peg/cell click events from the board view
     * @param pos Position that was clicked
     */
    void onPegCellClicked(const Position &pos);

    /**
     * @brief Handle undo button click
     */
    void onUndoClicked();

    /**
     * @brief Handle reset button click
     */
    void onResetClicked();

    /**
     * @brief Handle home button click
     */
    void onHomeClicked();    /**
     * @brief Handle suggest move button click (spacebar)
     */
    void onSuggestMoveClicked();

    /**
     * @brief Handle WASD peg selection requests
     * @param direction Direction to search for peg (0=up/W, 1=left/A, 2=down/S, 3=right/D)
     */
    void onPegSelectionRequested(int direction);

    /**
     * @brief Handle arrow key move requests
     * @param direction Direction to execute move (0=up, 1=left, 2=down, 3=right)
     */
    void onMoveRequested(int direction);

private slots:
    /**
     * @brief Handle completion of strategy computation
     * @param move The computed move
     * @param isDeadGame Whether the game is unwinnable
     */
    void onStrategyComputed(const Move &move, bool isDeadGame);
      /**
     * @brief Handle cancellation of strategy computation
     */
    void onStrategyComputationCancelled();
    
    /**
     * @brief Handle board view resize events
     */
    void onBoardViewResized();

private:
    Board *boardModel;
    BoardView *boardView;
    
    Position selectedPosition;
    bool hasPegSelected;
    QList<Move> currentValidMoves;
    
    // Loading circle for strategy computation
    LoadingCircle *loadingCircle;
    
    // Strategy worker thread
    StrategyWorker *strategyWorker;
    bool isComputingStrategy;

    /**
     * @brief Update the view to reflect current board state
     */
    void updateView();
    
    /**
     * @brief Clear current selection and highlights
     */
    void clearSelection();
    
    /**
     * @brief Get valid moves from the selected position
     * @param fromPos Position to get moves from
     * @return List of valid moves from the position
     */
    QList<Move> getMovesFromPosition(const Position &fromPos);
    
    /**
     * @brief Check if the game is complete
     */
    void checkGameStatus();    /**
     * @brief Get a suggested move (first available move)
     * @return A suggested move, or invalid move if none available
     */
    Move getSuggestedMove();
    
    /**
     * @brief Get a strategic move that leads to winning
     * @return A strategic move, or invalid move if game is unwinnable
     */
    Move getStrategicMove();

    /**
     * @brief Find the nearest peg in the specified direction
     * @param direction Direction to search (0=up, 1=left, 2=down, 3=right)
     * @return Position of the nearest peg, or invalid position if none found
     */
    Position findNearestPeg(int direction);/**
     * @brief Find the center position of the board
     * @return Center position of the board
     */
    Position getBoardCenter();
      /**
     * @brief Find the nearest peg in a specific direction using comprehensive search
     * @param direction Direction to search (0=up, 1=left, 2=down, 3=right)
     * @return Position of the nearest peg in that direction, or invalid position if none found
     */
    Position findNearestPegInDirection(int direction);
    
    /**
     * @brief Check if the current board state is solvable (can reach winning condition)
     * @return True if the board is solvable, false otherwise
     */
    bool isBoardSolvable();
    
    /**
     * @brief Solve the board using symmetry-optimized recursive backtracking
     * @param board Pointer to board to solve
     * @return True if a solution exists from this state
     */
    bool solveBoard(Board* board);
    
    /**
     * @brief Find the best strategic move using minimax-like approach
     * @return Best move that leads to winning, or invalid move if none exists
     */
    Move findBestStrategicMove();    /**
     * @brief Clear the failed board states cache for optimization
     * This should be called when starting a new game or changing board types
     */
    static void clearFailedStatesCache();

    // Keyboard navigation state
    Position currentKeyboardPosition;
};

#endif // BOARDCONTROLLER_H