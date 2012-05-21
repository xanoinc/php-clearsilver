<?
if(!extension_loaded('clearsilver')) {
	dl('clearsilver.' . PHP_SHLIB_SUFFIX);
}
$module = 'clearsilver';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:<br>\n";
foreach($functions as $func) {
    echo $func."<br>\n";
}
echo "<br>\n";
?>
