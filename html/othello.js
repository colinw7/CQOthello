'use strict';

window.addEventListener("load", eventWindowLoaded, false);

//------

var othello = new Othello();

function eventWindowLoaded () {
  if (canvasSupport()) {
    othello.init();
  }
}

function canvasSupport () {
  return true;
  //return Modernizr.canvas;
}

function assert(condition, message) {
  if (! condition) {
    throw message || "Assertion failed";
  }
}

//------

function Othello () {
}

Othello.prototype.init = function() {
  this.scoreText     = "";
  this.moveColor     = "White";
  this.whitePlayer   = "Human";
  this.blackPlayer   = "Computer";
  this.computerLevel = 3;

  //---

  this.createModels();

  //---

  this.connectWidgets();

  //---

  this.setWhitePlayer("Human");
  this.setBlackPlayer("Computer");

  this.setComputerLevel(3);

  this.setMoveColor("White");

  this.newGame();
};

Othello.prototype.connectWidgets = function() {
  this.canvas = document.getElementById("canvas");
  this.gc     = this.canvas.getContext("2d");

  //---

  this.colorLabel = document.getElementById("colorLabel");
  this.scoreLabel = document.getElementById("scoreLabel");
  //console.log("colorLabel", this.colorLabel);
  //console.log("scoreLabel", this.scoreLabel);

  //----

  var newGameButton = document.getElementById("newGameButton");

  newGameButton.addEventListener('click', this.newGamePressed, false);

  //---

  this.canvas.addEventListener("mousedown", this.eventMouseDown, false);
  this.canvas.addEventListener("mousemove", this.eventMouseMove, false);
  this.canvas.addEventListener("mouseup"  , this.eventMouseUp  , false);
};

Othello.prototype.eventMouseDown = function(e) {
//console.log("Othello.prototype.eventMouseDown", e);
  var rect = othello.canvas.getBoundingClientRect();

  var mouseX = e.clientX - rect.left;
  var mouseY = e.clientY - rect.top;

  var x = Math.floor(mouseX / 80);
  var y = Math.floor(mouseY / 80);

  if (x >= 0 && x < 8 && y >= 0 && y < 8) {
    othello.pressBoard(x, y);
  }
};

Othello.prototype.eventMouseMove = function(e) {
};

Othello.prototype.eventMouseUp = function(e) {
};

Othello.prototype.newGamePressed = function() {
  othello.newGame();
};

Othello.prototype.newGame = function() {
  this.initBoard();

  this.setMoveColor("White");

  this.setScore();

  this.drawScreen();
};

Othello.prototype.setWhitePlayer = function(player) {
  this.whitePlayer = player;
};

Othello.prototype.setBlackPlayer = function(player) {
  this.blackPlayer = player;
};

Othello.prototype.getCurrentPlayer = function() {
  if        (this.moveColor == "White") {
    return this.whitePlayer;
  } else if (this.moveColor == "Black") {
    return this.blackPlayer;
  } else {
    return "None";
  }
};

Othello.prototype.setComputerLevel = function(level) {
  this.computerLevel = level;
};

Othello.prototype.setMoveColor = function(color) {
  //console.log("setMoveColor", color);

  this.moveColor     = color;
  this.moveColorText = color + " to Move";

//console.log("colorLabel", this.colorLabel, this.moveColorText);
//this.colorLabel = document.getElementById("colorLabel");
//this.colorLabel.innerHTML = this.moveColorText;
  document.getElementById("colorLabel").innerHTML = this.moveColorText;
};

Othello.prototype.setCurrentPlayer = function(color) {
  this.currentPlayer = color;
};

Othello.prototype.setScore = function() {
  console.log("setScore");

  var numWhite = this.getNumWhite();
  var numBlack = this.getNumBlack();

  this.scoreText = "White " + numWhite + " Black " + numBlack;

//console.log("scoreLabel", this.scoreLabel, this.scoreText);
//this.scoreLabel = document.getElementById("scoreLabel");
//this.scoreLabel.innerHtml = this.scoreText;
  document.getElementById("scoreLabel").innerHTML = this.scoreText;
};

