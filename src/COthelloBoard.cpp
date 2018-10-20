#include <COthelloBoard.h>
#include <COSRand.h>

int COthelloBoard::board_score_[8][8] = {
  { 128,   1,  64,   8,   8,  64,   1, 128, },
  {   1,   0,   4,   2,   2,   4,   0,   1, },
  {  64,   4,  32,  16,  16,  32,   4,  64, },
  {   8,   2,  16,   0,   0,  16,   2,   8, },
  {   8,   2,  16,   0,   0,  16,   2,   8, },
  {  64,   4,  32,  16,  16,  32,   4,  64, },
  {   1,   0,   4,   2,   2,   4,   0,   1, },
  { 128,   1,  64,   8,   8,  64,   1, 128, },
};

COthelloBoard::
COthelloBoard()
{
  init();
}

void
COthelloBoard::
init()
{
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      board_[x][y] = COTHELLO_PIECE_NONE;

  board_[3][3] = COTHELLO_PIECE_WHITE;
  board_[4][3] = COTHELLO_PIECE_BLACK;
  board_[3][4] = COTHELLO_PIECE_BLACK;
  board_[4][4] = COTHELLO_PIECE_WHITE;
}

void
COthelloBoard::
copy(const COthelloBoard &board1, COthelloBoard &board2)
{
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      board2.board_[x][y] = board1.board_[x][y];
}

bool
COthelloBoard::
canMoveAnywhere(COthelloPieceValue piece)
{
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      if (canMove(x, y, piece))
        return true;

  return false;
}

bool
COthelloBoard::
canMove(int x, int y, COthelloPieceValue piece)
{
  if (board_[x][y] != COTHELLO_PIECE_NONE)
    return false;

  COthelloPieceValue other_piece = COthelloPiece::otherPiece(piece);

  if (canMoveDirection(x, y,  1,  0, piece, other_piece)) return true;
  if (canMoveDirection(x, y,  1,  1, piece, other_piece)) return true;
  if (canMoveDirection(x, y,  0,  1, piece, other_piece)) return true;
  if (canMoveDirection(x, y, -1,  1, piece, other_piece)) return true;
  if (canMoveDirection(x, y, -1,  0, piece, other_piece)) return true;
  if (canMoveDirection(x, y, -1, -1, piece, other_piece)) return true;
  if (canMoveDirection(x, y,  0, -1, piece, other_piece)) return true;
  if (canMoveDirection(x, y,  1, -1, piece, other_piece)) return true;

  return 0;
}

bool
COthelloBoard::
canMoveDirection(int x, int y, int dx, int dy,
                 COthelloPieceValue piece,
                 COthelloPieceValue other_piece) {
  int x1 = x + dx;
  int y1 = y + dy;

  COthelloPieceValue value = getPiece(x1, y1);

  if (value != other_piece)
    return false;

  x1 += dx;
  y1 += dy;

  value = getPiece(x1, y1);

  while (value == other_piece) {
    x1 += dx;
    y1 += dy;

    value = getPiece(x1, y1);
  }

  value = getPiece(x1, y1);

  if (value != piece)
    return false;

  return true;
}

void
COthelloBoard::
doMove(int x, int y, COthelloPieceValue piece)
{
  board_[x][y] = piece;

  COthelloPieceValue other_piece = COthelloPiece::otherPiece(piece);

  if (canMoveDirection(x, y,  1,  0, piece, other_piece))
    doMoveDirection(x, y,  1,  0, piece, other_piece);
  if (canMoveDirection(x, y,  1,  1, piece, other_piece))
    doMoveDirection(x, y,  1,  1, piece, other_piece);
  if (canMoveDirection(x, y,  0,  1, piece, other_piece))
    doMoveDirection(x, y,  0,  1, piece, other_piece);
  if (canMoveDirection(x, y, -1,  1, piece, other_piece))
    doMoveDirection(x, y, -1,  1, piece, other_piece);
  if (canMoveDirection(x, y, -1,  0, piece, other_piece))
    doMoveDirection(x, y, -1,  0, piece, other_piece);
  if (canMoveDirection(x, y, -1, -1, piece, other_piece))
    doMoveDirection(x, y, -1, -1, piece, other_piece);
  if (canMoveDirection(x, y,  0, -1, piece, other_piece))
    doMoveDirection(x, y,  0, -1, piece, other_piece);
  if (canMoveDirection(x, y,  1, -1, piece, other_piece))
    doMoveDirection(x, y,  1, -1, piece, other_piece);
}

void
COthelloBoard::
doMoveDirection(int x, int y, int dx, int dy,
                COthelloPieceValue piece,
                COthelloPieceValue other_piece) {
  int x1 = x + dx;
  int y1 = y + dy;

  COthelloPieceValue value = getPiece(x1, y1);

  if (value != other_piece)
    return;

  board_[x1][y1] = piece;

  x1 += dx;
  y1 += dy;

  value = getPiece(x1, y1);

  while (value == other_piece) {
    board_[x1][y1] = piece;

    x1 += dx;
    y1 += dy;

    value = getPiece(x1, y1);
  }
}

COthelloPieceValue
COthelloBoard::
getPiece(int x, int y)
{
  if (x < 0 || x >= 8 || y < 0 || y >= 8)
    return COTHELLO_PIECE_BORDER;

  return board_[x][y];
}

int
COthelloBoard::
getNumWhite()
{
  return getNum(COTHELLO_PIECE_WHITE);
}

int
COthelloBoard::
getNumBlack()
{
  return getNum(COTHELLO_PIECE_BLACK);
}

int
COthelloBoard::
getNum()
{
  return getNumWhite() + getNumBlack();
}

int
COthelloBoard::
getNum(COthelloPieceValue piece)
{
  int num = 0;

  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      if (board_[x][y] == piece)
        num++;

  return num;
}

int
COthelloBoard::
getMoves()
{
  return (64 - getNum(COTHELLO_PIECE_NONE));
}

int
COthelloBoard::
score(COthelloPieceValue piece)
{
  COthelloPieceValue other_piece = COthelloPiece::otherPiece(piece);

  int score = 0;

  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      if      (board_[x][y] == piece)
        score += board_score_[x][y];
      else if (board_[x][y] == other_piece)
        score -= board_score_[x][y];

  return score;
}

bool
COthelloBoard::
getBestMove(COthelloPieceValue piece, int depth, int *x, int *y)
{
  int score;

  return getBestMove1(piece, depth, x, y, &score);
}

bool
COthelloBoard::
getBestMove1(COthelloPieceValue piece, int depth, int *x, int *y, int *score)
{
  COthelloBoard board1;

  if (depth <= 0) {
    *score = 0;

    return false;
  }

  bool move = false;

  *score = -9999;

  COthelloBoard::copy(*this, board1);

  int board_score = this->score(piece);

  for (int x1 = 0; x1 < 8; x1++) {
    for (int y1 = 0; y1 < 8; y1++) {
      if (! board1.canMove(x1, y1, piece))
        continue;

      board1.doMove(x1, y1, piece);

      int score1 = board1.score(piece) - board_score;

      int x2, y2;
      int score2;

      bool move2 = board1.getBestMove1(COthelloPiece::otherPiece(piece),
                                       depth - 1, &x2, &y2, &score2);

      if (move2)
        score1 -= score2;

      if (! move || score1 > *score ||
          (score1 == *score && COSRand::randBool())) {
        move = true;

        *x = x1;
        *y = y1;

        *score = score1;
      }

      COthelloBoard::copy(*this, board1);
    }
  }

  return move;
}
