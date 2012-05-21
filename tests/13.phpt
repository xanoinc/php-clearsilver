--TEST--
Test hdf_get_child() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
hdf_set_value($hdf, 'parentNode.childNode', 'child value');
var_dump(hdf_get_child($hdf, 'parentNode'));
?>
--EXPECTF--
resource(%i) of type (ClearSilver HDF)