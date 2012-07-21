#!/bin/sh

set -x

python genfont.py --pointsize 12 --shave 20x0% --font Courier-New " 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" output/
#python genfont.py --size 20x30 TAB output/
