<?php

	define('DATA_DIR',  __DIR__.'/data/');
	define('DOWN_DIR', DATA_DIR.'down/');
	define('CREATE_TABLE_SQL', 'CREATE TABLE "data" ( 
		"url"  TEXT, 
		"belong"  TEXT, 
		"downway"  TEXT, "thumbnail"  TEXT, 
		"titlejap"  TEXT, "titleeng"  TEXT, 
		"type"  TEXT, "posttime"  TEXT, "filesize"  REAL, "pagenum"  INTEGER, "rating"  REAL, 
		"language"  TEXT, "postercomment"  TEXT, 
		"parodytag"  TEXT, "charactertag"  TEXT, "grouptag"  TEXT, 
		"artisttag"  TEXT, "maletag"  TEXT, "femaletag"  TEXT, "alltag"  TEXT, 
		"downtime"  INTEGER, "updatetime"  INTEGER);');
	define('CREATE_TABLE_SQL1', 'CREATE TABLE "config" ( 
		"username"  TEXT, 
		"igneous"  TEXT, "ipb_member_id"  TEXT, "ipb_pass_hash"  TEXT, 
		"proxy_type"  TEXT, "proxy_url"  TEXT, 
		"downway"  TEXT, "clientid"  TEXT, "clientkey"  TEXT, 
		"lastcrawl"  INTEGER, "lastdown"  INTEGER);');
	define('CURL_USER_AGENT', 'Mozilla/5.0 (Windows NT 6.1; WOW64) Gecko/20100101 Firefox/68.0');
	define('CURL_EXH_REFERERURL', 'https://forums.e-hentai.org/index.php');
	define('CURL_EXH_LOGINURL', 'https://forums.e-hentai.org/index.php?act=Login&CODE=01');
	define('CURL_EXH_FAILEDLOGIN_WRONG', 'Username or password incorrect');
	define('CURL_EXH_FAILEDLOGIN_CAPTCHA', 'The captcha was not entered correctly.');
	define('CURL_EXH_SUCCLOGIN', 'Logged in as:');
	define('CURL_EXH_MAINPAGE', 'https://exhentai.org/');
	define('CURL_EXH_MAINPAGE_FAILED','<img src="https://exhentai.org/img/kokomade.jpg" />');
	define('CURL_MAX_REQUESTS', 20); // IP will be blocked once this is set too high
	define('CURL_MAX_DOWNLOADS', 5); // Unknown error occurs when this is set too high, CLIENT_DLFETCH_REJECT
	define('CURL_EXH_HOMEURL', 'https://e-hentai.org/home.php');
	define('CURL_EXH_FAVURL', 'https://exhentai.org/favorites.php?favcat=');
	//define('IMAGE_LIMIT_MAX', 5000);
	$IMAGE_LIMIT_MAX = 5000;


	function GetContentBetween($str, $begin, $end)
	{
		$repstr = '\\*.?+$^[](){}|/';
		$rep = [];
		for($i=0; $i<strlen($repstr); $i++)
		{
			$begin = str_replace($repstr[$i], '\\'.$repstr[$i], $begin);
			$end = str_replace($repstr[$i], '\\'.$repstr[$i], $end);
		}
		preg_match_all("/{$begin}([\s\S]*?){$end}/u", $str, $arr);
		return $arr[1];
	}

	function InitializeRequest(&$page)
	{
		global $db;
		$res = $db->fetch('select * from config');
		if(!empty($res['proxy_type']) && !empty($res['proxy_url']))
			$page->setProxy($res['proxy_url'], $res['proxy_type']);
		$cookies = $page->getCookies();
		if(!empty($res['igneous']))
			$cookies['igneous'] = $res['igneous'];
		if(!empty($res['ipb_member_id']))
			$cookies['ipb_member_id'] = $res['ipb_member_id'];
		if(!empty($res['ipb_pass_hash']))
			$cookies['ipb_pass_hash'] = $res['ipb_pass_hash'];
		$page->setCookies($cookies);
		$page->setUserAgent(CURL_USER_AGENT);
	}

	function SetConfigs($type='')
	{
		global $db;
		
		/* Set proxy for local debugging */
		if(empty($type) || $type==='proxy')
		{
			$opt = $db->fetchColumn('select proxy_type from config');
			echo "[*] It's highly recommended to use [socks5h] to prevent DNS pollution.\n";
			while($proxytype!=='http' && $proxytype!=='socks4' && $proxytype!=='socks4a' && 
				 $proxytype!=='socks5' && $proxytype!=='socks5h' && $proxytype!=='https')
			{
				echo "Input proxy type: ({$opt})[http,socks4(a),socks5(h),https] ";
				fwrite(STDIN, 'socks5h'); fflush(STDOUT);
				$proxytype = strtolower(trim(getInput('proxytype')));
			}
			$opu = $db->fetchColumn('select proxy_url from config');
			echo "Input proxy url: ({$opu})[] ";
			$proxyurl = trim(getInput('proxyurl'));
			$db->update('config',[
				'proxy_type' => $proxytype,
				'proxy_url' => $proxyurl ]);
		}
		
		/* Get cookies for logging in */
		if(empty($type) || $type==='user')
		{
			resetuser:
			$ousn = $db->fetchColumn('select username from config');
			echo "Input your new username: ({$ousn})[] ";
			$username = trim(getInput('username'));
			$db->update('config', ['username' => $username]);
			while($method!=='password' && $method!=='cookie')
			{
				echo "Choose a login method: [password/cookie] ";
				$method = strtolower(trim(getInput('method')));
			}
			
			if(!empty($page))
				$page = null;
			$page = new Request;
			InitializeRequest($page);
			$page->setReferer(CURL_EXH_REFERERURL);
			if($method === 'password')
			{
				echo "Input your password(will not store): [] ";
				$password = getInput('password');
				$content = $page->post(CURL_EXH_LOGINURL, [
					'UserName' => $username,
					'PassWord' => $password,
					'CookieDate' => 1,
					'Privacy' => 1,
					'referer' => CURL_EXH_REFERERURL,
					'b' => '',
					'bt' => '' ]);
				if(strpos($content, CURL_EXH_FAILEDLOGIN_WRONG) !== false)
				{
					echo "[!] The inputted username/password is wrong!\n";
					echo "[-] Login failed...\n\n";
					goto resetuser;
				}
				if(strpos($content, CURL_EXH_FAILEDLOGIN_CAPTCHA) !== false)
				{
					echo "[!] A captcha is required for logging in...\n";
					echo "[!] Please use [cookie] method to login.\n\n";
					goto resetuser;
				}
			}
			else
			{
				echo "[*] Please login at '".CURL_EXH_LOGINURL."' and input the following cookies.\n";
				$org = $db->fetchColumn('select igneous from config');
				echo "Input igneous: ({$org})[()] ";
				$igneous = trim(getInput('igneous'));
				$org = $db->fetchColumn('select ipb_member_id from config');
				echo "Input ipb_member_id: ({$org})[] ";
				$ipb_member_id = trim(getInput('ipb_member_id'));
				$org = $db->fetchColumn('select ipb_pass_hash from config');
				echo "Input ipb_pass_hash: ({$org})[] ";
				$ipb_pass_hash = trim(getInput('ipb_pass_hash'));
				$page->setCookies([
					'igneous' => $igneous,
					'ipb_member_id' => $ipb_member_id,
					'ipb_pass_hash' => $ipb_pass_hash ]);
			}
			$content = $page->get(CURL_EXH_LOGINURL);
			if(strpos($content, CURL_EXH_SUCCLOGIN) === false)
			{
				echo "[-] Login failed, is the cookie correct?(or unexpected user/pass login failure)\n";
				echo "[-] Couldn't access your account...\n\n";
				goto resetuser;
			}
			echo "[+] Log in successfully!\n";
			$cookies = $page->getCookies();
			$db->update('config',[
				'igneous' => $cookies['igneous'],
				'ipb_member_id' => $cookies['ipb_member_id'],
				'ipb_pass_hash' => $cookies['ipb_pass_hash'] ]);
			
			$content = $page->get(CURL_EXH_MAINPAGE);
			if(strpos($content, CURL_EXH_MAINPAGE_FAILED) !== false)
			{
				echo "[-] Couldn't access the main page!\n";
				echo "[!] Re-try or login with cookie(set [igneous]) again!\n";
				$method = 'cookie';
				goto resetuser;
			}
		}
		
		if(empty($type) || $type==='down')
		{
			$opt = $db->fetchColumn('select downway from config');
			echo "[*] It's highly recommanded to use H@H(for downloading) if you have one.\n";
			echo "[*] Also, if you have a free archive quota, it's best to use it.\n";
			while($downway!=='page' && $downway!=='h@h' && $downway!=='archive')
			{
				echo "Input downloading way ({$opt})[page, h@h, archive] ";
				$downway = strtolower(trim(getInput('downway')));
			}
			$clientid = $db->fetchColumn('select clientid from config');
			$clientkey = $db->fetchColumn('select clientkey from config');
			if($downway === 'h@h')
			{
				echo "Input your clientid ({$clientid})[] ";
				$clientid = trim(getInput('clientid'));
				echo "Input your clientkey ({$clientkey})[] ";
				$clientkey = trim(getInput('clientkey'));
			}
			$db->update('config', [
				'downway' => $downway,
				'clientid' => $clientid,
				'clientkey' => $clientkey ]);
		}
		
	}

	function isBrowser()
	{
		return trim($_SERVER['QUERY_STRING'])!=='';
	}

	function diemsg($msg)
	{
		echo $msg."\n";
		echo "[x] Exiting...\n";
		if(isBrowser())
			echo "</pre>";
		die();
	}

	function getInput($varname)
	{
		if(isBrowser() && !$_GET[$varname])
			diemsg("\n[x] Input this variant in \$_GET......");
		else if(isBrowser())
			return $_GET[$varname];
		$str = fgets(STDIN);
		if(strpos($str, PHP_EOL) !== false)
			return substr($str, 0, strpos($str, PHP_EOL));
		else
			return $str;
	}

?>