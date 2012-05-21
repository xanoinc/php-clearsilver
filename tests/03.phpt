--TEST--
Test hdf_write_file() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
chdir(dirname(__FILE__));
hdf_read_file($hdf, 'test.hdf');
var_dump(hdf_write_file($hdf, 'temp.hdf'));
var_dump(unlink('temp.hdf'));
?>
--EXPECT--
bool(true)
bool(true)