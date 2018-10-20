#include <std_c++.h>
#include <CGLApp/CGLApp.h>
#include <CGLTexture/CGLTexture.h>
#include "CGLOthello.h"
#include <COthelloLib/COthello.h>
#include <CGLRenderer3D/CGLRenderer3D.h>
#include <CGLWidget/CGLLabel.h>
#include <CGLWidget/CGLIntSpinBox.h>
#include <CGLWidget/CGLPushButton.h>

#include "blue_marble_png.h"
#include "white_marble_png.h"
#include "images/tile.xpm"

enum { DEFAULT_LEVEL = 3 };

int
main(int argc, char **argv)
{
  CGLAppInst->init(argc, argv);

  CGLOthello *othello = new CGLOthello(0, 0, 800, 800);

  othello->setup();

  CGLAppInst->timerLoop(100);
}

CGLOthello::
CGLOthello(int x, int y, uint w, uint h) :
 overlay_(false), level_(DEFAULT_LEVEL), gameOver_(false)
{
  init(x, y, w, h);
}

void
CGLOthello::
setup()
{
  currentPlayer_ = COTHELLO_PIECE_WHITE;

  players_[COTHELLO_PIECE_WHITE] = HUMAN_PLAYER;
  players_[COTHELLO_PIECE_BLACK] = COMPUTER_PLAYER;

  CImageDataSrc black_src(blue_marble_data , BLUE_MARBLE_DATA_LEN);
  CImageDataSrc white_src(white_marble_data, WHITE_MARBLE_DATA_LEN);
  CImageXPMSrc  empty_src(tile_data        , sizeof(tile_data     ));

  black_image_ = CImageMgrInst->createImage(black_src);
  white_image_ = CImageMgrInst->lookupImage(white_src);
  empty_image_ = CImageMgrInst->lookupImage(empty_src);

  black_texture_ = new CGLTexture;
  white_texture_ = new CGLTexture;
  board_texture_ = new CGLTexture;

  black_texture_->setImage(black_image_);
  white_texture_->setImage(white_image_);
  board_texture_->setImage(empty_image_);

  CGLLight *light1 = createLight();

  light1->setAmbient (CRGBA(0.1, 0.1, 0.1));
  light1->setDiffuse (CRGBA(0.7, 0.7, 0.7));
  light1->setSpecular(CRGBA(1.0, 1.0, 1.0));

  lightPos_ = CPoint3D(0.5, 0.5, 2.0);

  light1->setPosition(lightPos_);

  light1->setEnabled(true);

  piece_material_.setSpecular (CRGBA(1.0, 1.0, 1.0));
  piece_material_.setShininess(128);

  board_material_.setSpecular (CRGBA(0.0, 0.0, 0.0));

  renderer_ = new CGLRenderer3D(this);

  addControl();

  enableDispAttr();

  bmin_ = -0.9;
  bmax_ =  0.9;

  //--------

  int border = 10;

  CFontPtr controlFont =
    CFontMgrInst->lookupFont("sans-serif", CFONT_STYLE_NORMAL, 28);

  CFontPtr infoFont =
    CFontMgrInst->lookupFont("sans-serif", CFONT_STYLE_NORMAL, 14);

  frame_ = new CGLWidget(this, "infoFrame");

  frame_->setGeometry(CIBBox2D(0, 0, 400, 30));

  frame_->setFont(controlFont);

  //--------

  scoreLabel_ = new CGLLabel(frame_, "score");

  scoreLabel_->setLabel("");

  scoreLabel_->setForeground(CRGBA(1,1,1));

  scoreLabel_->setFont(infoFont);

  int x1 = border;
  int y1 = border;
  int w1 = 200 - 2*border;
  int h1 =  30 - 2*border;

  scoreLabel_->setGeometry(CIBBox2D(CIPoint2D(x1, y1), CISize2D(w1, h1)));

  moveLabel_ = new CGLLabel(frame_, "move");

  moveLabel_->setLabel("");

  moveLabel_->setForeground(CRGBA(1,1,1));

  moveLabel_->setFont(infoFont);

  int x2 = 200 + border;
  int y2 = border;
  int w2 = 200 - 2*border;
  int h2 =  30 - 2*border;

  moveLabel_->setGeometry(CIBBox2D(CIPoint2D(x2, y2), CISize2D(w2, h2)));

  //--------

  levelButton_ = new CGLIntSpinBox(frame_, "Level");

  levelButton_->setValue(level_);

  levelButton_->setMinValue(1);
  levelButton_->setMaxValue(10);

  int x3 = (frame_->getWidth() - w2)/2;
  int y3 = 100;

  levelButton_->setGeometry(CIBBox2D(CIPoint2D(x3, y3), CISize2D(w2, 40)));

  levelButton_->connectValueChanged(
    boost::bind(&CGLOthello::levelChanged, this));

  CImagePtr image = CImageMgrInst->lookupImage("button.png");

  newGameButton_ = new CGLPushButton(frame_, "New Game");

  newGameButton_->setImage(image);

  int x4 = (frame_->getWidth() - image->getWidth())/2;
  int y4 = 200;

  newGameButton_->setGeometry(CIBBox2D(CIPoint2D(x4, y4), image->getSize()));

  newGameButton_->connectPressed(
    boost::bind(&CGLOthello::newGamePressed, this));
}

