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
  COthelloBoard();

  void init();

  bool canMoveAnywhere(COthelloPieceValue piece);

  bool canMove(int x, int y, COthelloPieceValue piece);
  void doMove (int x, int y, COthelloPieceValue piece);

  COthelloPieceValue getPiece(int x, int y);

  int getNumWhite();
  int getNumBlack();
  int getNum();

  int getMoves();

  int score(COthelloPieceValue piece);

  static void copy(const COthelloBoard &board1, COthelloBoard &board2);

  bool getBestMove(COthelloPieceValue piece, int depth, int *x, int *y);

 private:
  bool canMoveDirection(int x, int y, int dx, int dy, COthelloPieceValue piece,
                        COthelloPieceValue other_piece);
  void doMoveDirection (int x, int y, int dx, int dy, COthelloPieceValue piece,
                        COthelloPieceValue other_piece);

  int getNum(COthelloPieceValue piece);

  bool getBestMove1(COthelloPieceValue piece, int depth, int *x, int *y, int *score);

  bool flipCoin();

 private:
  COthelloPieceValue board_[8][8];

  static int board_score_[8][8];
};

#endif
