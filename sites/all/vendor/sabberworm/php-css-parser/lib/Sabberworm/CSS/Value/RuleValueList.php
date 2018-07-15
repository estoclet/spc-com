<?php /*457563643457563643*/ ?><?php  ?><?php  ?><?php

namespace Sabberworm\CSS\Value;

class RuleValueList extends ValueList {
	public function __construct($sSeparator = ',', $iLineNo = 0) {
		parent::__construct(array(), $sSeparator, $iLineNo);
	}
}