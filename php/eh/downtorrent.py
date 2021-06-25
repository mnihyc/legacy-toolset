# -*- coding: UTF-8 -*-
# downtorrent.py code by mnihyc at 27(8:00pm - 6:00am), July, 2019
from urllib import request
from urllib import error
from urllib import parse
from http import cookiejar
import getpass
import sys
import os

def downtorrent(fpath):
	headers = {
		'User-Agent': r'Mozilla/5.0 (Windows NT 6.1; WOW64) Gecko/20100101 Firefox/68.0',
		'Connection': 'keep-alive'
	}
	
	lines = 0
	with open(fpath + '/url.txt','r',encoding='UTF-8')as f:
		for url in f:
			if len(url.strip())==0:
				continue
			lines = lines + 1
			print('In {0} line {1}: '.format(fpath,str(lines)),end='')
			req = request.Request(url=url, headers=headers)
			try:
				response = request.urlopen(req)
			except:
				print('The page is lost !')
				continue
			html = response.read().decode('utf-8')
			pos = html.find('<h1 id="gj">')
			if pos==-1:
				pos = html.find('<h1 id="gn">')
			if pos==-1:
				print('Broken web-page !')
			pos = pos + len('<h1 id="gj">')
			rpos = html.find('</h1>', pos)
			title = html[pos:rpos]
			illegel = ['\\','/',':','*','?','"','<','>','|']
			for ilc in illegel:
				title = title.replace(ilc, ' ')
			
			pos = html.find('onclick="return popUp(\'https://e-hentai.org/gallerytorrents.php?')
			if pos==-1:
				print('No torrent found, skipped !')
				continue
			pos = pos + len('onclick="return popUp(\'')
			rpos = html.find('\',', pos)
			nurl = html[pos:rpos]
			
			dirname = fpath + '/' + title + '/'
			if os.path.exists(dirname):
				print('Directory exists, skipped !')
				continue
			else:
				os.makedirs(dirname)
			
			nurl = nurl.replace('&amp;', '&')
			req = request.Request(url=nurl, headers=headers)
			response = request.urlopen(req)
			html = response.read().decode('utf-8')
			pos = html.find('<a href="https://ehtracker.org/get/')
			if pos==-1:
				print('Unexpected error occurs !')
				continue
			pos = pos + len('<a href="')
			rpos = html.find('" onclick="', pos)
			nurl = html[pos:rpos]
			req = request.Request(url=nurl, headers=headers)
			response = request.urlopen(req)
			data = response.read()
			with open(dirname + '/download.torrent', 'wb') as f:
				f.write(data)
			print('Success !')
	
	
def walkpath():
	path = os.getcwd() + '/data/'
	for fpath, dirnames, fnames in os.walk(path):
		for dirname in dirnames:
			downtorrent(fpath + '/' + dirname)
	
	
if __name__=='__main__':
	walkpath()

input('Press enter to exit...')