Othello.prototype.canMove = function(x, y, color) {
  var rc = this.modelCanMove(this.boardValues, x, y, color);
  //console.log("canMove", x, y, color, rc);

  return rc;
};

Othello.prototype.modelCanMove = function(model, x, y, color) {
  if (! this.isModelNoPiece(model, x, y)) {
    return 0;
  }

  var color1 = this.otherColor(color);

  if (this.modelCanMoveDirection(model, x, y,  1,  0, color, color1)) { return 1; }
  if (this.modelCanMoveDirection(model, x, y,  1,  1, color, color1)) { return 1; }
  if (this.modelCanMoveDirection(model, x, y,  0,  1, color, color1)) { return 1; }
  if (this.modelCanMoveDirection(model, x, y, -1,  1, color, color1)) { return 1; }
  if (this.modelCanMoveDirection(model, x, y, -1,  0, color, color1)) { return 1; }
  if (this.modelCanMoveDirection(model, x, y, -1, -1, color, color1)) { return 1; }
  if (this.modelCanMoveDirection(model, x, y,  0, -1, color, color1)) { return 1; }
  if (this.modelCanMoveDirection(model, x, y,  1, -1, color, color1)) { return 1; }

  return 0;
};

Othello.prototype.canMoveDirection = function(x, y, dx, dy, color, otherColor) {
  //console.log("canMoveDirection", x, y, dx, dy, color, otherColor);

  return this.modelCanMoveDirection(this.boardValues, x, y, dx, dy, color, otherColor);
};

Othello.prototype.modelCanMoveDirection = function(model, x, y, dx, dy, color, otherColor) {
  var x1 = x + dx;
  var y1 = y + dy;

  var value = this.getModelPieceColor(model, x1, y1);

  if (value != otherColor) {
    return 0;
  }

  var x1 = x1 + dx;
  var y1 = y1 + dy;

  var value = this.getModelPieceColor(model, x1, y1);

  while (value == otherColor) {
    var x1 = x1 + dx;
    var y1 = y1 + dy;

    var value = this.getModelPieceColor(model, x1, y1);
  }

  var value = this.getModelPieceColor(model, x1, y1);

  if (value != color) {
    return 0;
  }

  return 1;
};

Othello.prototype.doMove = function(x, y, color) {
  //console.log("doMove", x, y, color);

  this.modelDoMove(this.boardValues, x, y, color);
};

Othello.prototype.modelDoMove = function(model, x, y, color) {
  var color1 = this.otherColor(color);

  if (this.modelCanMoveDirection(model, x, y,  1,  0, color, color1)) {
    this.modelDoMoveDirection(model, x, y,  1,  0, color, color1);
  }
  if (this.modelCanMoveDirection(model, x, y,  1,  1, color, color1)) {
    this.modelDoMoveDirection(model, x, y,  1,  1, color, color1);
  }
  if (this.modelCanMoveDirection(model, x, y,  0,  1, color, color1)) {
    this.modelDoMoveDirection(model, x, y,  0,  1, color, color1);
  }
  if (this.modelCanMoveDirection(model, x, y, -1,  1, color, color1)) {
    this.modelDoMoveDirection(model, x, y, -1,  1, color, color1);
  }
  if (this.modelCanMoveDirection(model, x, y, -1,  0, color, color1)) {
    this.modelDoMoveDirection(model, x, y, -1,  0, color, color1);
  }
  if (this.modelCanMoveDirection(model, x, y, -1, -1, color, color1)) {
    this.modelDoMoveDirection(model, x, y, -1, -1, color, color1);
  }
  if (this.modelCanMoveDirection(model, x, y,  0, -1, color, color1)) {
    this.modelDoMoveDirection(model, x, y,  0, -1, color, color1);
  }
  if (this.modelCanMoveDirection(model, x, y,  1, -1, color, color1)) {
    this.modelDoMoveDirection(model, x, y,  1, -1, color, color1);
  }

  this.setModelPieceColor(model, x, y, color);
};

