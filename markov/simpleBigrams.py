#!/usr/bin/python

import sys

fname = sys.argv[1]
previous = ''
with open(fname) as f:
    for line in f:
        for word in line.split():
            if previous != '':
              print previous, word
            previous = word
