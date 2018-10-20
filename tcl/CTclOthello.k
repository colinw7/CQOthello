static char CTclOthello_TK[] = "\n\
proc createWidgets { } {\n\
  global canvas is_image\n\
  gset scoreText \"\"\n\
  loadImages\n\
  frame .left -borderwidth 0 -highlightthickness 0\n\
  set canvas .left.canvas\n\
  if {! $is_image} {\n\
    canvas $canvas -width 600 -height 600 -borderwidth 0 -highlightthickness 0\n\
  } \\\n\
  else {\n\
    canvas $canvas -width 512 -height 512 -borderwidth 0 -highlightthickness 0\n\
  }\n\
  bind $canvas <Expose> drawBoard\n\
  bind $canvas <Button-1> \"canvasSelect %x %y\"\n\
  if {! $is_image} {\n\
    pack $canvas -side left -fill both -expand true\n\
    pack .left -side left -fill both -expand true\n\
  } \\\n\
  else {\n\
    pack $canvas -side left\n\
    pack .left -side left\n\
  }\n\
  if {! $is_image} {\n\
    frame .right -borderwidth 0\n\
    label .right.color -text \"White to Move\" -textvariable moveColorText\n\
    label .right.score -text \"White 2 Black 2\" -textvariable scoreText\n\
    button .right.new_game -text \"New Game\" -command newGame\n\
    pack .right.color .right.score .right.new_game -side top\n\
    pack .right -side right\n\
  } \\\n\
  else {\n\
    global sep_canvas info_canvas\n\
    frame .right -borderwidth 0\n\
    set sep_canvas .right.sep_canvas\n\
    canvas $sep_canvas -width 8 -height 512 \\\n\
           -borderwidth 0 -highlightthickness 0\n\
    bind $sep_canvas <Expose> drawSep\n\
    pack $sep_canvas -side left\n\
    set info_canvas .right.info_canvas\n\
    canvas $info_canvas -width 128 -height 512 \\\n\
           -borderwidth 0 -highlightthickness 0\n\
    bind $info_canvas <Expose> drawInfo\n\
    bind $info_canvas <Button-1> \"canvasInfoSelect %x %y\"\n\
    pack $info_canvas -side left\n\
    pack .right -side right\n\
  }\n\
}\n\
proc loadImages { } {\n\
  global is_image\n\
  if {$is_image} {\n\
    global vbar_image control_image logo_image\n\
    global no_piece_image white_piece_image black_piece_image\n\
    global big_button_image button_image small_button_image\n\
    global divider_image left_image right_image\n\
    set vbar_image         [image create photo -file vbar.ppm]\n\
    set control_image      [image create photo -file control.ppm]\n\
    set logo_image         [image create photo -file logo.ppm]\n\
    set no_piece_image     [image create photo -file tile.ppm]\n\
    set white_piece_image  [image create photo -file white_piece.ppm]\n\
    set black_piece_image  [image create photo -file blue_piece.ppm]\n\
    set divider_image      [image create photo -file divider.ppm]\n\
    set big_button_image   [image create photo -file big_button.ppm]\n\
    set button_image       [image create photo -file button.ppm]\n\
    set small_button_image [image create photo -file small_button.ppm]\n\
    set left_image         [image create photo -file left.ppm]\n\
    set right_image        [image create photo -file right.ppm]\n\
  }\n\
}\n\
proc newGame { } {\n\
  global moveColor\n\
  CTclOthelloInitBoard\n\
  drawBoard\n\
  setMoveColor \"White\"\n\
  setScore\n\
}\n\
proc setWhitePlayer { player } {\n\
  gset whitePlayer $player\n\
}\n\
proc setBlackPlayer { player } {\n\
  gset blackPlayer $player\n\
}\n\
proc getCurrentPlayer { } {\n\
  global moveColor whitePlayer blackPlayer\n\
  if     {$moveColor == \"White\"} {\n\
    return $whitePlayer\n\
  } \\\n\
  elseif {$moveColor == \"Black\"} {\n\
    return $blackPlayer\n\
  }\n\
  else {\n\
    return \"None\"\n\
  }\n\
}\n\
proc setComputerLevel { level } {\n\
  gset computerLevel $level\n\
}\n\
proc setMoveColor { color } {\n\
  gset moveColor $color\n\
  gset moveColorText \"$color to Move\"\n\
  drawMoveColor\n\
}\n\
proc setCurrentPlayer { color } {\n\
  global currentPlayer\n\
  set currentPlayer $color\n\
}\n\
proc setScore { } {\n\
  global scoreText\n\
  set numWhite [CTclOthelloGetNumWhite]\n\
  set numBlack [CTclOthelloGetNumBlack]\n\
  set scoreText \"White $numWhite Black $numBlack\"\n\
  drawScore\n\
}\n\
proc canvasSelect { x y } {\n\
  global cellSize moveColor\n\
  set player [getCurrentPlayer]\n\
  if {$player == \"Computer\"} return\n\
  set x1 [expr $x / $cellSize]\n\
  set y1 [expr $y / $cellSize]\n\
  if {[CTclOthelloCanMove $x1 $y1 $moveColor]} {\n\
    CTclOthelloDoMove $x1 $y1 $moveColor\n\
    setScore\n\
    nextMoveColor\n\
    drawBoard\n\
    computerMove\n\
  }\n\
}\n\
proc computerMove { } {\n\
  global moveColor computerLevel\n\
  set player [getCurrentPlayer]\n\
  while {$player == \"Computer\"} {\n\
    after 250\n\
    if {! [CTclOthelloGetBestMove $moveColor $computerLevel best_x best_y]} {\n\
      break\n\
    }\n\
    global best_x\n\
    global best_y\n\
    CTclOthelloDoMove $best_x $best_y $moveColor\n\
    setScore\n\
    nextMoveColor\n\
    drawBoard\n\
    set player [getCurrentPlayer]\n\
  }\n\
}\n\
proc canvasInfoSelect { x y } {\n\
  global info_canvas_cb info_canvas_cb_proc\n\
  global info_canvas_cb_xmin info_canvas_cb_ymin \n\
  global info_canvas_cb_xmax info_canvas_cb_ymax \n\
  foreach cb $info_canvas_cb {\n\
    set xmin $info_canvas_cb_xmin($cb)\n\
    set ymin $info_canvas_cb_ymin($cb)\n\
    set xmax $info_canvas_cb_xmax($cb)\n\
    set ymax $info_canvas_cb_ymax($cb)\n\
    if {$x >= $xmin && $x <= $xmax && $y >= $ymin && $y <= $ymax} {\n\
      set proc $info_canvas_cb_proc($cb)\n\
      $proc\n\
    }\n\
  }\n\
}\n\
proc drawBoard { } {\n\
  global canvas is_image\n\
  global cellSize pieceSize pieceBorder\n\
  $canvas delete white\n\
  $canvas delete black\n\
  $canvas delete line\n\
  if {$is_image} {\n\
    set cellSize    64\n\
    set pieceSize   64\n\
    set pieceBorder 0\n\
  } \\\n\
  else {\n\
    set canvas_width  [winfo width  $canvas]\n\
    set canvas_height [winfo height $canvas]\n\
    if {$canvas_width < $canvas_height} {\n\
      set cellSize [expr $canvas_width  / 8]\n\
    } \\\n\
    else {\n\
      set cellSize [expr $canvas_height / 8]\n\
    }\n\
    set pieceSize [expr $cellSize * 0.9 ]\n\
    set pieceBorder [expr ($cellSize - $pieceSize) / 2.0]\n\
  }\n\
  if {! $is_image} {\n\
    set x 0\n\
    for {set i 0} {$i < 8} {incr i} {\n\
      $canvas create line $x 0 $x [expr 8 * $cellSize] -tag line\n\
      set x [expr $x + $cellSize]\n\
    }\n\
    set y 0\n\
    for {set i 0} {$i < 8} {incr i} {\n\
      $canvas create line 0 $y [expr 8 * $cellSize] $y -tag line\n\
      set y [expr $y + $cellSize]\n\
    }\n\
  }\n\
  set y 0\n\
  for {set i 0} {$i < 8} {incr i} {\n\
    set x 0\n\
    for {set j 0} {$j < 8} {incr j} {\n\
      if     {[CTclOthelloIsWhitePiece $j $i]} {\n\
        drawWhitePiece $x $y\n\
      } \\\n\
      elseif {[CTclOthelloIsBlackPiece $j $i]} {\n\
        drawBlackPiece $x $y\n\
      } \\\n\
      else {\n\
        drawNoPiece $x $y\n\
      }\n\
      set x [expr $x + $cellSize]\n\
    }\n\
    set y [expr $y + $cellSize]\n\
  }\n\
  update\n\
}\n\
proc drawNoPiece { x y } {\n\
  global canvas is_image\n\
  if {$is_image} {\n\
    global no_piece_image\n\
    $canvas create image $x $y -image $no_piece_image -anchor nw\n\
  }\n\
}\n\
proc drawWhitePiece { x y } {\n\
  global canvas is_image\n\
  if {$is_image} {\n\
    global white_piece_image\n\
    $canvas create image $x $y -image $white_piece_image -anchor nw\n\
  } \\\n\
  else {\n\
    global pieceSize pieceBorder\n\
    set x1 [expr $x  + $pieceBorder]\n\
    set y1 [expr $y  + $pieceBorder]\n\
    set x2 [expr $x1 + $pieceSize]\n\
    set y2 [expr $y1 + $pieceSize]\n\
    $canvas create oval $x1 $y1 $x2 $y2 -tag white -fill white\n\
  }\n\
}\n\
proc drawBlackPiece { x y } {\n\
  global canvas is_image\n\
  if {$is_image} {\n\
    global black_piece_image\n\
    $canvas create image $x $y -image $black_piece_image -anchor nw\n\
  } \\\n\
  else {\n\
    global pieceSize pieceBorder\n\
    set x1 [expr $x  + $pieceBorder]\n\
    set y1 [expr $y  + $pieceBorder]\n\
    set x2 [expr $x1 + $pieceSize]\n\
    set y2 [expr $y1 + $pieceSize]\n\
    $canvas create oval $x1 $y1 $x2 $y2 -tag black -fill black\n\
  }\n\
}\n\
proc drawSep { } {\n\
  global sep_canvas vbar_image\n\
  $sep_canvas delete vbar\n\
  $sep_canvas create image 0 0 -image $vbar_image -anchor nw -tag vbar\n\
}\n\
proc drawInfo { } {\n\
  gset info_canvas_cb \"\"\n\
  global info_canvas control_image logo_image\n\
  global big_button_image button_image small_button_image\n\
  global divider_image left_image right_image\n\
  $info_canvas delete info\n\
  canvasCreateImage  0   0 $control_image    info\n\
  canvasCreateImage  0   0 $logo_image       info\n\
  canvasCreateImage  0  60 $divider_image    info\n\
  canvasCreateImage  0 290 $divider_image    info\n\
  canvasCreateImage  0 455 $divider_image    info\n\
  $info_canvas delete text_info\n\
  canvasCreateText 8 140 \"White\" text_info\n\
  canvasCreateText 8 180 \"Black\" text_info\n\
  canvasCreateButton  4  80 120 20 $big_button_image \\\n\
                     current_player_button current_player_cb\n\
  canvasCreateButton 55 130  70 20 $button_image \\\n\
                     white_player_button white_player_cb\n\
  canvasCreateButton 55 170  70 20 $button_image \\\n\
                     black_player_button black_player_cb\n\
  canvasCreateButton 10 220 20 20 $left_image \\\n\
                     level_down_button level_down_cb\n\
  canvasCreateButton 95 220 20 20 $right_image \\\n\
                     level_up_button level_up_cb\n\
  canvasCreateTextButton  2 470 70 20 $button_image \\\n\
                          4 480 \"New Game\" \\\n\
                         new_game_button new_game_button_cb\n\
  canvasCreateTextButton 76 470 50 20 $small_button_image \\\n\
                         88 480 \"Quit\" \\\n\
                         quit_button quit_cb\n\
  $info_canvas raise text_info\n\
  drawMoveColor\n\
  drawPlayers\n\
  drawLevel\n\
  drawScore\n\
}\n\
proc drawMoveColor { } {\n\
  global info_canvas is_image\n\
  if {[info exists info_canvas] && $is_image} {\n\
    global moveColorText\n\
    $info_canvas delete move_color\n\
    canvasCreateText 8 90 $moveColorText move_color\n\
    $info_canvas raise move_color\n\
  }\n\
}\n\
proc drawPlayers { } {\n\
  global info_canvas is_image\n\
  if {[info exists info_canvas] && $is_image} {\n\
    global whitePlayer blackPlayer\n\
    $info_canvas delete player\n\
    canvasCreateText 60 140 $whitePlayer player\n\
    canvasCreateText 60 180 $blackPlayer player\n\
    $info_canvas raise player\n\
  }\n\
}\n\
proc drawLevel { } {\n\
  global info_canvas is_image\n\
  if {[info exists info_canvas] && $is_image} {\n\
    global computerLevel\n\
    $info_canvas delete level\n\
    set levelText \"Level $computerLevel\"\n\
    canvasCreateText 40 230 $levelText level\n\
    $info_canvas raise level\n\
  }\n\
}\n\
proc drawScore { } {\n\
  global info_canvas is_image\n\
  if {[info exists info_canvas] && $is_image} {\n\
    global scoreText\n\
    $info_canvas delete score\n\
    canvasCreateText 18 330 $scoreText score\n\
    $info_canvas raise score\n\
  }\n\
}\n\
proc canvasCreateButton { ix iy wx wy image tag proc } {\n\
  global info_canvas\n\
  $info_canvas delete $tag\n\
  canvasCreateImage $ix $iy $image $tag\n\
  $info_canvas raise $tag\n\
  global info_canvas_cb info_canvas_cb_proc\n\
  global info_canvas_cb_xmin info_canvas_cb_ymin\n\
  global info_canvas_cb_xmax info_canvas_cb_ymax\n\
  lappend info_canvas_cb $tag\n\
  set info_canvas_cb_proc($tag) $proc\n\
  set info_canvas_cb_xmin($tag) $ix\n\
  set info_canvas_cb_ymin($tag) $iy\n\
  set info_canvas_cb_xmax($tag) [expr $ix + $wx]\n\
  set info_canvas_cb_ymax($tag) [expr $iy + $wy]\n\
}\n\
proc canvasCreateTextButton { ix iy wx wy image tx ty text tag proc } {\n\
  global info_canvas\n\
  $info_canvas delete $tag\n\
  canvasCreateImage $ix $iy $image $tag\n\
  canvasCreateText $tx $ty $text $tag\n\
  $info_canvas raise $tag\n\
  global info_canvas_cb info_canvas_cb_proc\n\
  global info_canvas_cb_xmin info_canvas_cb_ymin\n\
  global info_canvas_cb_xmax info_canvas_cb_ymax\n\
  lappend info_canvas_cb $tag\n\
  set info_canvas_cb_proc($tag) $proc\n\
  set info_canvas_cb_xmin($tag) $ix\n\
  set info_canvas_cb_ymin($tag) $iy\n\
  set info_canvas_cb_xmax($tag) [expr $ix + $wx]\n\
  set info_canvas_cb_ymax($tag) [expr $iy + $wy]\n\
}\n\
proc canvasCreateText { x y text tag } {\n\
  global info_canvas\n\
  set font *adobe*helvetica*bold-r-normal*12*\n\
  set x1 [expr $x - 1]\n\
  set y1 [expr $y - 1]\n\
  $info_canvas create text $x1 $y1 -text $text -tag $tag \\\n\
               -fill grey70 -justify left -anchor w -font $font\n\
  $info_canvas create text $x $y -text $text -tag $tag \\\n\
               -fill black -justify left -anchor w -font $font\n\
}\n\
proc canvasCreateImage { x y name tag } {\n\
  global info_canvas\n\
  $info_canvas create image $x $y -image $name -anchor nw -tag $tag\n\
}\n\
proc nextMoveColor { } {\n\
  global moveColor\n\
  setMoveColor [otherColor $moveColor]\n\
  if { ! [CTclOthelloCanMoveAnywhere $moveColor] } {\n\
    setMoveColor [otherColor $moveColor]\n\
    if { ! [CTclOthelloCanMoveAnywhere $moveColor] } {\n\
      gameOver\n\
    }\n\
  }\n\
}\n\
proc gameOver { } {\n\
  global scoreText\n\
  set numWhite [CTclOthelloGetNumWhite]\n\
  set numBlack [CTclOthelloGetNumBlack]\n\
  if     {$numWhite > $numBlack } {\n\
    set scoreText \"White Wins\"\n\
  } \\\n\
  elseif {$numBlack > $numWhite } {\n\
    set scoreText \"Black Wins\"\n\
  } \\\n\
  else {\n\
    set scoreText \"Draw\"\n\
  }\n\
  drawScore\n\
}\n\
proc otherPlayer { } {\n\
  global currentPlayer\n\
  set currentPlayer [otherColor $currentPlayer]\n\
}\n\
proc otherColor { color } {\n\
  if {$color == \"White\"} {\n\
    return \"Black\"\n\
  } \\\n\
  else {\n\
    return \"White\"\n\
  }\n\
}\n\
proc otherPlayer { player } {\n\
  if {$player == \"Human\"} {\n\
    return \"Computer\"\n\
  } \\\n\
  else {\n\
    return \"Human\"\n\
  }\n\
}\n\
proc current_player_cb { } {\n\
  set num [CTclOthelloGetNum]\n\
  if {$num == 4} {\n\
    global moveColor\n\
    setMoveColor [otherColor $moveColor]\n\
    computerMove\n\
  }\n\
}\n\
proc white_player_cb { } {\n\
  global whitePlayer\n\
  set whitePlayer [otherPlayer $whitePlayer]\n\
  drawPlayers\n\
  computerMove\n\
}\n\
proc black_player_cb { } {\n\
  global blackPlayer\n\
  set blackPlayer [otherPlayer $blackPlayer]\n\
  drawPlayers\n\
  computerMove\n\
}\n\
proc level_down_cb { } {\n\
  global computerLevel\n\
  if {$computerLevel > 1} {\n\
    incr computerLevel -1\n\
  }\n\
  drawLevel\n\
}\n\
proc level_up_cb { } {\n\
  global computerLevel\n\
  if {$computerLevel < 10} {\n\
    incr computerLevel\n\
  }\n\
  drawLevel\n\
}\n\
proc new_game_button_cb { } {\n\
  newGame\n\
}\n\
proc quit_cb { } {\n\
  exit\n\
}\n\
gset is_image 1\n\
CTclOthelloInitBoard\n\
setWhitePlayer \"Human\"\n\
setBlackPlayer \"Computer\"\n\
setComputerLevel 3\n\
setMoveColor \"White\"\n\
createWidgets\n\
newGame\n\
";

