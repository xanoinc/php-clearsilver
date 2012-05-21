--TEST--
Test hdf_init() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
var_dump(hdf_init());
?>
--EXPECTF--
resource(%i) of type (ClearSilver HDF)