proc createWidgets { } {
  global canvas is_image

  gset scoreText ""

  #----

  loadImages

  #----

  frame .left -borderwidth 0 -highlightthickness 0

  set canvas .left.canvas

  if {! $is_image} {
    canvas $canvas -width 600 -height 600 -borderwidth 0 -highlightthickness 0
  } \
  else {
    canvas $canvas -width 512 -height 512 -borderwidth 0 -highlightthickness 0
  }

  bind $canvas <Expose> drawBoard
  bind $canvas <Button-1> "canvasSelect %x %y"

  if {! $is_image} {
    pack $canvas -side left -fill both -expand true

    pack .left -side left -fill both -expand true
  } \
  else {
    pack $canvas -side left

    pack .left -side left
  }

  #----

  if {! $is_image} {
    frame .right -borderwidth 0

    label .right.color -text "White to Move" -textvariable moveColorText

    label .right.score -text "White 2 Black 2" -textvariable scoreText

    button .right.new_game -text "New Game" -command newGame

    pack .right.color .right.score .right.new_game -side top

    pack .right -side right
  } \
  else {
    global sep_canvas info_canvas

    frame .right -borderwidth 0

    set sep_canvas .right.sep_canvas

    canvas $sep_canvas -width 8 -height 512 \
           -borderwidth 0 -highlightthickness 0

    bind $sep_canvas <Expose> drawSep

    pack $sep_canvas -side left

    set info_canvas .right.info_canvas

    canvas $info_canvas -width 128 -height 512 \
           -borderwidth 0 -highlightthickness 0

    bind $info_canvas <Expose> drawInfo
    bind $info_canvas <Button-1> "canvasInfoSelect %x %y"

    pack $info_canvas -side left

    pack .right -side right
  }
}

proc loadImages { } {
  global is_image

  if {$is_image} {
    global vbar_image control_image logo_image
    global no_piece_image white_piece_image black_piece_image
    global big_button_image button_image small_button_image
    global divider_image left_image right_image

    set vbar_image         [image create photo -file vbar.ppm]
    set control_image      [image create photo -file control.ppm]
    set logo_image         [image create photo -file logo.ppm]
    set no_piece_image     [image create photo -file tile.ppm]
    set white_piece_image  [image create photo -file white_piece.ppm]
    set black_piece_image  [image create photo -file blue_piece.ppm]
    set divider_image      [image create photo -file divider.ppm]
    set big_button_image   [image create photo -file big_button.ppm]
    set button_image       [image create photo -file button.ppm]
    set small_button_image [image create photo -file small_button.ppm]
    set left_image         [image create photo -file left.ppm]
    set right_image        [image create photo -file right.ppm]
  }
}

proc newGame { } {
  global moveColor

  CTclOthelloInitBoard

  drawBoard

  setMoveColor "White"

  setScore
}

proc setWhitePlayer { player } {
  gset whitePlayer $player
}

proc setBlackPlayer { player } {
  gset blackPlayer $player
}

proc getCurrentPlayer { } {
  global moveColor whitePlayer blackPlayer

  if     {$moveColor == "White"} {
    return $whitePlayer
  } \
  elseif {$moveColor == "Black"} {
    return $blackPlayer
  }
  else {
    return "None"
  }
}

proc setComputerLevel { level } {
  gset computerLevel $level
}

proc setMoveColor { color } {
  gset moveColor $color

  gset moveColorText "$color to Move"

  drawMoveColor
}

proc setCurrentPlayer { color } {
  global currentPlayer

  set currentPlayer $color
}

proc setScore { } {
  global scoreText

  set numWhite [CTclOthelloGetNumWhite]
  set numBlack [CTclOthelloGetNumBlack]

  set scoreText "White $numWhite Black $numBlack"

  drawScore
}

