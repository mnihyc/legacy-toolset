<?php
	include_once('cFunction.php');
	if(!extension_loaded('pdo_sqlite'))
		diemsg('[-] Needs pdo_sqlite being enabled!');
	class Database
	{
		private $dbh = null;
		private $sth = null;
		
		public $lastSQL = null;
		
		private function watchException($execute_state)
		{
			if(!$execute_state || $execute_state===false)
				diemsg("SQL Halt: {$this->lastSQL}\n".var_export($this->dbh->errorInfo(),true)."\n");
			return $execute_state;
		}
		
		private function format_table_name($table)
		{
			$parts = explode('.', $table, 2);
			if(count($parts) > 1)
				return $parts[0]."`{$parts[1]}`";
			else
				return "`{$table}`";
		}
		
		public function connect($dsn, $user='', $pass='', $charset='utf-8')
		{
			if($this->dbh)
				return;
			$this->dbh = new PDO($dsn, $user, $pass);
			return $this;
		}
		
		public function close()
		{
			$this->dbh = null;
			return $this;
		}
		
		public function fetchAll($sql, $params=array())
		{
			$result = array();
			$this->lastSQL = $sql;
			$this->sth = $this->watchException($this->dbh->prepare($sql));
			$this->watchException($this->sth->execute($params));
			while($result[] = $this->sth->fetch(PDO::FETCH_ASSOC));
			array_pop($result);
			return $result;
		}
		
		public function fetchAllColumn($sql, $params=array(), $position=0)
		{
			$result = array();
			$this->lastSQL = $sql;
			$this->sth = $this->watchException($this->dbh->prepare($sql));
			$this->watchException($this->sth->execute($params));
			while($result[] = $this->sth->fetch(PDO::FETCH_COLUMN, $position));
			array_pop($result);
			return $result;
		}
		
		public function exists($sql, $params=array())
		{
			$this->lastSQL = $sql;
			$data = $this->fetch($sql, $params);
			return !empty($data);
		}
		
		public function query($sql, $params=array())
		{
			$this->lastSQL = $sql;
			$this->sth = $this->watchException($this->dbh->prepare($sql));
			$this->watchException($this->sth->execute($params));
			return $this->sth->rowCount();
		}
		
		public function fetch($sql, $params=array(), $type=PDO::FETCH_ASSOC)
		{
			$this->lastSQL = $sql;
			$this->sth = $this->watchException($this->dbh->prepare($sql));
			$this->watchException($this->sth->execute($params));
			return $this->sth->fetch($type);
		}
		
		public function fetchColumn($sql, $params=array(), $position=0)
		{
			$this->lastSQL = $sql;
			$this->sth = $this->watchException($this->dbh->prepare($sql));
			$this->watchException($this->sth->execute($params));
			return $this->sth->fetch(PDO::FETCH_COLUMN, $position);
		}
		
		public function update($table, $params=array(), $cond=array())
		{
			$table = $this->format_table_name($table);
			$sql = "UPDATE {$table} SET ";
			$fields = array();
			$pdo_params = array();
			foreach($params as $field => $value)
			{
				$fields[] = "`{$field}`=:field_{$field}";
				$pdo_params["field_{$field}"] = $value;
			}
			$sql.=implode(',', $fields);
			$fields = array();
			$where = '';
			if(is_string($cond))
				$where = $cond;
			else if(is_array($cond))
			{
				foreach($cond as $field => $value)
				{
					$fields[] = "`{$field}`=:cond_{$field}";
					$pdo_params["cond_{$field}"] = $value;
				}
				$where = implode(' AND ', $fields);
			}
			if(!empty($where))
				$sql.=' WHERE '.$where;
			return $this->query($sql, $pdo_params);
		}
		
		public function insert($table, $params=array())
		{
			$table = $this->format_table_name($table);
			$sql = "INSERT INTO {$table} ";
			$fields = array();
			$placeholder = array();
			foreach($params as $field => $value)
			{
				$placeholder[] = ":{$field}";
				$fields[] = "`{$field}`";
			}
			$sql.='('.implode(',', $fields).') VALUES ('.implode(',', $placeholder).')';
			
			$this->lastSQL = $sql;
			$this->sth = $this->watchException($this->dbh->prepare($sql));
			$this->watchException($this->sth->execute($params));
			$id = $this->dbh->lastInsertId();
			if(empty($id))
				return $this->sth->rowCount();
			else
				return $id;
		}
		
		public function delete($table, $params=array())
		{
			$table = $this->format_table_name($table);
			$sql = "DELETE FROM {$table}";
			$fields = array();
			$pdo_params = array();
			foreach($params as $field => $value)
			{
				$fields[] = "`{$field}`=:field_{$field}";
				$pdo_params["field_{$field}"] = $value;
			}
			$where = implode(' AND ', $fields);
			if(!empty($where))
				$sql.=' WHERE '.$where;
			return $this->query($sql, $pdo_params);
		}
		
		
	};
?>