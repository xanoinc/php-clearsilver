--TEST--
Test cs_parse_string() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
hdf_set_value($hdf, 'message', 'Hello World!');
$cs = cs_init($hdf);

$template = 'Your message is "<?cs var message ?>".';
var_dump(cs_parse_string($cs, $template));
?>
--EXPECT--
bool(true)