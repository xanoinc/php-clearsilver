--TEST--
Test hdf_set_value() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
var_dump(hdf_set_value($hdf, 'test.string', 'foobar'));
var_dump(hdf_set_value($hdf, 'test.integer', 101));
var_dump(hdf_set_value($hdf, 'test.float', 101.345));
var_dump(hdf_set_value($hdf, 'test.array', array('foo' => 'bar', 'string')));
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)