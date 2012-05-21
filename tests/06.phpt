--TEST--
Test hdf_get_value() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
hdf_set_value($hdf, 'test.string', 'foobar');
var_dump(hdf_get_value($hdf, 'test.string', ''));
hdf_set_value($hdf, 'test.integer', 101);
var_dump(hdf_get_value($hdf, 'test.integer', ''));
hdf_set_value($hdf, 'test.float', 101.345);
var_dump(hdf_get_value($hdf, 'test.float', ''));
hdf_set_value($hdf, 'test.array', array('foo' => 'bar', 'foobar'));
var_dump(hdf_get_value($hdf, 'test.array.foo', ''));
var_dump(hdf_get_value($hdf, 'test.array.0', ''));
?>
--EXPECT--
string(6) "foobar"
string(3) "101"
string(7) "101.345"
string(3) "bar"
string(6) "foobar"