void
CGLOthello::
drawScene()
{
  renderer_->clear(CRGBA(0,0,0));

  renderer_->setForeground(CRGBA(1,1,1));

  drawBoard();

  drawShadows();

  renderer_->setForeground(CRGBA(1.0, 1.0, 1.0));

  drawPieces(true);
}

void
CGLOthello::
drawBoard()
{
  board_material_.setup();

  renderer_->setForeground(CRGBA(1.0, 1.0, 1.0));

  double s = (bmax_ - bmin_)/8;

  for (uint y = 0; y < 8; ++y) {
    double y1 = y*s + bmin_;

    for (uint x = 0; x < 8; ++x) {
      double x1 = x*s + bmin_;

      glLoadName(8*y + x + 1);

      renderer_->fillXYRectangle(x1, y1, x1 + s, y1 + s, 0, board_texture_);
    }
  }
}

void
CGLOthello::
drawShadows()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  glPushMatrix();

  //double viewMatrix[16];

  //glGetDoublev(GL_MODELVIEW_MATRIX, viewMatrix);

  //glLoadIdentity();

  //double viewMatrix1[16];

  //glGetDoublev(GL_MODELVIEW_MATRIX, viewMatrix1);

  CGLAppInst->shadowProjection(lightPos_, CPoint3D(0, 0, 0),
                               CVector3D(0, 0, 1));

  //glGetDoublev(GL_MODELVIEW_MATRIX, viewMatrix1);

  //glMultMatrixd(viewMatrix);

  //glGetDoublev(GL_MODELVIEW_MATRIX, viewMatrix1);

  renderer_->setForeground(CRGBA(0.0, 0.0, 0.0, 0.2));

  drawPieces(false);

  glPopMatrix();

  // draw Light
  renderer_->setForeground(CRGBA(1.0, 1.0, 0.0));

  renderer_->drawPoint(lightPos_);

  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
}

void
CGLOthello::
drawPieces(bool texture)
{
  piece_material_.setup();

  double s = (bmax_ - bmin_)/8;

  for (uint y = 0; y < 8; ++y) {
    for (uint x = 0; x < 8; ++x) {
      COthelloPieceValue piece = getBoardPiece(x, y);

      CPoint2D p(x*s + bmin_, y*s + bmin_);

      if      (piece == COTHELLO_PIECE_WHITE)
        drawWhitePiece(p, texture);
      else if (piece == COTHELLO_PIECE_BLACK)
        drawBlackPiece(p, texture);
    }
  }
}

void
CGLOthello::
drawWhitePiece(const CPoint2D &p, bool texture)
{
  double s = (bmax_ - bmin_)/8;

  CPoint3D c(p.x + s/2, p.y + s/2, s/2);

  if (texture)
    renderer_->drawSphere(c, s/2, white_texture_);
  else
    renderer_->drawSphere(c, s/2, NULL);
}

void
CGLOthello::
drawBlackPiece(const CPoint2D &p, bool texture)
{
  double s = (bmax_ - bmin_)/8;

  CPoint3D c(p.x + s/2, p.y + s/2, s/2);

  if (texture)
    renderer_->drawSphere(c, s/2, black_texture_);
  else
    renderer_->drawSphere(c, s/2, NULL);
}

void
CGLOthello::
drawOverlay()
{
  int w = getWidth ();
  int h = getHeight();

  int fw = frame_->getWidth ();

  scoreLabel_   ->setVisible(! overlay_);
  moveLabel_    ->setVisible(! overlay_);
  levelButton_  ->setVisible(  overlay_);
  newGameButton_->setVisible(  overlay_);

  frame_->setAutoFill(overlay_);

  if (! overlay_) {
    int fh = 30;

    frame_->setGeometry(CIBBox2D(CIPoint2D((w - fw)/2, h - fh - 4),
                                 CISize2D (fw, fh)));

    std::ostringstream str1, str2;

    str1 << "White " << getNumWhite() << " Black " << getNumBlack();

    scoreLabel_->setLabel(str1.str());

    if (! gameOver_)
      str2 << (COTHELLO_PIECE_WHITE ? "White" : "Black") << " to Move";
    else {
      string winner;

      if      (getNumWhite() > getNumBlack())
        winner = "White Wins";
      else if (getNumWhite() < getNumBlack())
        winner = "Black Wins";
      else
        winner = "Draw";

      str2 << "Game Over : " << winner;
    }

    moveLabel_->setLabel(str2.str());
  }
  else {
    int fh = 400;

    frame_->setGeometry(CIBBox2D(CIPoint2D((w - fw)/2, (h - fh)/2),
                                 CISize2D (fw, fh)));
  }

  drawWidgets();
}