Othello.prototype.doMoveDirection = function(x, y, dx, dy, color, otherColor) {
  //console.log("doMoveDirection", x, y, dx, dy, color, otherColor);

  return this.modelDoMoveDirection(this.boardValues, x, y, dx, dy, color, otherColor);
};

Othello.prototype.modelDoMoveDirection = function(model, x, y, dx, dy, color, otherColor) {
  var x1 = x + dx;
  var y1 = y + dy;

  var value = this.getModelPieceColor(model, x1, y1);

  if (value != otherColor) {
    return;
  }

  this.setPieceColor(x1, y1, color);

  var x1 = x1 + dx;
  var y1 = y1 + dy;

  var value = this.getModelPieceColor(model, x1, y1);

  while (value == otherColor) {
    this.setPieceColor(x1, y1, color);

    var x1 = x1 + dx;
    var y1 = y1 + dy;

    var value = this.getModelPieceColor(model, x1, y1);
  }
};

Othello.prototype.getBestMove = function(value, maxDepth) {
  return this.modelGetBestMove(this.boardValues, value, maxDepth);
};

Othello.prototype.modelGetBestMove = function(model, value, maxDepth) {
  return this.modelGetBestMove1(model, value, maxDepth, 0);
};

Othello.prototype.modelGetBestMove1 = function(model, value, maxDepth, depth) {
  //console.log("modelGetBestMove1", model, value, maxDepth, depth);

  if (maxDepth <= 0) {
    var score = 0;

    return {}
  }

  var color = this.valueColor(value);

  var move = {};

  var score = -9999;

  var board1 = this.getDepthBoard(model, depth);

  var board_score = this.modelBoardScore(model, value);

  for (var x1 = 0; x1 < 8; x1++) {
    for (var y1 = 0; y1 < 8; y1++) {
      var rc = this.modelCanMove(board1, x1, y1, color);
      //console.log("modelCanMove", board1, x1, y1, color, rc);

      if (! rc) {
        continue;
      }

      this.modelDoMove(board1, x1, y1, value);

      var score1 = this.modelBoardScore(board1, value) - board_score;

      var value1    = this.otherValue(value);
      var maxDepth1 = maxDepth - 1;
      var depth1    = maxDepth + 1;

      var move2 = this.modelGetBestMove1(board1, value1, maxDepth1, depth1);
      //console.log("modelGetBestMove1", board1, value1, maxDepth1, depth1, move2);

      if (move2.length) {
        //console.log("move2:", move2);

        var score2 = move2[2];

        var score1 = score1 - score2;
      }

      if (! move.length || score1 > score || (score1 == score && this.randBool())) {
        var move = [];

        move.push(x1);
        move.push(y1);
        move.push(score1);

        var score = score1;
      }

      var board1 = this.getDepthBoard(model, depth);
    }
  }

  return move;
};

Othello.prototype.randBool = function() {
  var r = Math.random();

  return r < 0.5;
};

Othello.prototype.copyModel = function(fromModel, toModel) {
  for (var y = 0; y < 8; ++y) {
    for (var x = 0; x < 8; ++x) {
      var value = fromModel[y][x];

      toModel[y][x] = value;
    }
  }
};

Othello.prototype.createTempModel = function() {
  var values = [
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ]
  ];

  return values;
};

Othello.prototype.getDepthBoard = function(model, depth) {
  //console.log("getDepthBoard", model, depth);

  var numDepthModels = this.depthModels.length;

  var depth1 = depth + 1;

  while (depth1 > numDepthModels) {
    var model1 = this.createTempModel();

    this.depthModels.push(model1);

    numDepthModels++;
  }

  var model1 = this.depthModels[depth];

  this.copyModel(model, model1);

  return model1;
};

