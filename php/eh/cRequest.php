<?php
	include_once('cFunction.php');
	include_once('cProgBar.php');
	if(!extension_loaded('curl'))
		diemsg('[-] Needs libcurl being enabled!');
	class Request
	{
		private $ch = null; // curl handle
		private $proxy = null; // proxy
		private $cookies = array();
		public $status = null;
		private $pb = null;
		
		private function curlResponseHeaderCallback($ch, $headerLine)
		{
			if(preg_match('/^Set-Cookie:\s*([^;]*)/mi', $headerLine, $cookie) == 1)
				$this->cookies[substr($cookie[1], 0 ,strpos($cookie[1],'='))] = substr($cookie[1], strpos($cookie[1], '=')+1);
			return strlen($headerLine); // needed by curl
		}
		
		private function curlResponseProgressCallback($resource, $download_size, $downloaded, $upload_size, $uploaded)
		{
			if(version_compare(PHP_VERSION, '5.5.0') < 0)
			{
				$uploaded = $upload_size;
				$upload_size = $downloaded;
				$downloaded = $download_size;
				$download_size = $resource;
			}

			if($download_size > 0)
				$this->pb->setTotal($download_size / 1024)->setCurrent($downloaded / 1024)->draw();
		}
		
		public function __construct()
		{
			$this->ch = curl_init();
			curl_setopt($this->ch, CURLOPT_CONNECTTIMEOUT, 60);
			curl_setopt($this->ch, CURLOPT_TIMEOUT, 300);
			curl_setopt($this->ch, CURLOPT_RETURNTRANSFER, true);
			curl_setopt($this->ch, CURLOPT_HEADERFUNCTION, "Request::curlResponseHeaderCallback");
			curl_setopt($this->ch, CURLOPT_SSL_VERIFYPEER, false);
			curl_setopt($this->ch, CURLOPT_SSL_VERIFYHOST, false);
			curl_setopt($this->ch, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
			curl_setopt($this->ch, CURLOPT_FOLLOWLOCATION, true);
			curl_setopt($this->ch, CURLOPT_ENCODING, 'gzip,deflate');
		}
		
		public function __destruct()
		{
			curl_close($this->ch);
		}
		
		public function setOptions($opt, $val)
		{
			curl_setopt($this->ch, $opt, $val);
			return $this;
		}
		
		public function setHeader($header)
		{
			if(!is_array($header))
				$header = array($header);
			curl_setopt($this->ch, CURLOPT_HTTPHEADER, $header);
			return $this;
		}
		
		public function setTimeout($time)
		{
			curl_setopt($this->ch, CURLOPT_TIMEOUT, $time);
			return $this;
		}
		
		public function setProxy($proxy, $type='http', $userpass='')
		{
			curl_setopt($this->ch, CURLOPT_PROXY, $type.'://'.$proxy);
			switch(strtolower($type))
			{
				case 'http':
					curl_setopt($this->ch, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
					break;
				case 'socks':case 'socks4':
					curl_setopt($this->ch, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4);
					break;
				case 'socks4a':
					curl_setopt($this->ch, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4A);
					break;
				case 'socks5h':
					curl_setopt($this->ch, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);
					break;
				case'socks5h':
					curl_setopt($this->ch, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
					break;
				case 'https':
					curl_setopt($this->ch, CURLOPT_PROXYTYPE, CURLPROXY_HTTPS);
					break;
			}
			if(!empty($userpass))
				curl_setopt($this->ch, CURLOPT_PROXYUSERPWD, $userpass);
			return $this;
		}
		
		public function setReferer($referer = '')
		{
			if(!empty($referer))
				curl_setopt($this->ch, CURLOPT_REFERER, $referer);
			return $this;
		}
		
		public function setUserAgent($agent = '')
		{
			if(!empty($agent))
				curl_setopt($this->ch, CURLOPT_USERAGENT, $agent);
			return $this;
		}
		
		public function getCookies()
		{
			return $this->cookies;
		}
		
		public function getCookieString()
		{
			return http_build_query($this->cookies, '', '; ', PHP_QUERY_RFC3986);
		}
		
		public function setCookies($cookies)
		{
			if(is_array($cookies))
				$this->cookies = $cookies;
			else
			{
				$this->cookies = array();
				$cookies = explode(';', $cookies);
				foreach($cookies as $val)
				{
					$key = substr($val, 0, strpos($val, '='));
					$val = substr($val, strpos($val, '=')+1);
					$this->cookies[trim($key)] = trim($val);
				}
			}
			return $this;
		}
		
		public function getErrorMsg($ch)
		{
			return strval(curl_errno($ch)).': '.curl_error($ch);
		}
		
		public function get($url, $params=array())
		{
			if(!empty($params))
			{
				if(strpos($url, '?') === false)
					$url.='?';
				else
					$url.='&';
				if(is_array($params))
					$url.=http_build_query($params);
				else if(is_string($params))
					$url.=$params;
			}
			curl_setopt($this->ch, CURLOPT_POST, false);
			curl_setopt($this->ch, CURLOPT_URL, $url);
			if(!empty($this->cookies))
				curl_setopt($this->ch, CURLOPT_COOKIE, http_build_query($this->cookies, '', '; ', PHP_QUERY_RFC3986));
			$content = curl_exec($this->ch);
			$this->status = curl_getinfo($this->ch);
			if($content === false)
				diemsg('[x] cURL error: '.$this->getErrorMsg($this->ch));
			//$content = mb_convert_encoding($content, 'utf-8', 'GBK,GB2312,UTF-8,ASCII');
			return $content;
		}
		
		public function download($url, $file='', $prog=null)
		{
			curl_setopt($this->ch, CURLOPT_POST, false);
			curl_setopt($this->ch, CURLOPT_URL, $url);
			if(!empty($this->cookies))
				curl_setopt($this->ch, CURLOPT_COOKIE, http_build_query($this->cookies, '', '; ', PHP_QUERY_RFC3986));
			
			$ch = curl_copy_handle($this->ch);
			curl_setopt($ch, CURLOPT_TIMEOUT, 0);
			if(!empty($file))
			{
				$fp = fopen($file, 'wb');
				curl_setopt($ch, CURLOPT_FILE, $fp);
			}
			
			$pb = null;
			if(!is_null($prog))
			{
				$pb = new ProgressBar;
				$pb->setPreShow($prog)->draw();
				curl_setopt($ch, CURLOPT_NOPROGRESS, false);
				curl_setopt($ch, CURLOPT_PROGRESSFUNCTION, 'Request::curlResponseProgressCallback');
				$this->pb = $pb;
			}
			
			$bptime = microtime(true);
			@$content = curl_exec($ch);
			$this->status = curl_getinfo($ch);
			if($content === false)
			{
				if(!is_null($prog))
				{
					$pb->clear();
					echo "\r";
				}
				diemsg('[x] cURL error: '.$this->getErrorMsg($ch));
			}
			$tsize = $this->status['size_download'];
			
			if(!is_null($prog))
			{
				$pb->clear();
				echo "\r";
			}
			echo "Get 1 file in ".number_format(abs(microtime(true)-$bptime), 1)." secs, ".
				"total ".number_format($tsize/1024, 0)." KiByte, ".
				"speed ".number_format($tsize / 1024 / abs(microtime(true)-$bptime), 2)." KiB/s\n";
			
			curl_close($ch);
			if(!empty($file))
				fclose($fp);
			return $content;
		}
		
		public function multiget($urlarr=array(), $filearr=array(), $prog=null)
		{
			curl_setopt($this->ch, CURLOPT_POST, false);
			if(!empty($this->cookies))
				curl_setopt($this->ch, CURLOPT_COOKIE, http_build_query($this->cookies, '', '; ', PHP_QUERY_RFC3986));
			
			if(!is_array($urlarr))
				$urlarr = array($urlarr);
			if(!is_array($filearr))
				$filearr = array($filearr);
			$charr = [];
			foreach($urlarr as $idx => $url)
			{
				$charr[$idx] = curl_copy_handle($this->ch);
				curl_setopt($charr[$idx], CURLOPT_CONNECTTIMEOUT, 300);
				curl_setopt($charr[$idx], CURLOPT_TIMEOUT, 0);
				curl_setopt($charr[$idx], CURLOPT_PRIVATE, $idx);
				if(!empty($filearr[$idx]))
				{
					$fp = fopen($filearr[$idx], 'wb');
					$filehandle[$idx] = $fp;
					curl_setopt($charr[$idx], CURLOPT_FILE, $fp);
				}
				curl_setopt($charr[$idx], CURLOPT_URL, $url);
			}
			$mh = curl_multi_init();
			foreach($charr as $ch)
				curl_multi_add_handle($mh, $ch);
			
			$pb = null;
			if(!is_null($prog))
			{
				$pb = new ProgressBar;
				$pb->setTotal(count($urlarr))->setPreShow($prog)->draw();
			}
			
			$bptime = microtime(true);
			$cnt = $tsize = 0;
			do
			{
				while(($execrun=curl_multi_exec($mh, $active)) == CURLM_CALL_MULTI_PERFORM);
				if($execrun != CURLM_OK)
					break;
				while($done = curl_multi_info_read($mh))
				{
					if(!is_null($pb))
						$pb->setCurrent(++$cnt)->draw();
					$ch = $done['handle'];
					$tsize += intval(curl_getinfo($ch, CURLINFO_SIZE_DOWNLOAD));
					$k = curl_getinfo($ch, CURLINFO_PRIVATE);
					$res[$k] = curl_multi_getcontent($ch);
					if($res[$k] === false)
					{
						if(!is_null($prog))
							echo "\r";
						diemsg('[x] cURL error: '.$this->getErrorMsg($ch));
					}
					$status = curl_getinfo($ch);
					if(intval($status['http_code']) != 200)
					{
						echo "\n[-] Failed request to {$urlarr[$k]}. ({$status['http_code']})\n";
						echo '[-] cURL error: '.$this->getErrorMsg($ch)." \n";
					}
					if(!empty($filearr[$k]))
						$res[$k] = '';
					echo '.';
				}
				if($active && curl_multi_select($mh, 1)==-1)
					usleep(1000);
			}while($active);

			if(!is_null($prog))
			{
				$pb->clear();
				echo "\r";
			}
			echo "Get ".number_format($cnt)." files in ".number_format(abs(microtime(true)-$bptime), 1)." secs, ".
				"total ".number_format($tsize/1024, 0)." KiByte, ".
				"speed ".number_format($tsize / 1024 / abs(microtime(true)-$bptime), 2)." KiB/s\n";

			foreach($charr as $ch)
				curl_multi_remove_handle($mh, $ch);
			curl_multi_close($mh);
			if(!empty($filehandle))
				foreach($filehandle as $fp)
					fclose($fp);
			if(!empty($res))
				ksort($res);
			return $res;
		}
		
		public function post($url, $params=array())
		{
			if(is_array($params))
				curl_setopt($this->ch, CURLOPT_POSTFIELDS, http_build_query($params));
			else if(is_string($params))
				curl_setopt($this->ch, CURLOPT_POSTFIELDS, $params);
			curl_setopt($this->ch, CURLOPT_POST, true);
			curl_setopt($this->ch, CURLOPT_URL, $url);
			if(!empty($this->cookies))
				curl_setopt($this->ch, CURLOPT_COOKIE, http_build_query($this->cookies, '', '; ', PHP_QUERY_RFC3986));
			$content = curl_exec($this->ch);
			$this->status = curl_getinfo($this->ch);
			if($content === false)
				diemsg('[x] cURL error: '.$this->getErrorMsg($this->ch));
			//$content = mb_convert_encoding($content, 'utf-8', 'GBK,GB2312,UTF-8,ASCII');
			return $content;
		}
		
		
		
	};
?>