proc canvasSelect { x y } {
  global cellSize moveColor

  set player [getCurrentPlayer]

  if {$player == "Computer"} return

  set x1 [expr $x / $cellSize]
  set y1 [expr $y / $cellSize]

  if {[CTclOthelloCanMove $x1 $y1 $moveColor]} {
    CTclOthelloDoMove $x1 $y1 $moveColor

    setScore

    nextMoveColor

    drawBoard

    computerMove
  }
}

proc computerMove { } {
  global moveColor computerLevel

  set player [getCurrentPlayer]

  while {$player == "Computer"} {
    after 250

    if {! [CTclOthelloGetBestMove $moveColor $computerLevel best_x best_y]} {
      break
    }

    global best_x
    global best_y

    CTclOthelloDoMove $best_x $best_y $moveColor

    setScore

    nextMoveColor

    drawBoard

    set player [getCurrentPlayer]
  }
}

proc canvasInfoSelect { x y } {
  global info_canvas_cb info_canvas_cb_proc
  global info_canvas_cb_xmin info_canvas_cb_ymin 
  global info_canvas_cb_xmax info_canvas_cb_ymax 

  foreach cb $info_canvas_cb {
    set xmin $info_canvas_cb_xmin($cb)
    set ymin $info_canvas_cb_ymin($cb)
    set xmax $info_canvas_cb_xmax($cb)
    set ymax $info_canvas_cb_ymax($cb)

    if {$x >= $xmin && $x <= $xmax && $y >= $ymin && $y <= $ymax} {
      set proc $info_canvas_cb_proc($cb)

      $proc
    }
  }
}

proc drawBoard { } {
  global canvas is_image
  global cellSize pieceSize pieceBorder

  $canvas delete white
  $canvas delete black
  $canvas delete line

  if {$is_image} {
    set cellSize    64
    set pieceSize   64
    set pieceBorder 0
  } \
  else {
    set canvas_width  [winfo width  $canvas]
    set canvas_height [winfo height $canvas]

    if {$canvas_width < $canvas_height} {
      set cellSize [expr $canvas_width  / 8]
    } \
    else {
      set cellSize [expr $canvas_height / 8]
    }

    set pieceSize [expr $cellSize * 0.9 ]

    set pieceBorder [expr ($cellSize - $pieceSize) / 2.0]
  }

  if {! $is_image} {
    set x 0

    for {set i 0} {$i < 8} {incr i} {
      $canvas create line $x 0 $x [expr 8 * $cellSize] -tag line

      set x [expr $x + $cellSize]
    }

    set y 0

    for {set i 0} {$i < 8} {incr i} {
      $canvas create line 0 $y [expr 8 * $cellSize] $y -tag line

      set y [expr $y + $cellSize]
    }
  }

  set y 0

  for {set i 0} {$i < 8} {incr i} {
    set x 0

    for {set j 0} {$j < 8} {incr j} {
      if     {[CTclOthelloIsWhitePiece $j $i]} {
        drawWhitePiece $x $y
      } \
      elseif {[CTclOthelloIsBlackPiece $j $i]} {
        drawBlackPiece $x $y
      } \
      else {
        drawNoPiece $x $y
      }

      set x [expr $x + $cellSize]
    }

    set y [expr $y + $cellSize]
  }

  update
}

proc drawNoPiece { x y } {
  global canvas is_image

  if {$is_image} {
    global no_piece_image

    $canvas create image $x $y -image $no_piece_image -anchor nw
  }
}

proc drawWhitePiece { x y } {
  global canvas is_image

  if {$is_image} {
    global white_piece_image

    $canvas create image $x $y -image $white_piece_image -anchor nw
  } \
  else {
    global pieceSize pieceBorder

    set x1 [expr $x  + $pieceBorder]
    set y1 [expr $y  + $pieceBorder]
    set x2 [expr $x1 + $pieceSize]
    set y2 [expr $y1 + $pieceSize]

    $canvas create oval $x1 $y1 $x2 $y2 -tag white -fill white
  }
}