Othello.prototype.computerMove = function() {
  var player = this.getCurrentPlayer();

  console.log("computerMove", player);

  while (player == "Computer") {
    //after(250);

    var moveValue = this.colorValue(this.moveColor);

    var bestMove = this.getBestMove(moveValue, this.computerLevel);

    if (! bestMove.length) {
      console.log("No computer Move");
      break;
    }

    console.log("computerBestMove", bestMove);

    this.best_x = bestMove[0];
    this.best_y = bestMove[1];

    this.doMove(this.best_x, this.best_y, this.moveColor);

    this.setScore();

    this.nextMoveColor();

    var player = this.getCurrentPlayer();

    this.drawScreen();
  }
};

Othello.prototype.isNoPiece = function(x, y) {
  return this.isModelNoPiece(this.boardValues, x, y);
};

Othello.prototype.isModelNoPiece = function(model, x, y) {
  var value = this.getModelPiece(model, x, y);

  if (value == -1) {
    return 1;
  } else {
    return 0;
  }
};

Othello.prototype.isWhitePiece = function(x, y) {
  return this.isModeWhitePiece(this.boardValues, x, y);
};

Othello.prototype.isModelWhitePiece = function(model, x, y) {
  var value = this.getModelPiece(model, x, y);

  if (value == 1) {
    return 1;
  } else {
    return 0;
  }
};

Othello.prototype.isBlackPiece = function(x, y) {
  return this.isModelBlackPiece(this.boardValues, x, y);
};

Othello.prototype.isModelBlackPiece = function(model, x, y) {
  var value = this.getModelPiece(model, x, y);

  if (value == 0) {
    return 1;
  } else {
    return 0;
  }
};

Othello.prototype.getPiece = function(x, y) {
  return this.getModelPiece(this.boardValues, x, y);
};

Othello.prototype.getModelPiece = function(model, x, y) {
  if (x < 0 || x >= 8 || y < 0 || y >= 8) {
    return -1;
  }

  return model[y][x];
};

Othello.prototype.getPieceColor = function(x, y) {
  return this.getModelPieceColor(this.boardValues, x, y);
};

Othello.prototype.getModelPieceColor = function(model, x, y) {
  var value = this.getModelPiece(model, x, y);

  return this.valueColor(value);
};

Othello.prototype.setPiece = function(x, y, value) {
  return this.setModelPiece(this.boardValues, x, y, value);
};

Othello.prototype.setModelPiece = function(model, x, y, value) {
  if (x < 0 || x >= 8 || y < 0 || y >= 8) {
    return -1;
  }

  model[y][x] = value;
};

Othello.prototype.setPieceColor = function(x, y, color) {
  this.setModelPieceColor(this.boardValues, x, y, color);
};

Othello.prototype.setModelPieceColor = function(model, x, y, color) {
  //echo "console.log("setModelPieceColor", model, x, y, color);

  var value = this.colorValue(color);

  this.setModelPiece(model, x, y, value);
};

Othello.prototype.getNumWhite = function() {
  return this.getModelNumWhite(this.boardValues);
};

Othello.prototype.getModelNumWhite = function(model) {
  return this.getModelNumOfValue(model, 1);
};

Othello.prototype.getNumBlack = function() {
  return this.getModelNumBlack(this.boardValues);
};

Othello.prototype.getModelNumBlack = function(model) {
  return this.getModelNumOfValue(model, 0);
};

Othello.prototype.getNum = function() {
  return this.getModelNum(this.boardValues);
};

Othello.prototype.getModelNum = function(model) {
  return this.getModelNumWhite(model) + this.getModelNumBlack(model);
};

Othello.prototype.getNumOfValue = function(value) {
  return this.getModelNumOfValue(this.boardValues, value);
};

Othello.prototype.getModelNumOfValue = function(model, value) {
  var n = 0;

  for (var y = 0; y < 8; ++y) {
    for (var x = 0; x < 8; ++x) {
      var value1 = this.getModelPiece(model, x, y);

      if (value1 == value) {
        ++n;
      }
    }
  }

  return n;
};

