#!/bin/sh

set -x

python genfont.py " 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" output/
#python genfont.py --size 20x30 TAB output/
