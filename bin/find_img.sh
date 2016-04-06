#!/bin/bash

echo "Usage: find_img.sh <database>"
echo "\tDisplay the image most similar to the one under the cursor"

#Sets $X $Y $SCREEN $WINDOW
eval $(xdotool getmouselocation --shell)
echo "Looking for a card at: $X $Y"

SCREENSHOT=/tmp/_screenshot.bmp
scrot $SCREENSHOT

DATASET=$1

CARD=$(~/Programming/RevImgSearch/bin/search $DATASET $SCREENSHOT $X $Y)

eog "$CARD"
