#include "std_c++.h"
#include "CTclApp/CTclApp.h"
#include "CTclApp/CTclAppCommand.h"
#include "COthelloLib/COthello.h"

#include "CTclOthello.k"

class CTclOthello : public CTclApp {
 private:
  COthelloBoard board_;

 public:
  CTclOthello(int argc, char **argv) :
   CTclApp(argc, argv) {
    init();
  }

  string getTclStr() { return CTclOthello_TK; }

  void initBoard() {
    board_.init();
  }

  COthelloPieceValue getBoardPiece(int x, int y) {
    return board_.getPiece(x, y);
  }

  int canMoveAnywhere(COthelloPieceValue piece) {
    return board_.canMoveAnywhere(piece);
  }

  int canMove(int x, int y, COthelloPieceValue piece) {
    return board_.canMove(x, y, piece);
  }

  void doMove(int x, int y, COthelloPieceValue piece) {
    board_.doMove(x, y, piece);
  }

  int isWhitePiece(int x, int y) {
    return (board_.getPiece(x, y) == COTHELLO_PIECE_WHITE);
  }

  int isBlackPiece(int x, int y) {
    return (board_.getPiece(x, y) == COTHELLO_PIECE_BLACK);
  }

  int getNumWhite() {
    return board_.getNumWhite();
  }

  int getNumBlack() {
    return board_.getNumBlack();
  }

  int getNum() {
    return board_.getNum();
  }

  bool getBestMove(COthelloPieceValue piece, int depth, int *x, int *y) {
    return  board_.getBestMove(piece, depth, x, y);
  }

  COthelloPieceValue stringToPiece(const char *str) {
    if (strcmp(str, "White") == 0)
      return COTHELLO_PIECE_WHITE;
    else
      return COTHELLO_PIECE_BLACK;
  }

 protected:
  void addCommands();
};

#define CTclOthelloAppCommand(NAME) \
class NAME : public CTclAppCommand { \
 private: \
  CTclOthello *othello_; \
\
 public: \
  NAME(CTclOthello *othello) : \
    CTclAppCommand(othello, #NAME), othello_(othello) { \
  } \
\
 protected: \
  bool proc(int argc, const char **argv); \
};

CTclOthelloAppCommand(CTclOthelloInitBoard)
CTclOthelloAppCommand(CTclOthelloGetBoardPiece)
CTclOthelloAppCommand(CTclOthelloCanMoveAnywhere)
CTclOthelloAppCommand(CTclOthelloCanMove)
CTclOthelloAppCommand(CTclOthelloDoMove)
CTclOthelloAppCommand(CTclOthelloIsWhitePiece)
CTclOthelloAppCommand(CTclOthelloIsBlackPiece)
CTclOthelloAppCommand(CTclOthelloGetNumWhite)
CTclOthelloAppCommand(CTclOthelloGetNumBlack)
CTclOthelloAppCommand(CTclOthelloGetNum)
CTclOthelloAppCommand(CTclOthelloGetBestMove)

int
main(int argc, char **argv)
{
  CTclOthello app(argc, argv);
}

void
CTclOthello::
addCommands()
{
  new CTclOthelloInitBoard(this);
  new CTclOthelloGetBoardPiece(this);
  new CTclOthelloCanMoveAnywhere(this);
  new CTclOthelloCanMove(this);
  new CTclOthelloDoMove(this);
  new CTclOthelloIsWhitePiece(this);
  new CTclOthelloIsBlackPiece(this);
  new CTclOthelloGetNumWhite(this);
  new CTclOthelloGetNumBlack(this);
  new CTclOthelloGetNum(this);
  new CTclOthelloGetBestMove(this);
}

bool
CTclOthelloInitBoard::
proc(int, const char **)
{
  othello_->initBoard();

  return true;
}

bool
CTclOthelloGetBoardPiece::
proc(int argc, const char **argv)
{
  if (argc != 3)
    return false;

  int x = atoi(argv[1]);
  int y = atoi(argv[2]);

  COthelloPieceValue piece = othello_->getBoardPiece(x, y);

  setIntegerResult(piece);

  return true;
}

bool
CTclOthelloCanMoveAnywhere::
proc(int argc, const char **argv)
{
  if (argc != 2)
    return false;

  COthelloPieceValue piece = othello_->stringToPiece(argv[1]);

  if (othello_->canMoveAnywhere(piece))
    setIntegerResult(1);
  else
    setIntegerResult(0);

  return true;
}

bool
CTclOthelloCanMove::
proc(int argc, const char **argv)
{
  if (argc != 4)
    return false;

  int x = atoi(argv[1]);
  int y = atoi(argv[2]);

  COthelloPieceValue piece = othello_->stringToPiece(argv[3]);

  if (othello_->canMove(x, y, piece))
    setIntegerResult(1);
  else
    setIntegerResult(0);

  return true;
}

bool
CTclOthelloDoMove::
proc(int argc, const char **argv)
{
  if (argc != 4)
    return false;

  int x = atoi(argv[1]);
  int y = atoi(argv[2]);

  COthelloPieceValue piece = othello_->stringToPiece(argv[3]);

  othello_->doMove(x, y, piece);

  return true;
}

bool
CTclOthelloIsWhitePiece::
proc(int argc, const char **argv)
{
  if (argc != 3)
    return false;

  int x = atoi(argv[1]);
  int y = atoi(argv[2]);

  if (othello_->isWhitePiece(x, y))
    setIntegerResult(1);
  else
    setIntegerResult(0);

  return true;
}

bool
CTclOthelloIsBlackPiece::
proc(int argc, const char **argv)
{
  if (argc != 3)
    return false;

  int x = atoi(argv[1]);
  int y = atoi(argv[2]);

  if (othello_->isBlackPiece(x, y))
    setIntegerResult(1);
  else
    setIntegerResult(0);

  return true;
}

bool
CTclOthelloGetNumWhite::
proc(int, const char **)
{
  int num = othello_->getNumWhite();

  setIntegerResult(num);

  return true;
}

bool
CTclOthelloGetNumBlack::
proc(int, const char **)
{
  int num = othello_->getNumBlack();

  setIntegerResult(num);

  return true;
}

bool
CTclOthelloGetNum::
proc(int, const char **)
{
  int num = othello_->getNum();

  setIntegerResult(num);

  return true;
}

bool
CTclOthelloGetBestMove::
proc(int argc, const char **argv)
{
  if (argc != 5)
    return false;

  COthelloPieceValue piece = othello_->stringToPiece(argv[1]);

  int depth = atoi(argv[2]);

  const char *x_var = argv[3];
  const char *y_var = argv[4];

  int x, y;

  if (othello_->getBestMove(piece, depth, &x, &y)) {
    setIntegerVar(x_var, x);
    setIntegerVar(y_var, y);

    setIntegerResult(1);
  }
  else
    setIntegerResult(0);

  return true;
}
