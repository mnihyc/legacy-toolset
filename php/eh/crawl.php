<?php
	include_once('cFunction.php');
	include_once('cRequest.php');
	include_once('cSqlite.php');
	
	function saveURL($url, $belong=null, $orgcontent='')
	{
		global $db;
		$num2 = basename($url);
		$num1 = basename(dirname($url));
		if(empty($num1) || empty($num2))
			diemsg("[x] Invalid url {$url} is given.");
		$url = CURL_EXH_MAINPAGE.'g/'.$num1.'/'.$num2.'/';
		if(intval($db->fetchColumn('select count(*) from data where url=:url',[':url' => $url])) != 0)
		{
			$obl = $db->fetchColumn('select belong from data where url=:url',[':url' => $url]);
			global $saveURL_overwrite;
			if(!$saveURL_overwrite)
			{
				echo "Already existed, don't overwrite.\n";
				return;
			}
		}
		else
		{
			$obl = '';
			$db->insert('data', ['url' => $url, 'downtime' => 0]);
		}
		while(is_null($belong) || strpos($belong, '|')!==false)
		{
			echo "What group/tag does this book belong to? [{$obl}] ";
			$belong = trim(getInput('belong'));
		}
		if(!empty($belong) && !empty($obl) && !in_array($belong, explode('|', $obl)))
			$belong = $obl.'|'.$belong;
		if(empty($belong))
			$belong = $obl;
		
		if(empty($orgcontent))
		{
			$page = new Request;
			InitializeRequest($page);
			$content = $page->get($url);
			if(intval($page->status['http_code']) == 404)
			{
				$db->delete('data', ['url' => $url]);
				echo "[-] Invalid url {$url} is given.";
			}
			else if(intval($page->status['http_code'])/100 == 5)
				diemsg("[x] Service unavailable. ({$page->status['http_code']})");
		}
		else
			$content = $orgcontent;
		$arr = GetContentBetween($content, 'background:transparent url(', ') 0 0 no-repeat');
		$thumbnail = $arr[0];
		$arr = GetContentBetween($content, '<h1 id="gj">', '</h1>');
		$titlejap = $arr[0];
		$arr = GetContentBetween($content, '<h1 id="gn">', '</h1>');
		$titleeng = $arr[0];
		$arr = GetContentBetween($content, "return popUp('", "',");
		$downway = str_replace('&amp;', '&', $arr[0]);
		$arr = GetContentBetween($content, '<div id="gdc">', '/div></div>');
		$arr = GetContentBetween($arr[0], '>', '<');
		$type = $arr[0];
		$arr = GetContentBetween($content, '<td class="gdt2">', '</td>');
		$posttime = $arr[0];
		$language = str_replace(' ', '', str_replace('&nbsp;', ' ', $arr[3]));
		$filesize = floatval($arr[4]);
		$arr = GetContentBetween($arr[5], '', ' pages');
		$pagenum = intval($arr[0]);
		$arr = GetContentBetween($content, 'Average: ', '</td>');
		$rating = floatval($arr[0]);
		$arr = GetContentBetween($content, 'td_parody:', '"');
		$parodytag = str_replace('_', ' ', implode('|', $arr));
		$arr = GetContentBetween($content, 'td_character:', '"');
		$charactertag = str_replace('_', ' ', implode('|', $arr));
		$arr = GetContentBetween($content, 'td_group:', '"');
		$grouptag = str_replace('_', ' ', implode('|', $arr));
		$arr = GetContentBetween($content, 'td_artist:', '"');
		$artisttag = str_replace('_', ' ', implode('|', $arr));
		$arr = GetContentBetween($content, 'td_male:', '"');
		$maletag = str_replace('_', ' ', implode('|', $arr));
		$arr = GetContentBetween($content, 'td_female:', '"');
		$femaletag = str_replace('_', ' ', implode('|', $arr));
		$arr = GetContentBetween($content, '<div id="td_', '"');
		$alltag = str_replace('_', ' ', implode('|', $arr));
		$arr = GetContentBetween($content, '<div class="c6" id="comment_0">', '</div>');
		$postercomment = html_entity_decode(str_replace('<br />', "\n", $arr[0]));
		
		$db->update('data', [
			'belong' => $belong,
			'downway' => $downway, 'thumbnail' => $thumbnail,
			'titlejap' => $titlejap, 'titleeng' => $titleeng,
			'type' => $type, 'posttime' => $posttime, 'filesize' => $filesize, 'pagenum' => $pagenum, 'rating' => $rating,
			'language' => $language, 'postercomment' => $postercomment,
			'parodytag' => $parodytag, 'charactertag' => $charactertag, 'grouptag' => $grouptag,
			'artisttag' => $artisttag, 'maletag' => $maletag, 'femaletag' => $femaletag, 'alltag' => $alltag,
			'updatetime' => time() ], [
			'url' => $url ]);
	}
	
	function crawlURL($url, $belong='')
	{
		while(empty($belong) || strpos($belong, '|')!==false)
		{
			echo "What group/tag does these books belong to? [{$obl}] ";
			$belong = trim(getInput('belong'));
		}
		
		$cnt = 0;
		recrawl:
		$page = new Request;
		InitializeRequest($page);
		$content = $page->get($url/*, ['inline_set' => 'dm_m']*/);
		$arr = GetContentBetween($content, '<a href="https://exhentai.org/g/', '"');
		$arr = array_unique($arr);
		foreach($arr as $key => $surl)
		{
			global $db;
			if(intval($db->fetchColumn('select count(*) from data where url=:url',[':url' => $surl])) != 0)
			{
				global $saveURL_overwrite;
				if(!$saveURL_overwrite)
					unset($arr[$key]);
			}
		}
		$arr1 = GetContentBetween($content, '<p class="ip">Showing ', ' results</p>');
		$total = intval($arr1[0]);
		echo "[+] Total {$total} results.\n";
		if(empty($arr))
			return;
		while($cnt < $total)
		{
			if(empty($arr))
				break;
			$ncnt = $cnt + min(CURL_MAX_REQUESTS, count($arr));
			echo "[*] Updating {$cnt}->{$ncnt}/{$total}...\n";
			
			$urls = [];
			foreach($arr as $key => $surl)
			{
				$cnt++;
				array_push($urls, 'https://exhentai.org/g/'.$surl);
				unset($arr[$key]);
				if($cnt == $ncnt)
					break;
			}
			
			$res = $page->multiget($urls, array(), '');
		
			foreach($res as $idx => $orgcontent)
				saveURL($urls[$idx], $belong, $orgcontent);
		}
		
		if(strpos($content, '<td class="ptdd">&gt;</td>') !== false)
			return;
		$arr = GetContentBetween($content, '<td onclick="document.location=this.firstChild.href"><a href="', '" onclick="return false">');
		echo "[*] Going on the next page...\n";
		$url = str_replace('&amp;', '&', $arr[count($arr)-1]);
		goto recrawl;
	}
	
	function updateURLs($url)
	{
		if(!is_array($url))
			$url = array($url);
		$total = count($url);
		echo "[+] Total {$total} results.\n";
		$cnt = 0;
		$page = new Request;
		InitializeRequest($page);
		while($cnt < $total)
		{
			if(empty($url))
				break;
			$ncnt = $cnt + min(CURL_MAX_REQUESTS, count($url));
			echo "[*] Updating {$cnt}->{$ncnt}/{$total}...\n";
			
			$urls = [];
			foreach($url as $key => $surl)
			{
				$cnt++;
				array_push($urls, $surl);
				unset($url[$key]);
				if($cnt == $ncnt)
					break;
			}
			$res = $page->multiget($urls, array(), '');
			foreach($res as $idx => $content)
				saveURL($urls[$idx], '', $content);
		}
	}
	
	function downURLs($urls=array())
	{
		global $db;
		$downway = $db->fetchColumn('select downway from config');
		$clientid = $db->fetchColumn('select clientid from config');
		$clientkey = $db->fetchColumn('select clientkey from config');
		if($downway !== 'h@h')
			$clientid = $clientkey = '';
		global $limit, $IMAGE_LIMIT_MAX;
		$limit = checkLimit();
		$tot = count($urls);
		$cnt = 0;
		foreach($urls as $url)
		{
			$cnt++;
			$name2 = basename($url);
			$name1 = basename(dirname($url));
			$sdir = DATA_DIR.'down/'."{$name1}.{$name2}/";
			if($downway !== 'archive')
			{
				echo "[+] Downloading {$cnt}/{$tot}, image limit {$limit}/{$IMAGE_LIMIT_MAX} ...\n";
				$files = downURL($url, $clientid, $clientkey);
				if(!file_exists($sdir))
					if(!mkdir($sdir,0755,true))
						diemsg("[x] Couldn't create directory ".$sdir);
				if(!copy($files[0], $sdir.'thumb.jpg'))
					diemsg("[x] Couldn't copy {$files[0]} to ".$sdir.'thumb.jpg');
				$zip = new ZipArchive();
				$flag = $zip->open($sdir.'arch.zip', ZipArchive::OVERWRITE);
				if($flag !== true)
					diemsg("Couldn't open ".$sdir.'arch.zip'." ({$flag})");
				for($i=0; $i<count($files); $i++)
					$zip->addFile($files[$i], "{$i}.jpg");
				$flag = $zip->close();
				for($i=0; $i<count($files); $i++)
					unlink($files[$i]);
			}
			else
			{
				echo "[+] Downloading {$cnt}/{$tot} ...\n";
				$res = $db->fetchColumn('select downway from data where url = :url', [':url' => $url]);
				//var_export($res);
				$file = downURL2($res);
				$zip = new ZipArchive();
				$flag = $zip->open($file, ZipArchive::CREATE);
				if($flag !== true)
					diemsg("Couldn't open {$file} ({$flag})");
				$fp = $zip->getStream($zip->getNameIndex(0));
				$fpo = fopen($sdir.'thumb.jpg', 'wb');
				while(!feof($fp))
				{
					$content = fread($fp, 1024);
					fwrite($fpo, $content);
				}
				fclose($fp);
				fclose($fpo);
				$flag = $zip->close();
				if(!copy($file, $sdir.'arch.zip'))
					diemsg("[x] Couldn't copy {$file} to ".$sdir.'arch.zip');
				
			}
			$db->update('data', [
				'downtime' => time()], [
				'url' => $url ]);
			echo "[+] Saved.\n";
		}
	}
	
	function downURL2($url)
	{
		$page = new Request;
		InitializeRequest($page);
		$content = $page->get($url);
		$arr = GetContentBetween($content, 'Download Cost: &nbsp; <strong>', '</strong>');
		$cost = intval($arr[0]);
		echo "[*] Download cost: {$cost} GP.\n";
		if(strpos($content, '<input type="submit" name="dlcheck" value="Download Original Archive" style="width:180px" />')===false ||
			strpos($content, '<input type="hidden" name="dltype" value="org" />')===false)
			diemsg('[x] Unknown error, maybe GP isn\'t enough.');
		$content = $page->post($url, ['dltype' => 'org', 'dlcheck' => 'Download Original Archive']);
		$arr = GetContentBetween($content, 'document.location = "', '"');
		$url = $arr[0] . '?start=1';
		$file = tempnam(sys_get_temp_dir(), 'save.zip');
		$page->download($url, $file, '');
		return $file;
	}
	
	function downURL($ourl, $clientid='', $clientkey='')
	{
		global $limit, $IMAGE_LIMIT_MAX;
		$files = [];
		$cnt = 0;
		recrawl:
		$page = new Request;
		InitializeRequest($page);
		$content = $page->get($ourl);
		$arr = GetContentBetween($content, '0 no-repeat"><a href="', '"');
		$arr1 = GetContentBetween($content, ' of ', ' images</p>');
		$tot = intval($arr1[0]);
		while($cnt < $tot)
		{
			if(empty($arr))
				break;
			$thiscount = min(CURL_MAX_DOWNLOADS, strval(count($arr)));
			$ncnt = $cnt + $thiscount;
			echo "[*] Found {$cnt}->{$ncnt}/{$tot} images.\n";
			onImageLimit($thiscount);
			$urls = [];
			foreach($arr as $key => $surl)
			{
				$cnt++;
				array_push($urls, $surl);
				unset($arr[$key]);
				if($cnt == $ncnt)
					break;
			}
			$round++;
			$res = $page->multiget($urls, array(), "[*] Round {$round} ");
			
			$sfile = [];
			if(empty($clientid) || empty($clientkey))
			{
				onImageLimit(16*$thiscount);
				foreach($res as $idx => &$url)
				{
					$sfile[$idx] = tempnam(sys_get_temp_dir(), 'save.jpg');
					array_push($files, $sfile[$idx]);
					$arr = GetContentBetween($url, '<a href="https://exhentai.org/fullimg.php', '"');
					$url = 'https://exhentai.org/fullimg.php'.str_replace('&amp;', '&', $arr[0]);
				}
			}
			else
			{
				foreach($res as $idx => &$url)
				{
					$act = 'dlfetch';
					$arr = GetContentBetween($url, ';fileindex=', ';');
					$add = basename(dirname($ourl)).';'.strval($cnt-$thiscount+$idx+1).';'.$arr[0].';org;0';
					$ctime = strval(time());
					$actkey = sha1('hentai@home-'.$act.'-'.$add.'-'.$clientid.'-'.$ctime.'-'.$clientkey);
					$querystr = 'clientbuild=142&act='.$act.'&add='.urlencode($add).'&cid='.$clientid.'&acttime='.$ctime.'&actkey='.$actkey;
					$url = 'http://rpc.hentaiathome.net/clientapi13.php?'.$querystr;
					var_export($url);die;
				}
				$npage = new Request;
				InitializeRequest($npage);
				$npage->setUserAgent('Hentai@Home 142');
				$round++;
				$res = $npage->multiget($res, array(), "[*] Round {$round} ");
				foreach($res as $idx => &$url)
				{
					$sfile[$idx] = tempnam(sys_get_temp_dir(), 'save.jpg');
					array_push($files, $sfile[$idx]);
					$arr = explode("\n", str_replace("\r", '', $url));
					if($arr[0] !== 'OK')
						diemsg("[x] Download failed because {$arr[0]} ...\n");
					$url = $arr[1];
				}
			}
			$round++;
			$res = $page->multiget($res, $sfile, "[*] Round {$round} ");
		}
		
		if(strpos($content, '<td class="ptdd">&gt;</td>') !== false)
			return $files;
		$arr = GetContentBetween($content, '<td onclick="document.location=this.firstChild.href"><a href="', '" onclick="return false">');
		echo "Going on the next page...\n";
		$url = 'https://exhentai.org/g/'.$arr[count($arr)-1];
		goto recrawl;
	}
	
	function onImageLimit($nlimit)
	{
		global $limit, $IMAGE_LIMIT_MAX;
		recheck:
		if($limit+$nlimit <= $IMAGE_LIMIT_MAX)
		{
			$limit = $limit + $nlimit;
			return;
		}
		echo "[*] Will exceed the image limit {$limit}/{$IMAGE_LIMIT_MAX} ...\n";
		while(empty($con) || ($con!=='yes' && $con!=='no'))
		{
			echo "Change an IP and continue? [yes/no] ";
			$con = getInput('con');
		}
		if($con === 'yes')
		{
			$limit = checkLimit();
			goto recheck;
		}
		else
			diemsg("[x] Exceed the image limit {$IMAGE_LIMIT_MAX}");
	}
	
	function checkLimit()
	{
		global $limit, $IMAGE_LIMIT_MAX;
		$page = new Request;
		InitializeRequest($page);
		$content = $page->get(CURL_EXH_HOMEURL);
		$arr = GetContentBetween($content, 'towards a limit of <strong>', '</strong>');
		$IMAGE_LIMIT_MAX = intval($arr[0]);
		$arr = GetContentBetween($content, 'You are currently at <strong>', '</strong>');
		$cnt = intval($arr[0]);
		echo "[*] Image limit: {$cnt}/{$IMAGE_LIMIT_MAX} ...\n";
		return $cnt;
	}
	
	

?>