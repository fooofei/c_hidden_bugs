#coding=utf-8

import os
import sys
import time
import shutil

def entry():
  p = './dir_test_mkdir'
  while 1:
    if os.path.exists(p):
      shutil.rmtree(p)
    time.sleep(1)

if __name__ == '__main__':
    entry()
