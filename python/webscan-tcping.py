#!/usr/bin/env python
# -*- coding: utf-8 -*-

from timeit import default_timer as timer
import netaddr
import sys
import socket
import threading
import os
import time

timeout = 0.21
retn = 3
pmval = 200
thdn = 500
#ipslist = ['172.253.0.0/16', '172.217.0.0/16', '172.110.32.0/21', '216.239.32.0/19', '216.73.80.0/20', '216.58.192.0/19', '209.85.128.0/17', '208.81.188.0/22', '208.68.108.0/22', '207.223.160.0/20', '199.223.232.0/21', '199.192.112.0/22', '173.194.0.0/16']
ipslist = ['1.1.1.0/24', '1.0.0.0/24', '173.245.48.0/20', '103.21.244.0/22', '103.22.200.0/22', '103.31.4.0/22', '141.101.64.0/18', '108.162.192.0/18', '190.93.240.0/20', '188.114.96.0/20', '197.234.240.0/22', '198.41.128.0/17', '162.158.0.0/15', '104.16.0.0/12', '172.64.0.0/13', '131.0.72.0/22']

class Socket(object):
	def __init__(self, family, type_, timeout):
		s = socket.socket(family, type_)
		s.settimeout(timeout)
		self._s = s

	def connect(self, host, port=80):
		self._s.connect((host, int(port)))

	def shutdown(self):
		self._s.shutdown(socket.SHUT_RD)

	def close(self):
		self._s.close()

def getpmin(host, port):
	pmin = timeout * 1000
	for i in range(0, retn):
		s = Socket(socket.AF_INET, socket.SOCK_STREAM, timeout)
		try:
			stime = timer()
			s.connect('%s'%host, port)
			s.shutdown()
			etime = timer()
			ctime = etime - stime
			rtime = int(ctime * 1000)
			pmin = min(pmin, rtime)
			if pmin <= pmval:
				s.close()
				return pmin
		except socket.timeout:
			pass
		finally:
			s.close()
	return pmin

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
		tm = getpmin(nowip, 443)
		if tm<=pmval:
			print('%s --- %dms'%(nowip, tm))
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
	time.sleep(0.1)
	threads = []
	for i in range(0, thdn):
		thread = scanThread(i)
		thread.start()
		threads.append(thread)
	for thread in threads:
		thread.join()
	print('Scanned %d/%d IP(s)'%(scncnt, tiplen))
	print('All the process have finished.')
	