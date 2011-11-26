# -*- encoding: utf8 -*-
from rockdb import RokDB
import time

def test():
    db = RokDB()
    s = 'IN table тесте асд INSERT bla\n'
    print ("Escrevendo:", s)
    db.raw_send(s)

if __name__ == '__main__':
    test() 