void
CGLOthello::
select(int x, int y)
{
  if (gameOver_) return;

  if (players_[currentPlayer_] != HUMAN_PLAYER) return;

  if (canMove(x, y, currentPlayer_)) {
    doMove(x, y, currentPlayer_);

    currentPlayer_ = COthelloPiece::otherPiece(currentPlayer_);

    if (! canMoveAnywhere(currentPlayer_)) {
      currentPlayer_ = COthelloPiece::otherPiece(currentPlayer_);

      if (! canMoveAnywhere(currentPlayer_))
        gameOver_ = true;
    }

    renderer_->flush();
  }
}

void
CGLOthello::
setLevel(int level)
{
  level_ = level;
}

void
CGLOthello::
doNewGame()
{
  board_.init();

  players_[COTHELLO_PIECE_WHITE] = HUMAN_PLAYER;
  players_[COTHELLO_PIECE_BLACK] = COMPUTER_PLAYER;

  currentPlayer_ = COTHELLO_PIECE_WHITE;

  level_ = DEFAULT_LEVEL;

  gameOver_ = false;

  renderer_->flush();
}

void
CGLOthello::
computerMove()
{
  if (gameOver_) return;

  if (players_[currentPlayer_] != COMPUTER_PLAYER) return;

  int x, y;

  if (getBestMove(currentPlayer_, level_, &x, &y)) {
    doMove(x, y, currentPlayer_);

    currentPlayer_ = COthelloPiece::otherPiece(currentPlayer_);

    if (! canMoveAnywhere(currentPlayer_)) {
      currentPlayer_ = COthelloPiece::otherPiece(currentPlayer_);

      if (! canMoveAnywhere(currentPlayer_))
        gameOver_ = true;
    }

    renderer_->flush();
  }
}

COthelloPieceValue
CGLOthello::
getBoardPiece(int x, int y)
{
  return board_.getPiece(x, y);
}

bool
CGLOthello::
canMoveAnywhere(COthelloPieceValue piece)
{
  return board_.canMoveAnywhere(piece);
}

bool
CGLOthello::
canMove(int x, int y, COthelloPieceValue piece)
{
  return board_.canMove(x, y, piece);
}

void
CGLOthello::
doMove(int x, int y, COthelloPieceValue piece)
{
  board_.doMove(x, y, piece);
}

bool
CGLOthello::
isWhitePiece(int x, int y)
{
  return (board_.getPiece(x, y) == COTHELLO_PIECE_WHITE);
}

bool
CGLOthello::
isBlackPiece(int x, int y)
{
  return (board_.getPiece(x, y) == COTHELLO_PIECE_BLACK);
}

int
CGLOthello::
getNumWhite()
{
  return board_.getNumWhite();
}

int
CGLOthello::
getNumBlack()
{
  return board_.getNumBlack();
}

int
CGLOthello::
getNum()
{
  return board_.getNum();
}

bool
CGLOthello::
getBestMove(COthelloPieceValue piece, int depth, int *x, int *y)
{
  return  board_.getBestMove(piece, depth, x, y);
}

COthelloPieceValue
CGLOthello::
stringToPiece(const char *str)
{
  if (strcmp(str, "White") == 0)
    return COTHELLO_PIECE_WHITE;
  else
    return COTHELLO_PIECE_BLACK;
}

enum { BUFSIZE = 512 };

bool
CGLOthello::
buttonPressEvent(const CMouseEvent &e)
{
  if (! e.isButton(CBUTTON_LEFT)) return true;

  int name = pickScene(e);

  if (name >= 1 && name <= 64) {
    int y = (name - 1)/8;
    int x = name - 8*y - 1;

    select(x, y);
  }

  CGLWindow::buttonPressEvent(e);

  return true;
}

bool
CGLOthello::
keyPressEvent(const CKeyEvent &e)
{
  if      (e.isType(CKEY_TYPE_s))
    renderer_->getPixelRenderer()->saveImage("snap.png");
  else if (e.isType(CKEY_TYPE_Tab) || e.isType(CKEY_TYPE_TAB))
    overlay_ = ! overlay_;

  renderer_->flush();

  return true;
}

bool
CGLOthello::
timerEvent()
{
  computerMove();

  return true;
}

void
CGLOthello::
levelChanged()
{
  level_ = levelButton_->getValue();
}

void
CGLOthello::
newGamePressed()
{
  board_.init();

  players_[COTHELLO_PIECE_WHITE] = HUMAN_PLAYER;
  players_[COTHELLO_PIECE_BLACK] = COMPUTER_PLAYER;

  currentPlayer_ = COTHELLO_PIECE_WHITE;

  level_ = DEFAULT_LEVEL;

  gameOver_ = false;

  renderer_->flush();
}
