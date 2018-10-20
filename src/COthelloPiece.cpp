#include <COthelloPiece.h>

COthelloPieceValue
COthelloPiece::
otherPiece(COthelloPieceValue piece)
{
  if      (piece == COTHELLO_PIECE_WHITE)
    return COTHELLO_PIECE_BLACK;
  else if (piece == COTHELLO_PIECE_BLACK)
    return COTHELLO_PIECE_WHITE;
  else
    return COTHELLO_PIECE_NONE;
}
