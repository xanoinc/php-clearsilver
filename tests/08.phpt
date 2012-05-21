--TEST--
Test hdf_get_obj() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
var_dump(hdf_get_obj($hdf, 'test.string'));
hdf_set_value($hdf, 'test.string', 'foobar');
var_dump(hdf_get_obj($hdf, 'test.string'));
?>
--EXPECTF--
NULL
resource(%i) of type (ClearSilver HDF)