Othello.prototype.getMovesTaken = function() {
  return this.getModelMovesTaken(this.boardValues);
};

Othello.prototype.getModelMovesTaken = function(model) {
  return 80 - this.getModelNumOfValue(model, -1);
};

Othello.prototype.canMoveAnywhere = function(color) {
  return this.modelCanMoveAnywhere(this.boardValues, color);
};

Othello.prototype. modelCanMoveAnywhere = function(model, color) {
  for (var x = 0; x < 8; ++x) {
    for (var y = 0; y < 8; ++y) {
      var rc = this.modelCanMove(model, x, y, color);
      //console.log("modelCanMove", model, x, y, color, rc);

      if (rc) {
        return 1;
      }
    }
  }

  return 0;
};

Othello.prototype.nextMoveColor = function() {
  this.setMoveColor(this.otherColor(this.moveColor));

  if (! this.canMoveAnywhere(this.moveColor)) {
    this.setMoveColor(this.otherColor(this.moveColor));

    if (! this.canMoveAnywhere(this.moveColor)) {
      this.gameOver();
    }
  }
};

Othello.prototype.boardScore = function(value) {
  return this.modelBoardScore(this.boardValues, value);
};

Othello.prototype.modelBoardScore = function(model, value) {
  //console.log("modelBoardScore", model, value);

  var other_value = this.otherValue(value);

  var score = 0;

  for (var x = 0; x < 8; ++x) {
    for (var y = 0; y < 8; ++y) {
      var value1 = model[y][x];
      var score  = this.scoreValues[y][x];

      if        (value1 == value) {
        var score = score + value;
      } else if (value1 == other_value) {
        var score = score - value;
      }
    }
  }

  return score;
};

Othello.prototype.gameOver = function() {
  var numWhite = this.getNumWhite();
  var numBlack = this.getNumBlack();

  if        (numWhite > numBlack ) {
    this.scoreText = "White Wins";
  } else if (numBlack > numWhite ) {
    this.scoreText = "Black Wins";
  } else {
    this.scoreText = "Draw";
  }
};

Othello.prototype.otherPlayer = function() {
  this.currentPlayer = this.otherColor(this.currentPlayer);
};

Othello.prototype.otherColor = function(color) {
  if (color == "White") {
    return "Black";
  } else {
    return "White";
  }
};

Othello.prototype.otherPlayer = function(player) {
  if (player == "Human") {
    return "Computer";
  } else {
    return "Human";
  }
};

Othello.prototype.otherValue = function(value) {
  if (value == 0) { return 1; }
  if (value == 1) { return 0; }

  return -1;
};

Othello.prototype.colorValue = function(color) {
  if (color == "Black") { return 0; }
  if (color == "White") { return 1; }

  return -1;
};

Othello.prototype. valueColor = function(value) {
  if (value == 0) { return "Black"; }
  if (value == 1) { return "White"; }

  return "None";
};

//---

Othello.prototype.createModels = function() {
  this.scoreValues = [
    [ 128,   1,  64,   8,   8,  64,   1, 128 ],
    [   1,   0,   4,   2,   2,   4,   0,   1 ],
    [  64,   4,  32,  16,  16,  32,   4,  64 ],
    [   8,   2,  16,   0,   0,  16,   2,   8 ],
    [   8,   2,  16,   0,   0,  16,   2,   8 ],
    [  64,   4,  32,  16,  16,  32,   4,  64 ],
    [   1,   0,   4,   2,   2,   4,   0,   1 ],
    [ 128,   1,  64,   8,   8,  64,   1, 128 ]
  ];

  //---

  this.boardValues = [
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ],
    [ -1, -1, -1, -1, -1, -1, -1, -1 ]
  ];

  //---

  this.initBoard();

  //---

  this.depthModels = [];
};