proc drawBlackPiece { x y } {
  global canvas is_image

  if {$is_image} {
    global black_piece_image

    $canvas create image $x $y -image $black_piece_image -anchor nw
  } \
  else {
    global pieceSize pieceBorder

    set x1 [expr $x  + $pieceBorder]
    set y1 [expr $y  + $pieceBorder]
    set x2 [expr $x1 + $pieceSize]
    set y2 [expr $y1 + $pieceSize]

    $canvas create oval $x1 $y1 $x2 $y2 -tag black -fill black
  }
}

proc drawSep { } {
  global sep_canvas vbar_image

  $sep_canvas delete vbar

  $sep_canvas create image 0 0 -image $vbar_image -anchor nw -tag vbar
}

proc drawInfo { } {
  gset info_canvas_cb ""

  global info_canvas control_image logo_image
  global big_button_image button_image small_button_image
  global divider_image left_image right_image

  $info_canvas delete info

  canvasCreateImage  0   0 $control_image    info
  canvasCreateImage  0   0 $logo_image       info
  canvasCreateImage  0  60 $divider_image    info
  canvasCreateImage  0 290 $divider_image    info
  canvasCreateImage  0 455 $divider_image    info

  $info_canvas delete text_info

  canvasCreateText 8 140 "White" text_info
  canvasCreateText 8 180 "Black" text_info

  canvasCreateButton  4  80 120 20 $big_button_image \
                     current_player_button current_player_cb

  canvasCreateButton 55 130  70 20 $button_image \
                     white_player_button white_player_cb
  canvasCreateButton 55 170  70 20 $button_image \
                     black_player_button black_player_cb

  canvasCreateButton 10 220 20 20 $left_image \
                     level_down_button level_down_cb
  canvasCreateButton 95 220 20 20 $right_image \
                     level_up_button level_up_cb

  canvasCreateTextButton  2 470 70 20 $button_image \
                          4 480 "New Game" \
                         new_game_button new_game_button_cb
  canvasCreateTextButton 76 470 50 20 $small_button_image \
                         88 480 "Quit" \
                         quit_button quit_cb

  $info_canvas raise text_info

  drawMoveColor

  drawPlayers

  drawLevel

  drawScore
}

proc drawMoveColor { } {
  global info_canvas is_image

  if {[info exists info_canvas] && $is_image} {
    global moveColorText

    $info_canvas delete move_color

    canvasCreateText 8 90 $moveColorText move_color

    $info_canvas raise move_color
  }
}

proc drawPlayers { } {
  global info_canvas is_image

  if {[info exists info_canvas] && $is_image} {
    global whitePlayer blackPlayer

    $info_canvas delete player

    canvasCreateText 60 140 $whitePlayer player
    canvasCreateText 60 180 $blackPlayer player

    $info_canvas raise player
  }
}

proc drawLevel { } {
  global info_canvas is_image

  if {[info exists info_canvas] && $is_image} {
    global computerLevel

    $info_canvas delete level

    set levelText "Level $computerLevel"

    canvasCreateText 40 230 $levelText level

    $info_canvas raise level
  }
}

proc drawScore { } {
  global info_canvas is_image

  if {[info exists info_canvas] && $is_image} {
    global scoreText

    $info_canvas delete score

    canvasCreateText 18 330 $scoreText score

    $info_canvas raise score
  }
}

proc canvasCreateButton { ix iy wx wy image tag proc } {
  global info_canvas

  $info_canvas delete $tag

  canvasCreateImage $ix $iy $image $tag

  $info_canvas raise $tag

  global info_canvas_cb info_canvas_cb_proc
  global info_canvas_cb_xmin info_canvas_cb_ymin
  global info_canvas_cb_xmax info_canvas_cb_ymax

  lappend info_canvas_cb $tag

  set info_canvas_cb_proc($tag) $proc
  set info_canvas_cb_xmin($tag) $ix
  set info_canvas_cb_ymin($tag) $iy
  set info_canvas_cb_xmax($tag) [expr $ix + $wx]
  set info_canvas_cb_ymax($tag) [expr $iy + $wy]
}

