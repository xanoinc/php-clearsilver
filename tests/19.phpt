--TEST--
Test cs_destroy() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
$cs = cs_init($hdf);
var_dump($cs);
cs_destroy($cs);
var_dump($cs);
?>
--EXPECTF--
resource(%i) of type (ClearSilver CS)
resource(%i) of type (Unknown)