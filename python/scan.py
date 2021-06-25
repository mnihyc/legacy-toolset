#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import time
import sys
import netaddr
from timeit import default_timer as timer

ipslist = ['104.16.0.0/12']

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
	pmin = 1000
	for i in range(3):
		s = Socket(socket.AF_INET, socket.SOCK_STREAM, 0.3)
		try:
			stime = timer()
			s.connect('%s'%host, port)
			s.shutdown()
			etime = timer()
			ctime = etime - stime
			rtime = int(ctime * 1000)
			print('%dms '%rtime, end='')
			sys.stdout.flush()
			pmin = min(pmin, rtime)
			if pmin < 80 :
				s.close()
				return pmin
		except socket.timeout:
			print('-1ms ', end='')
			sys.stdout.flush()
			pass
		finally:
			s.close()
	return pmin

if __name__ == '__main__':
	print('Checking ...')
	for ips in ipslist:
		iplist = list(netaddr.IPNetwork(ips))
		for ip in iplist:
			print('%s:80 ... '%ip, end='')
			esct = getpmin(ip, 80)
			if esct == 1000 :
				print('\r'+' '*119+'\r', end='')
				continue
			print('  %s:443 ... '%ip, end='')
			esct = getpmin(ip, 443)
			if esct < 80 or esct == 1000 :
				print('\r'+' '*119+'\r%s ... GOT (%d)'%(ip,esct))
			else:
				print('\r'+' '*119+'\r', end='')
			pass
