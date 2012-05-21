--TEST--
Test cs_parse_file() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
hdf_set_value($hdf, 'message', 'Hello World!');
$cs = cs_init($hdf);
chdir(dirname(__FILE__));
var_dump(cs_parse_file($cs, 'test.cs'));
?>
--EXPECT--
bool(true)