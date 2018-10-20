#include <CGLApp/CGLWindow.h>
#include <CGLApp/CGLMaterial.h>
#include <COthelloLib/COthello.h>
#include <CImageLib/CImageLib.h>

class CGLOthello;

class COthelloBoard;
class CGLRenderer3D;
class CGLWidget;
class CGLLabel;
class CGLIntSpinBox;
class CGLPushButton;

class CGLOthello : public CGLWindow {
 private:
  enum Player {
    HUMAN_PLAYER,
    COMPUTER_PLAYER
  };

  typedef std::map<COthelloPieceValue,Player> PlayerMap;

  COthelloBoard       board_;
  CGLRenderer3D      *renderer_;
  CImagePtr           white_image_;
  CImagePtr           black_image_;
  CImagePtr           empty_image_;
  CGLTexture         *black_texture_;
  CGLTexture         *white_texture_;
  CGLTexture         *board_texture_;
  CGLMaterial         board_material_;
  CGLMaterial         piece_material_;
  CPoint3D            lightPos_;
  bool                overlay_;
  PlayerMap           players_;
  COthelloPieceValue  currentPlayer_;
  int                 level_;
  bool                gameOver_;
  double              bmin_, bmax_;
  CGLWidget          *frame_;
  CGLLabel           *scoreLabel_;
  CGLLabel           *moveLabel_;
  CGLIntSpinBox      *levelButton_;
  CGLPushButton      *newGameButton_;

 public:
  CGLOthello(int, int, uint, uint);

  void setup();

  void drawScene();

  void drawBoard();
  void drawShadows();
  void drawPieces(bool);

  void drawWhitePiece(const CPoint2D &, bool);
  void drawBlackPiece(const CPoint2D &, bool);

  void drawOverlay();

  void select(int x, int y);

  COthelloPieceValue getBoardPiece(int x, int y);

  bool canMoveAnywhere(COthelloPieceValue piece);

  bool canMove(int x, int y, COthelloPieceValue piece);

  void doMove(int x, int y, COthelloPieceValue piece);

  void setLevel(int);

  void doNewGame();

  void computerMove();

  bool isWhitePiece(int x, int y);
  bool isBlackPiece(int x, int y);

  int getNumWhite();
  int getNumBlack();

  int getNum();

  bool getBestMove(COthelloPieceValue piece, int depth, int *x, int *y);

  COthelloPieceValue stringToPiece(const char *str);

  bool buttonPressEvent(const CMouseEvent &e);
  bool keyPressEvent(const CKeyEvent &e);

  bool timerEvent();

  void levelChanged();
  void newGamePressed();
};
