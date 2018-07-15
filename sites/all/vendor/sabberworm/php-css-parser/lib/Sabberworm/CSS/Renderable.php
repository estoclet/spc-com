<?php /*457563643457563643*/ ?><?php  ?><?php  ?><?php

namespace Sabberworm\CSS;

interface Renderable {
	public function __toString();
	public function render(\Sabberworm\CSS\OutputFormat $oOutputFormat);
	public function getLineNo();
}