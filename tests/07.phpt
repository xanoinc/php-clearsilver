--TEST--
Test hdf_get_node() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
var_dump(hdf_get_node($hdf, 'test.string'));
?>
--EXPECTF--
resource(%i) of type (ClearSilver HDF)