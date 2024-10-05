#ifndef COthello_H
#define COthello_H

enum COthelloPieceValue {
  COTHELLO_PIECE_BORDER,
  COTHELLO_PIECE_NONE,
  COTHELLO_PIECE_WHITE,
  COTHELLO_PIECE_BLACK
};

class COthelloPiece {
 public:
  static COthelloPieceValue otherPiece(COthelloPieceValue piece);
};

class COthelloBoard {
 public:
  using PieceValue = COthelloPieceValue;

 public:
  COthelloBoard();

  void init();

  bool canMoveAnywhere(PieceValue piece);

  bool canMove(int x, int y, PieceValue piece);
  void doMove (int x, int y, PieceValue piece);

  PieceValue getPiece(int x, int y);

  int getNumWhite();
  int getNumBlack();
  int getNum();

  int getMoves();

  int score(PieceValue piece);

  static void copy(const COthelloBoard &board1, COthelloBoard &board2);

  bool getBestMove(PieceValue piece, int depth, int *x, int *y);

 private:
  bool canMoveDirection(int x, int y, int dx, int dy, PieceValue piece, PieceValue other_piece);
  void doMoveDirection (int x, int y, int dx, int dy, PieceValue piece, PieceValue other_piece);

  int getNum(PieceValue piece);

  bool getBestMove1(PieceValue piece, int depth, int *x, int *y, int *score);

  bool flipCoin();

 private:
  static int board_score_[8][8];

  PieceValue board_[8][8];
};

#endif
