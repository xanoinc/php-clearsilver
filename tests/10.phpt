--TEST--
Test hdf_obj_value() behavior
--SKIPIF--
<?php if (!extension_loaded("clearsilver")) print "skip clearsilver extension not loaded"; ?>
--FILE--
<?php
$hdf = hdf_init();
hdf_set_value($hdf, '', 'my value');
var_dump(hdf_obj_value($hdf));
?>
--EXPECT--
string(8) "my value"