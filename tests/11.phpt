--TEST--
Test hdf_obj_child() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
hdf_set_value($hdf, 'foo', 'bar');
var_dump(hdf_obj_child($hdf));
?>
--EXPECTF--
resource(%i) of type (ClearSilver HDF)