--TEST--
Test hdf_destroy() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
var_dump($hdf);
hdf_destroy($hdf);
var_dump($hdf);
?>
--EXPECTF--
resource(%i) of type (ClearSilver HDF)
resource(%i) of type (Unknown)