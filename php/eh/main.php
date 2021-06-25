<?php
	include_once('cFunction.php');
	set_time_limit(0);
	ob_end_flush();
	ob_implicit_flush(true);
	if(isBrowser() && strpos($_SERVER['USER-AGENT'],'Firefox')===false)
		echo "<script>alert(\"We strongly recommend you to use Firefox, \\n or data losses could happen.\");</script>";
	if(isBrowser())
		echo "<pre>\n";
	
	include_once('cRequest.php');
	include_once('cSqlite.php');
	include_once('crawl.php');
	include_once('cProgBar.php');

	if(!file_exists(DATA_DIR))
	{
		echo "[*] No data directory is found, creating new one.\n";
		if(!mkdir(DATA_DIR,0755,false))
			diemsg("[x] Couldn't create data directory ".DATA_DIR);
	}
	/* Access local database */
	$db = new Database;
	$db->connect('sqlite:'.DATA_DIR.'data.db');
	$tbnum=intval($db->fetchColumn('select count(*) from sqlite_master where type=:type and name=:name',
		[':type' => 'table', ':name' => 'data']));
	if(!$tbnum)
	{
		echo "[*] No table 'data' is found, creating new one.\n";
		$db->query(CREATE_TABLE_SQL);
	}
	$tbnum=intval($db->fetchColumn('select count(*) from sqlite_master where type=:type and name=:name',
		[':type' => 'table', ':name' => 'config']));
	if(!$tbnum)
	{
		echo "[*] No table 'config' is found, creating new one.\n";
		$db->query(CREATE_TABLE_SQL1);
		$db->insert('config', ['username' => '']);
	}
	
	echo "[+] Access local database successfully.\n";
	
	/* Set config settings */
	if(!$tbnum)
	{
		echo "\n[*] First time to come here?\n[+] Finish the config setttings!\n";
		SetConfigs();
	}
	
	/* Route commands */
	echo "\nWhat to do? [exit, config, purge, add, update, query, sync, down] ";
	$cmd = getInput('cmd');
	if($cmd == 'config')
	{
		echo "View or what to edit? [view, all, user, proxy, down] ";
		$type = strtolower(trim(getInput('type')));
		if($type == 'view')
			var_export($db->fetch('select * from config'));
		else
		{
			if($type == 'all')
				$type='';
			SetConfigs($type);
		}
	}
	if($cmd == 'purge')
	{
		echo "[*] Are you sure?\nThis will cause permanent data loss! [I'm sure, else exit] ";
		$sure = getInput('sure');
		if($sure == "I'm sure")
		{
			$db->close();
			if(!unlink(DATA_DIR.'data.db'))
				diemsg('[x] Access denied!');
		}
	}
	if($cmd == 'query')
	{
		echo "Input SQL query: (select count(*) from data)[] ";
		$sql = trim(getInput('sql'));
		var_export($db->fetchAll($sql));
	}
	if($cmd == 'update' || $cmd == 'add')
	{
		echo "{$cmd} through ... [all, url, crawl, sql, fav] ";
		global $saveURL_overwrite;
		$saveURL_overwrite = (bool)($cmd == 'add');
		$type = strtolower(trim(getInput('type')));
		if($type == 'url')
		{
			echo "Input url needed to be updated (https://exh.org/g/xxx/xxx/)[] ";
			$url = getInput('url');
			saveURL($url);
		}
		else if($type == 'all')
			updateURLs($db->fetchAllColumn('select url from data'));
		else if($type == 'crawl')
		{
			echo "Input url needed to be crawled (https://exh.org/?f_s=xxx)[] ";
			$url = getInput('url');
			crawlURL($url);
		}
		else if($type == 'sql')
		{
			echo "Input where query: (updatetime < xxx)[] ";
			$sql = trim(getInput('sql'));
			updateURLs($db->fetchAllColumn('select url from data where '.$sql));
		}
		else if($type == 'fav')
			for($i=0; $i<=9; $i++)
			{
				echo "[+] Crawling favorite{$i}...\n";
				crawlURL(CURL_EXH_FAVURL.strval($i), "favorite{$i}");
			}
	}
	if($cmd == 'sync')
	{
		$db->update('data', ['downtime' => 0]);
		$tot = intval($db->fetchColumn('select count(*) from data'));
		echo "[*] Total {$tot} records.\n";
		$dirs = scandir(DOWN_DIR);
		$cnt = 0;
		foreach($dirs as $dir)
		{
			if($dir=='.' || $dir=='..')
				continue;
			if(file_exists(DOWN_DIR.$dir.'/arch.zip') && file_exists(DOWN_DIR.$dir.'/thumb.jpg'))
				$db->update('data', ['downtime' => time()], [
					'url' => 'https://exhentai.org/g/'.str_replace('.', '/', $dir) ]);
			$cnt++;
			echo ".";
		}
		echo "\n";
		echo "{$cnt}/{$tot} is downloaded.\n";
	}
	if($cmd == 'down')
	{
		echo "Download ... [all, sql, new] ";
		$type = strtolower(trim(getInput('type')));
		if($type == 'all')
			downURLs($db->fetchAllColumn('select url from data'));
		else if($type == 'sql')
		{
			echo "Input where query: (alltag like '%xxx%')[] ";
			$sql = trim(getInput('sql'));
			downURLs($db->fetchAllColumn('select url from data where '.$sql));
		}
		else if($type == 'new')
			downURLs($db->fetchAllColumn('select url from data where downtime=0'));
	}
	

	diemsg("\n[+] The specified command was successfully ended.",false);
	exit();
?>