proc canvasCreateTextButton { ix iy wx wy image tx ty text tag proc } {
  global info_canvas

  $info_canvas delete $tag

  canvasCreateImage $ix $iy $image $tag

  canvasCreateText $tx $ty $text $tag

  $info_canvas raise $tag

  global info_canvas_cb info_canvas_cb_proc
  global info_canvas_cb_xmin info_canvas_cb_ymin
  global info_canvas_cb_xmax info_canvas_cb_ymax

  lappend info_canvas_cb $tag

  set info_canvas_cb_proc($tag) $proc
  set info_canvas_cb_xmin($tag) $ix
  set info_canvas_cb_ymin($tag) $iy
  set info_canvas_cb_xmax($tag) [expr $ix + $wx]
  set info_canvas_cb_ymax($tag) [expr $iy + $wy]
}

proc canvasCreateText { x y text tag } {
  global info_canvas

  set font *adobe*helvetica*bold-r-normal*12*

  set x1 [expr $x - 1]
  set y1 [expr $y - 1]

  $info_canvas create text $x1 $y1 -text $text -tag $tag \
               -fill grey70 -justify left -anchor w -font $font
  $info_canvas create text $x $y -text $text -tag $tag \
               -fill black -justify left -anchor w -font $font
}

proc canvasCreateImage { x y name tag } {
  global info_canvas

  $info_canvas create image $x $y -image $name -anchor nw -tag $tag
}

proc nextMoveColor { } {
  global moveColor

  setMoveColor [otherColor $moveColor]

  if { ! [CTclOthelloCanMoveAnywhere $moveColor] } {
    setMoveColor [otherColor $moveColor]

    if { ! [CTclOthelloCanMoveAnywhere $moveColor] } {
      gameOver
    }
  }
}

proc gameOver { } {
  global scoreText

  set numWhite [CTclOthelloGetNumWhite]
  set numBlack [CTclOthelloGetNumBlack]

  if     {$numWhite > $numBlack } {
    set scoreText "White Wins"
  } \
  elseif {$numBlack > $numWhite } {
    set scoreText "Black Wins"
  } \
  else {
    set scoreText "Draw"
  }

  drawScore
}

proc otherPlayer { } {
  global currentPlayer

  set currentPlayer [otherColor $currentPlayer]
}

proc otherColor { color } {
  if {$color == "White"} {
    return "Black"
  } \
  else {
    return "White"
  }
}

proc otherPlayer { player } {
  if {$player == "Human"} {
    return "Computer"
  } \
  else {
    return "Human"
  }
}

proc current_player_cb { } {
  set num [CTclOthelloGetNum]

  if {$num == 4} {
    global moveColor

    setMoveColor [otherColor $moveColor]

    computerMove
  }
}

proc white_player_cb { } {
  global whitePlayer

  set whitePlayer [otherPlayer $whitePlayer]

  drawPlayers

  computerMove
}

proc black_player_cb { } {
  global blackPlayer

  set blackPlayer [otherPlayer $blackPlayer]

  drawPlayers

  computerMove
}

proc level_down_cb { } {
  global computerLevel

  if {$computerLevel > 1} {
    incr computerLevel -1
  }

  drawLevel
}

proc level_up_cb { } {
  global computerLevel

  if {$computerLevel < 10} {
    incr computerLevel
  }

  drawLevel
}

proc new_game_button_cb { } {
  newGame
}

proc quit_cb { } {
  exit
}

gset is_image 1

CTclOthelloInitBoard

setWhitePlayer "Human"
setBlackPlayer "Computer"

setComputerLevel 3

setMoveColor "White"

createWidgets

newGame
