#!/usr/bin/python
# -*- coding: utf-8 -*-

# ./bigramsWord.py books/neverendingStory.txt | tee /dev/stderr | say

import sys
import random

# print all bigrams
def main():
    # 1. learn model
    model = {}
    # skip program name
    for arg in sys.argv[1:]:
        model = allBigrams(model,arg)
    # 2. generate
    longest = ''
    for i in range(0, 5):
        state = 'START'
        word = ''
        while state != 'END':
            state = step(model, state)
            if not state=='END':
                word += state
        if max(len(word),len(longest)) == len(word):
            longest = word
    print longest

def step(model, state):
    nextStates = model[state].items()
    return weighted_choice(nextStates)

def weighted_choice(choices):
    total = sum(w for word, w in choices)
    r = random.uniform(0, total)
    upto = 0
    for word, w in choices:
       if upto + w > r:
          return word#, w
       upto += w
    assert False, "Shouldn't get here"

def allBigrams(bigrams,fname):
    with open(fname) as f:
        p = '' # previous
        c = ''
        for line in f:
            for word in line.split():
                for c in word:
                    if c != ',' and c != '"' and c != '\'' and c != '.' and c != ':': 
                      if p == '' or p == ' ': # first word!
                        bigrams = addBigram(bigrams, 'START', c)
                      else: 
                        if c == ' ': # new word!
                            bigrams = addBigram(bigrams, p, 'END')
                        else: # new character!
                            bigrams = addBigram(bigrams, p, c)
                      p = c
                bigrams = addBigram(bigrams, p, 'END')
                p = ''
        bigrams = addBigram(bigrams, c, 'END')
    return bigrams

def addBigram(bigrams, first, second):
    if not first in bigrams:
        bigrams[first] = {}
    if not second in bigrams[first]:
        bigrams[first][second] = 1
    else:
        bigrams[first][second] = bigrams[first][second]+1
    return bigrams
 
if __name__ == "__main__":
    main()
