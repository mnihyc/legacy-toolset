#!/usr/bin/env python
# -*- coding: utf-8 -*-

import requests
import netaddr
import sys
import threading
import os
import time

timeout = 1
retry = 3
thdn = 2000
rgip = '112.5.37.243'
ipslist = ['1.1.1.0/24', '1.0.0.0/24', '173.245.48.0/20', '103.21.244.0/22', '103.22.200.0/22', '103.31.4.0/22', '141.101.64.0/18', '108.162.192.0/18', '190.93.240.0/20', '188.114.96.0/20', '197.234.240.0/22', '198.41.128.0/17', '162.158.0.0/15', '104.16.0.0/12', '172.64.0.0/13', '131.0.72.0/22']

class scanThread(threading.Thread):
	def __init__(self, tidx):
		threading.Thread.__init__(self)
		self.tidx = tidx
	def run(self):
		scanthread(self.tidx)

tip = []
tiplen = scncnt = lasttime = avers = starttime = 0
def scanthread(tidx):
	times = 0
	while True:
		nowipidx = thdn*times + tidx
		if nowipidx>=tiplen:
			return
		nowip = tip[nowipidx]
		if lock.acquire():
			try:
				global scncnt, lasttime, avers
				scncnt = scncnt + 1
				if time.time()-lasttime > 0.2:
					avers = scncnt/(time.time()-starttime)
					os.system('title %.2f%% (%d/%d) ~%d/s threads %d/%d'%(scncnt/tiplen*100, scncnt, tiplen, avers, threading.activeCount(), thdn))
					lasttime = time.time()
			finally:
				lock.release()
		gip = rgip
		for i in range(0, retry):
			try:
				res = requests.get('http://%s/cdn-cgi/trace'%nowip, timeout=timeout)
				gip = res.text[res.text.index('ip=')+3 : res.text.index('ts=')-1]
				break
			except:
				gip = rgip
		if gip!=rgip:
			print('%s --- %s'%(nowip, gip))
		times = times + 1

if __name__ == '__main__':
	print('Preparing to start ...')
	for ips in ipslist:
		iplist = list(netaddr.IPNetwork(ips))
		for ip in iplist:
			tip.append(ip)
			tiplen = tiplen + 1
	lock = threading.Lock()
	starttime = time.time()
	threads = []
	for i in range(0, thdn):
		thread = scanThread(i)
		thread.start()
		threads.append(thread)
	for thread in threads:
		thread.join()
	print('Scanned %d/%d IP(s)'%(scncnt, tiplen))
	print('All the process have finished.')
	