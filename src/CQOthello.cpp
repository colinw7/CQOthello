#include <CQApp.h>
#include <CQOthello.h>
#include <COthello.h>
#include <CQUtil.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include <QMouseEvent>
#include <QSpinBox>
#include <QPainter>
#include <QTimer>

#include <images/black_piece_png.h>
#include <images/white_piece_png.h>
#include <images/tile_png.h>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  CQOthello othello;

  othello.show();

  return app.exec();
}

CQOthello::
CQOthello()
{
  cell_size_ = 64; // must match png

  black_pixmap_.loadFromData(black_piece_data, BLACK_PIECE_DATA_LEN);
  white_pixmap_.loadFromData(white_piece_data, WHITE_PIECE_DATA_LEN);
  tile_pixmap_ .loadFromData(tile_data       , TILE_DATA_LEN       );

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  canvas_ = new CQOthelloCanvas(this);

  layout->addWidget(canvas_);

  QWidget *rframe = new QWidget;

  layout->addWidget(rframe);

  QVBoxLayout *rlayout = new QVBoxLayout(rframe);

  color_label_ = new QLabel;

  rlayout->addWidget(color_label_);

  QHBoxLayout *white_layout = new QHBoxLayout;

  QLabel *white_label = new QLabel("White");

  QToolButton *white_button = new QToolButton();

  white_button->setText("Human");

  white_layout->addWidget(white_label);
  white_layout->addWidget(white_button);

  rlayout->addLayout(white_layout);

  QHBoxLayout *black_layout = new QHBoxLayout;

  QLabel *black_label = new QLabel("Black");

  QToolButton *black_button = new QToolButton();

  black_button->setText("Computer");

  black_layout->addWidget(black_label);
  black_layout->addWidget(black_button);

  rlayout->addLayout(black_layout);

  rlayout->addStretch();

  QHBoxLayout *level_layout = new QHBoxLayout;

  QLabel *level_label = new QLabel("Level");

  QSpinBox *level_text = new QSpinBox;

  connect(level_text, SIGNAL(valueChanged(int)), this, SLOT(levelSlot(int)));

  level_text->setRange(1, 10);

  level_text->setValue(3);

  level_layout->addWidget(level_label);
  level_layout->addWidget(level_text);

  rlayout->addLayout(level_layout);

  score_label_ = new QLabel;

  rlayout->addWidget(score_label_);

  rlayout->addStretch();

  QPushButton *newGame = new QPushButton("New Game");

  connect(newGame, SIGNAL(clicked()), this, SLOT(newGameSlot()));

  rlayout->addWidget(newGame);

  newGameSlot();

  timer_ = new QTimer;

  connect(timer_, SIGNAL(timeout()), this, SLOT(computerMoveSlot()));

  timer_->start(500);
}

void
CQOthello::
draw(QPainter *painter)
{
  for (uint y = 0; y < 8; ++y) {
    for (uint x = 0; x < 8; ++x) {
      COthelloPieceValue piece = getBoardPiece(x, y);

      QPoint p(x*cell_size_, y*cell_size_);

      painter->drawPixmap(p, tile_pixmap_);

      QPoint p1(x*cell_size_ + 2, y*cell_size_ + 2);

      if      (piece == COTHELLO_PIECE_WHITE)
        painter->drawPixmap(p1, white_pixmap_);
      else if (piece == COTHELLO_PIECE_BLACK)
        painter->drawPixmap(p1, black_pixmap_);
    }
  }

  color_label_->setText(QString("%1 to Move").
    arg(current_player_ == COTHELLO_PIECE_WHITE ? "White" : "Black"));

  score_label_->setText(QString("White %1 Black %2").
    arg(getNumWhite()).arg(getNumBlack()));
}

void
CQOthello::
select(int x, int y)
{
  if (game_over_) return;

  if (players_[current_player_] != HUMAN_PLAYER) return;

  if (canMove(x, y, current_player_)) {
    doMove(x, y, current_player_);

    current_player_ = COthelloPiece::otherPiece(current_player_);

    if (! canMoveAnywhere(current_player_)) {
      current_player_ = COthelloPiece::otherPiece(current_player_);

      if (! canMoveAnywhere(current_player_))
        game_over_ = true;
    }

    canvas_->setChanged();
  }
}

void
CQOthello::
levelSlot(int level)
{
  level_ = level;
}

void
CQOthello::
newGameSlot()
{
  board_.init();

  players_[COTHELLO_PIECE_WHITE] = HUMAN_PLAYER;
  players_[COTHELLO_PIECE_BLACK] = COMPUTER_PLAYER;

  current_player_ = COTHELLO_PIECE_WHITE;

  level_ = 3;

  game_over_ = false;

  canvas_->setChanged();
}

void
CQOthello::
computerMoveSlot()
{
  if (game_over_) return;

  if (players_[current_player_] != COMPUTER_PLAYER) return;

  int x, y;

  if (getBestMove(current_player_, level_, &x, &y)) {
    doMove(x, y, current_player_);

    current_player_ = COthelloPiece::otherPiece(current_player_);

    if (! canMoveAnywhere(current_player_)) {
      current_player_ = COthelloPiece::otherPiece(current_player_);

      if (! canMoveAnywhere(current_player_))
        game_over_ = true;
    }

    canvas_->setChanged();
  }

  timer_->start(500);
}

COthelloPieceValue
CQOthello::
getBoardPiece(int x, int y)
{
  return board_.getPiece(x, y);
}

bool
CQOthello::
canMoveAnywhere(COthelloPieceValue piece)
{
  return board_.canMoveAnywhere(piece);
}

bool
CQOthello::
canMove(int x, int y, COthelloPieceValue piece)
{
  return board_.canMove(x, y, piece);
}

void
CQOthello::
doMove(int x, int y, COthelloPieceValue piece)
{
  board_.doMove(x, y, piece);
}

bool
CQOthello::
isWhitePiece(int x, int y)
{
  return (board_.getPiece(x, y) == COTHELLO_PIECE_WHITE);
}

bool
CQOthello::
isBlackPiece(int x, int y)
{
  return (board_.getPiece(x, y) == COTHELLO_PIECE_BLACK);
}

int
CQOthello::
getNumWhite()
{
  return board_.getNumWhite();
}

int
CQOthello::
getNumBlack()
{
  return board_.getNumBlack();
}

int
CQOthello::
getNum()
{
  return board_.getNum();
}

bool
CQOthello::
getBestMove(COthelloPieceValue piece, int depth, int *x, int *y)
{
  return  board_.getBestMove(piece, depth, x, y);
}

COthelloPieceValue stringToPiece(const char *str)
{
  if (strcmp(str, "White") == 0)
    return COTHELLO_PIECE_WHITE;
  else
    return COTHELLO_PIECE_BLACK;
}

//------

CQOthelloCanvas::
CQOthelloCanvas(CQOthello *othello) :
 CQCanvas2D(NULL), othello_(othello)
{
  int size = 8*othello_->getCellSize();

  setFixedSize(size + 1, size + 1);
}

void
CQOthelloCanvas::
drawContents(QPainter *p)
{
  othello_->draw(p);
}

void
CQOthelloCanvas::
mousePressEvent(QMouseEvent *e)
{
  QPoint pos = e->pos();

  int x = pos.x()/othello_->getCellSize();
  int y = pos.y()/othello_->getCellSize();

  othello_->select(x, y);
}
