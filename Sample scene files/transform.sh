#!/bin/sh

for i in `ls *[57]*.test`
do
  echo "Working on $i ..."
  python2.7 "Transcode scenefile.py" $i $i.out
done
