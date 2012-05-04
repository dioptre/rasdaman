#!/usr/bin/env python

import time

from threading import Thread, enumerate
from urllib import urlopen

LAYER = "australia_wms"
SRS = "EPSG:4326"
STYLE = "standard"
BBOX = "110,-40,120,-30"
WIDTH = "900"
HEIGHT = "900"
URI = "http://localhost:8080/petascope/wms?VERSION=1.1.0&SERVICE=WMS&REQUEST=GetMap&SRS=" + SRS + "&LAYERS=" + LAYER + "&STYLES=" + STYLE + "&BBOX=" + BBOX + "&WIDTH=" + WIDTH + "&HEIGHT=" + HEIGHT + "&FORMAT=image/png&EXCEPTIONS=application/vnd.ogc.se_xml"
CTYPE = "image/png"
COUNT = 100

UPDATE_INTERVAL = 0.01

class URLThread(Thread):
    
    def __init__(self, url, expected_ctype):
        super(URLThread, self).__init__()
        self.url = url
        self.expected_ctype = expected_ctype
        self.request = None
        self.response = None
        self.time = 0
        self.got_tile = False
        self.error = False

    def run(self):
        start = time.time()
        self.request = urlopen(self.url)
        self.response = self.request.read()
        self.time = time.time() - start
        try:
            if self.request.info().get("Content-Type")==self.expected_ctype:
                self.got_tile = True
        except TypeError: pass


def stress(uri, ctype, count=10, timeout=2.0):
    def alive_count(lst):
        alive = map(lambda x : 1 if x.isAlive() else 0, lst)
        return reduce(lambda a,b : a + b, alive)
    threads = [ URLThread(uri, ctype) for i in range(count) ]
    for thread in threads:
        thread.start()
        time.sleep(0.1)
    while alive_count(threads) > 0 and timeout > 0.0:
        timeout = timeout - UPDATE_INTERVAL
        time.sleep(UPDATE_INTERVAL)
    return [ {'got_tile': x.got_tile, 'time': x.time} for x in threads ]

times = [x['time'] for x in stress(URI, CTYPE, COUNT) if x['got_tile']]

print '%d%% successful of %d requests' % ((len(times)*100/COUNT), COUNT)
if len(times)>0:
    print 'avg. time for successful requests %d ms' % (sum(times)*1000/len(times))
