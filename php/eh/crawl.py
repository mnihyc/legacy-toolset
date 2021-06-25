# -*- coding: UTF-8 -*-
# crawl.py code by mnihyc at 27(8:00pm - 6:00am), July, 2019
from urllib import request
from urllib import error
from urllib import parse
from http import cookiejar
import getpass
import sys
import os

def crawl():
	print('If you don\'t use a proxy, press [Enter] to skip this.');
	proxy_type = input('Input the proxy type(socks5, http, ...): ')
	if len(proxy_type)>0 :
		proxy_ip = input('Input the proxy address(ip:port): ')
		proxy_support = request.ProxyHandler({proxy_type: proxy_ip})
		opener = request.build_opener(proxy_support)
		request.install_opener(opener)
	headers = {
		'Referer': 'https://forums.e-hentai.org/index.php',
		'User-Agent': r'Mozilla/5.0 (Windows NT 6.1; WOW64) Gecko/20100101 Firefox/68.0',
		'Connection': 'keep-alive'
	}
	login_url = r'https://forums.e-hentai.org/index.php?act=Login&CODE=01'
	username = input('Input your username: ')
	password = getpass.getpass('Input your password: ')
	postdata = {
		'UserName': username,
		'PassWord': password,
		'CookieDate': '1',
		'Privacy': '1',
		'referer': 'https://forums.e-hentai.org/index.php?',
		'b': '',
		'bt': ''
	}
	postdata = parse.urlencode(postdata).encode('utf-8')
	cookie = cookiejar.CookieJar()
	cookie_support = request.HTTPCookieProcessor(cookie)
	opener = request.build_opener(cookie_support)
	reqlogin = request.Request(url=login_url, data=postdata, headers=headers)
	response = opener.open(reqlogin)
	html = response.read().decode('utf-8')
	errortext = 'Username or password incorrect'
	if html.find(errortext)!=-1 :
		input('The user/pass is wrong!')
		sys.exit(0)
	recaptchatext = 'The captcha was not entered correctly.'
	if html.find(recaptchatext)!=-1 :
		print('Please login at {0} and input the following cookie!'.format(login_url))
		cookie_ipb_session_id=input('ipb_session_id = ')
		cookie_ipb_member_id=input('ipb_member_id = ')
		cookie_ipb_pass_hash=input('ipb_pass_hash = ')
		headers['Cookie'] = 'ipb_session_id={0};ipb_member_id={1};ipb_pass_hash={2}'.format(
			cookie_ipb_session_id, cookie_ipb_member_id, cookie_ipb_pass_hash)
	reqlogin = request.Request(url=login_url, data=None, headers=headers)
	response = opener.open(reqlogin)
	html = response.read().decode('utf-8')
	succtext = 'Logged in as:'
	if html.find(succtext)==-1 :
		input("Couldn't access your account!")
		sys.exit(0)
	
	print('Login successfully!')
	
	for which in range(10):
		url = 'https://e-hentai.org/favorites.php?favcat=' + str(which)
		req = request.Request(url=url, data=None, headers=headers)
		response = opener.open(req)
		html = response.read().decode('utf-8')
		lposstr = '<p class="ip">Showing '
		lpos = html.find(lposstr) + len(lposstr)
		rposstr = ' results</p>'
		rpos = html.find(rposstr)
		if html.find(lposstr)!=-1:
			print('Find {0} results in favorite {1}'.format(html[lpos:rpos],str(which)))
		else:
			print('Find {0} results in favorite {1}'.format('0',str(which)))
			continue
		result = ''
		restitle = ''
		page = 0
		posstr = r'<a href="https://e-hentai.org/favorites.php?page='
		posendstr = r'" onclick="return false">&gt;</a></td></tr></table>'
		while True:
			print('Print results in favorite {0} page {1}'.format(str(which),str(page)))
			imgstr = '<a href="https://e-hentai.org/g/'
			pos = html.find(imgstr)
			while pos!=-1:
				pos = pos + len('<a href="')
				rpos = html.find('">', pos)
				result = result + html[pos:rpos] + '\r\n'
				rpos = rpos + len('"><div class="glink">')
				rrpos = html.find('</div>', rpos)
				restitle = restitle + html[rpos:rrpos] + '\r\n'
				pos = html.find(imgstr, pos)
			
			lpos = html.rfind(posstr)
			rpos = html.find(posendstr,lpos)
			if lpos==-1 or rpos==-1:
				break
			lpos = lpos + len('<a href="')
			page = page + 1
			url = html[lpos:rpos].replace('&amp;','&')
			req = request.Request(url=url, data=None, headers=headers)
			response = opener.open(req)
			html = response.read().decode('utf-8')
		dirname = os.getcwd() + '/data/favorite{0}/'.format(str(which))
		filename = dirname + 'url.txt'
		print('Write to {0}'.format(filename))
		if not os.path.exists(dirname):
			os.makedirs(dirname)
		with open(filename,'w',encoding='UTF-8')as f:
			f.write(result)
		filename = dirname + 'name.txt'
		with open(filename,'w',encoding='UTF-8')as f:
			f.write(restitle)
		print('Finished !')
	
	print('All finished !')	

	
if __name__=='__main__':
	crawl()

input('Press enter to exit...')
