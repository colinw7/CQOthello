#include <QLabel>
#include <QPixmap>

#include <COthello.h>
#include <CImageLib.h>
#include <CQCanvas2D.h>

class CQOthello;

class QPainter;

class CQOthelloCanvas : public CQCanvas2D {
 private:
  CQOthello *othello_;

 public:
  CQOthelloCanvas(CQOthello *othello);

 private:
  void drawContents(QPainter *);

  void mousePressEvent(QMouseEvent *);
};

class COthelloBoard;
class QTimer;

class CQOthello : public QWidget {
  Q_OBJECT

 private:
  enum Player {
    HUMAN_PLAYER,
    COMPUTER_PLAYER
  };

  typedef std::map<COthelloPieceValue,Player> PlayerMap;

  COthelloBoard       board_;
  uint                cell_size_;
  CQOthelloCanvas    *canvas_;
  QLabel             *color_label_;
  QLabel             *score_label_;
  QPixmap             white_pixmap_;
  QPixmap             black_pixmap_;
  QPixmap             tile_pixmap_;
  PlayerMap           players_;
  COthelloPieceValue  current_player_;
  int                 level_;
  bool                game_over_;
  QTimer             *timer_;

 public:
  CQOthello();

  uint getCellSize() const { return cell_size_; }

  void draw(QPainter *painter);

  void select(int x, int y);

  COthelloPieceValue getBoardPiece(int x, int y);

  bool canMoveAnywhere(COthelloPieceValue piece);

  bool canMove(int x, int y, COthelloPieceValue piece);

  void doMove(int x, int y, COthelloPieceValue piece);

  bool isWhitePiece(int x, int y);
  bool isBlackPiece(int x, int y);

  int getNumWhite();
  int getNumBlack();

  int getNum();

  bool getBestMove(COthelloPieceValue piece, int depth, int *x, int *y);

  COthelloPieceValue stringToPiece(const char *str);

 private slots:
  void levelSlot(int);
  void newGameSlot();

  void computerMoveSlot();
};
