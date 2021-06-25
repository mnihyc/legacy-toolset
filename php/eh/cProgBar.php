<?php
	
	class ProgressBar
	{
		private $total = null;
		private $curcnt = null;
		private $length = null;
		private $preShow = '';
		
		public function __construct()
		{
			$this->total = 0;
			$this->curcnt = 0;
			$this->length = 40;
		}
		
		public function setPreShow($str = '')
		{
			$this->preShow = $str;
			return $this;
		}
		
		public function setTotal($tot = 0)
		{
			$this->total = intval($tot);
			return $this;
		}
		
		public function setLength($len)
		{
			$this->length = intval($len);
			return $this;
		}
		
		public function setCurrent($cur = 0)
		{
			$this->curcnt = $cur;
			return $this;
		}
		
		public function getCurrent()
		{
			return $this->curcnt;
		}
		
		public function clear()
		{
			$this->draw( ' ', ' ', ' ', ' ', ' ', ' ', ' ', true);
			return $this;
		}
		
		public function draw($char1='[', $char2=']', $char3='=', $char4='>', $char5='(', $char6=')', $char7='/', $clear=false)
		{
			if($this->total)
				$percent = $this->curcnt / $this->total;
			else
				$percent = 0;
			$takecnt = intval(round($this->length * $percent));
			$str = $this->preShow;
			$str.=" {$char1}".str_repeat($char3, $takecnt);
			if($takecnt < $this->length)
				$str.=$char4.str_repeat(' ',$this->length - $takecnt - 1);
			if(!$clear)
				$str.="{$char2} {$char5}".number_format($this->curcnt).$char7.number_format($this->total).$char6;
			else
				$str.="   ".str_repeat(' ', strlen(number_format($this->curcnt)) + strlen(number_format($this->total)) + 2);
			echo "\r{$str}";
		}
	};
	
?>