Othello.prototype.initBoard = function() {
  for (var y = 0; y < 8; ++y) {
    for (var x = 0; x < 8; ++x) {
      this.boardValues[y][x] = -1;
    }
  }

  this.boardValues[3][3] = 1;
  this.boardValues[3][4] = 0;
  this.boardValues[4][3] = 0;
  this.boardValues[4][4] = 1;
};

Othello.prototype.pressBoard = function(x, y) {
  var player = this.getCurrentPlayer();

  if (player == "Computer") {
    return;
  }

  console.log("Press", x, y);

  if (this.canMove(x, y, this.moveColor)) {
    this.doMove(x, y, this.moveColor);

    this.setScore();

    this.nextMoveColor();
 
    this.drawScreen();

    this.computerMove();
  }
};

Othello.prototype.drawScreen = function() {
  this.gc.clearRect(0, 0, this.canvas.width, this.canvas.height);

  var w = this.canvas.width  - 2*this.border;
  var h = this.canvas.height - 4*this.border;

  var tw = 80;
  var th = 80;

  var bx1 = 0;
  var bx2 = bx1 + 8*tw;

  var by1 = 0;
  var by2 = by1 + 8*th;

  this.gc.fillStyle   = "#8888aa";
  this.gc.strokeStyle = "#00000000";

  this.gc.beginPath();

  this.gc.moveTo(bx1, by1);
  this.gc.lineTo(bx2, by1);
  this.gc.lineTo(bx2, by2);
  this.gc.lineTo(bx1, by2);

  this.gc.closePath();

  this.gc.fill();
  this.gc.stroke();

  //---

  for (var x = 0; x <= 8; ++x) {
    this.gc.fillStyle   = "#00000000";
    this.gc.strokeStyle = "#dddddd";

    var lx  = x*tw;
    var ly1 = by1;
    var ly2 = by2;

    this.gc.beginPath();
    
    this.gc.moveTo(lx, ly1);
    this.gc.lineTo(lx, ly2);

    this.gc.stroke();
  }

  for (var y = 0; y < 8; ++y) {
    this.gc.fillStyle   = "#00000000";
    this.gc.strokeStyle = "#dddddd";

    var ly  = y*th;
    var lx1 = bx1;
    var lx2 = bx2;

    this.gc.beginPath();

    this.gc.moveTo(lx1, ly);
    this.gc.lineTo(lx2, ly);

    this.gc.stroke();
  }

  for (var y = 0; y < 8; ++y) {
    var ty1 = y*th;
    var ty2 = ty1 + th;

    for (var x = 0; x < 8; ++x) {
      var tx1 = x*tw;
      var tx2 = tx1 + tw;

      if      (this.boardValues[y][x] == 0) {
        this.gc.fillStyle   = "#000000";
        this.gc.strokeStyle = "#ffffff";
      }
      else if (this.boardValues[y][x] == 1) {
        this.gc.fillStyle   = "#ffffff";
        this.gc.strokeStyle = "#000000";
      }
      else {
        continue;
      }

      var xc = (tx1 + tx2)/2;
      var yc = (ty1 + ty2)/2;

      var w = tx2 - tx1;
      var h = ty2 - ty1;

      var r = (w + h)/4;

      var d = r*4*(Math.sqrt(2) - 1)/3;

      this.gc.beginPath();

      this.gc.moveTo       (                                xc + r, yc    );
      this.gc.bezierCurveTo(xc + r, yc + d, xc + d, yc + r, xc    , yc + r);
      this.gc.bezierCurveTo(xc - d, yc + r, xc - r, yc + d, xc - r, yc    );
      this.gc.bezierCurveTo(xc - r, yc - d, xc - d, yc - r, xc    , yc - r);
      this.gc.bezierCurveTo(xc + d, yc - r, xc + r, yc - d, xc + r, yc    );

      this.gc.closePath();

      this.gc.fill();
      this.gc.stroke();
    }
  }
};
