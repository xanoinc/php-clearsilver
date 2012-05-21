--TEST--
Test cs_init() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
var_dump(cs_init($hdf));
?>
--EXPECTF--
resource(%i) of type (ClearSilver CS)