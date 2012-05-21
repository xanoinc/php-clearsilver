--TEST--
Test hdf_read_file() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
var_dump(hdf_read_file($hdf, dirname(__FILE__).'/test.hdf'));
?>
--EXPECT--
bool(true)